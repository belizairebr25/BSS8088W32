//CHATGPT generated program to write the No Operation command to every byte on the SST39SF010A
//Program runs on Arduino Mega, bus pins are in order
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

    programNOP();
    verifyNOP();
    Serial.println("Programming and verification complete!");
}

void loop() {}

// Function to set the address lines
void setAddress(uint32_t addr) {
    for (int i = 0; i < 18; i++) {
        digitalWrite(ADDR_PINS[i], (addr >> i) & 1);
    }
}

// Function to set the data lines
void setData(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(DATA_PINS[i], (data >> i) & 1);
    }
}

// Function to write a byte to ROM
void writeByte(uint32_t addr, uint8_t data) {
    setAddress(addr);
    setData(data);

    digitalWrite(CE, LOW);
    digitalWrite(WE, LOW);
    delayMicroseconds(1);  // Small delay for write cycle
    digitalWrite(WE, HIGH);
    digitalWrite(CE, HIGH);
}

// Function to read a byte from ROM
uint8_t readByte(uint32_t addr) {
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

// Function to write NOP (0x90) to every address
void programNOP() {
    Serial.println("Writing NOPs to ROM...");
    for (uint32_t addr = 0; addr < (1 << 17); addr++) {  // 128KB size SST39SF010A
        writeByte(addr, 0x90);
        delayMicroseconds(10);  // Small delay for write cycle
    }
    Serial.println("NOP programming complete.");
}

// Function to verify NOP (0x90) in every address
void verifyNOP() {
    Serial.println("Verifying NOPs in ROM...");
    for (uint32_t addr = 0; addr < (1 << 17); addr++) {
        uint8_t data = readByte(addr);
        if (data != 0x90) {
            Serial.print("Verification failed at address: ");
            Serial.println(addr, HEX);
            return;
        }
    }
    Serial.println("NOP verification successful.");
}
