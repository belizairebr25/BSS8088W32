//CHATGPT generated program to erase all sectors of the SST39SF010A Flash chip
//For Arduino Mega; bus pins are in order
// Define pin assignments based on your wiring
const int ADDR_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5};  // Address lines A0-A18
const int DATA_PINS[] = {22, 23, 24, 25, 26, 27, 28, 29};  // D0-D7
const int CE = 30;   // Chip Enable
const int OE = 31;   // Output Enable
const int WE = 32;   // Write Enable

void setup() {
    Serial.begin(115200);

    // Set address and data bus as outputs
    for (int i = 0; i < 18; i++) {
        pinMode(ADDR_PINS[i], OUTPUT);
    }
    for (int i = 0; i < 8; i++) {
        pinMode(DATA_PINS[i], OUTPUT);
    }

    pinMode(CE, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(WE, OUTPUT);

    digitalWrite(CE, HIGH);
    digitalWrite(OE, HIGH);
    digitalWrite(WE, HIGH);

    eraseFlash();
    verifyErase();
}

void loop() {
    // Nothing here, everything runs in setup
}

// Function to send a command to the flash chip
void writeCommand(uint32_t addr, uint8_t data) {
    setAddress(addr);
    setData(data);

    digitalWrite(CE, LOW);
    digitalWrite(WE, LOW);
    delayMicroseconds(1);
    digitalWrite(WE, HIGH);
    digitalWrite(CE, HIGH);
}

// Set the address lines
void setAddress(uint32_t addr) {
    for (int i = 0; i < 18; i++) {
        digitalWrite(ADDR_PINS[i], (addr >> i) & 1);
    }
}

// Set the data lines
void setData(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(DATA_PINS[i], (data >> i) & 1);
    }
}

// Function to erase the flash memory
void eraseFlash() {
    Serial.println("Starting chip erase...");

    // SST39SF010A erase command sequence
    writeCommand(0x5555, 0xAA);
    writeCommand(0x2AAA, 0x55);
    writeCommand(0x5555, 0x80);
    writeCommand(0x5555, 0xAA);
    writeCommand(0x2AAA, 0x55);
    writeCommand(0x5555, 0x10);  // Chip Erase command

    delay(100);  // Wait for erase operation to complete

    Serial.println("Erase command sent, verifying...");
}

// Read data from the flash chip
uint8_t readData(uint32_t addr) {
    setAddress(addr);

    for (int i = 0; i < 8; i++) {
        pinMode(DATA_PINS[i], INPUT);
    }

    digitalWrite(CE, LOW);
    digitalWrite(OE, LOW);
    delayMicroseconds(1);

    uint8_t data = 0;
    for (int i = 0; i < 8; i++) {
        data |= (digitalRead(DATA_PINS[i]) << i);
    }

    digitalWrite(OE, HIGH);
    digitalWrite(CE, HIGH);

    for (int i = 0; i < 8; i++) {
        pinMode(DATA_PINS[i], OUTPUT);
    }

    return data;
}

// Function to verify the erase operation
void verifyErase() {
    bool success = true;
    for (uint32_t addr = 0; addr < 0x20000; addr++) {  // 128KB memory size
        uint8_t value = readData(addr);
        if (value != 0xFF) {
            Serial.print("Erase failed at address 0x");
            Serial.print(addr, HEX);
            Serial.print(" Read: 0x");
            Serial.println(value, HEX);
            success = false;
            break;
        }
    }

    if (success) {
        Serial.println("Erase successful, all bytes are 0xFF.");
    } else {
        Serial.println("Erase failed, check wiring or power stability.");
    }
}
