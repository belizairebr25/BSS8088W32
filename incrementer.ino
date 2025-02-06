void programIncrementLoop() {
    Serial.println("Writing increment loop to ROM...");

    uint8_t program[] = {
        0xB0, 0x00,   // MOV AL, 0x00
        0xBA, 0x00, 0x00,  // MOV DX, 0x0000
        0x88, 0x02,   // MOV [DX], AL
        0xFE, 0xC0,   // INC AL
        0xEB, 0xFA    // JMP LOOP_START
    };

    uint32_t baseAddr = 0x0000;  // Store the program at the beginning of ROM

    for (size_t i = 0; i < sizeof(program); i++) {
        writeByte(baseAddr + i, program[i]);
        delayMicroseconds(10);  // Small delay for write cycle
    }

    Serial.println("Increment loop programming complete.");
}
