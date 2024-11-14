/* usage

	INIT EEPROM 

	i2cStart(I2C_EEPROM, &i2ccfg);//in function main

	READ FROM EEPROM

	MotorHours motorHoursPrimary, motorHoursBackup;

	bool primaryValid = readDataFromEEPROM(0, motorHoursPrimary);
		bool backupValid = readDataFromEEPROM(BACKUP_OFFSET, motorHoursBackup);

		if (primaryValid) {
			if (!backupValid || motorHoursPrimary.hours >= motorHoursBackup.hours) {
				writeDataToEEPROM(BACKUP_OFFSET, motorHoursPrimary);
			}
		} else if (backupValid) {
			motorHoursPrimary = motorHoursBackup;
			writeDataToEEPROM(0, motorHoursPrimary);
		} else {
			motorHoursPrimary.hours = 0;
			writeDataToEEPROM(0, motorHoursPrimary);
			writeDataToEEPROM(BACKUP_OFFSET, motorHoursPrimary);
		}

	WRITE TO EEPROM

	MotorHours motorHoursPrimary, motorHoursBackup;

	motorHoursPrimary.hours += 1;
    writeDataToEEPROM(0, motorHoursPrimary);
    writeDataToEEPROM(BACKUP_OFFSET, motorHoursPrimary);



*/
#pragma once

#include "ch.h"
#include "hal.h"
#include <cstring>
#include <cstdint>

static const I2CConfig i2ccfg = {
    OPMODE_I2C,//mode type i2c
    400000, // frequency kHz
    FAST_DUTY_CYCLE_2, // coafficient work cycle
};

uint32_t calculateCRCForEeprom(const uint8_t* data, size_t length);

void writeEEPROMPage(uint16_t addr, const uint8_t* data, size_t length);

uint8_t readEEPROM(uint16_t addr);

template <typename T>
bool readDataFromEEPROM(uint16_t startAddr, T& data);

template <typename T>
void writeDataToEEPROM(uint16_t startAddr, const T& data);
