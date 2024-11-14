#include "eeprom_main.h"

uint32_t calculateCRC(const uint8_t* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc ^ 0xFFFFFFFF;
}

void writeEEPROMPage(uint16_t addr, const uint8_t* data, size_t length) {
    size_t offset = 0;
    while (length > 0) {
        size_t chunkSize = PAGE_SIZE - (addr % PAGE_SIZE);
        if (chunkSize > length) chunkSize = length;

        uint8_t txbuf[chunkSize + 2];
        txbuf[0] = (uint8_t)(addr >> 8);
        txbuf[1] = (uint8_t)(addr & 0xFF);
        std::memcpy(&txbuf[2], &data[offset], chunkSize);

        i2cAcquireBus(I2C_EEPROM);
        i2cMasterTransmitTimeout(I2C_EEPROM, EEPROM_ADDRESS, txbuf, chunkSize + 2, NULL, 0, TIME_MS2I(100));
        i2cReleaseBus(I2C_EEPROM);
        chThdSleepMilliseconds(5);

        addr += chunkSize;
        offset += chunkSize;
        length -= chunkSize;
    }
}

template <typename T>
void writeDataToEEPROM(uint16_t startAddr, const T& data) {
    uint8_t buffer[sizeof(T)];
    std::memcpy(buffer, &data, sizeof(T));
    uint32_t crc = calculateCRC(buffer, sizeof(T));
    uint8_t crcBuffer[sizeof(crc)];
    std::memcpy(crcBuffer, &crc, sizeof(crc));

    // Запись данных
    writeEEPROMPage(startAddr, buffer, sizeof(T));
    // Запись CRC
    writeEEPROMPage(startAddr + sizeof(T), crcBuffer, sizeof(crc));
}

uint8_t readEEPROM(uint16_t addr) {
    uint8_t txbuf[2] = { (uint8_t)(addr >> 8), (uint8_t)(addr & 0xFF) };
    uint8_t rxbuf = 0xFF;
    i2cAcquireBus(I2C_EEPROM);
    i2cMasterTransmitTimeout(I2C_EEPROM, EEPROM_ADDRESS, txbuf, 2, &rxbuf, 1, TIME_MS2I(100));
    i2cReleaseBus(I2C_EEPROM);
    return rxbuf;
}

template <typename T>
bool readDataFromEEPROM(uint16_t startAddr, T& data) {
    uint8_t buffer[sizeof(T)];
    uint8_t crcBuffer[sizeof(uint32_t)];

    // Чтение данных
    for (size_t i = 0; i < sizeof(T); ++i) {
        buffer[i] = readEEPROM(startAddr + i);
    }
    // Чтение CRC
    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        crcBuffer[i] = readEEPROM(startAddr + sizeof(T) + i);
    }

    uint32_t storedCrc;
    std::memcpy(&storedCrc, crcBuffer, sizeof(storedCrc));
    uint32_t calculatedCrc = calculateCRC(buffer, sizeof(T));

    if (storedCrc == calculatedCrc) {
        std::memcpy(&data, buffer, sizeof(T));
        return true;
    } else {
        return false;
    }
}
