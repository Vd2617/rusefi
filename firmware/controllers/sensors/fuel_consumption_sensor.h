#pragma once

/* This is a class for an instantaneous fuel consumption sensor in liters per hour
 * Has init file in init/sensors 
 * 
 */

#include "sensor.h"
#include "sensor_type.h"
#include "fuel_math.h"
#include "injector_model.h"
#include "fuel_computer.h"
#include "rusefi_types.h"
#include <cstdint>

class FuelConsumptionSensor final : public Sensor{

public:	
	FuelConsumptionSensor(SensorType type)
	: Sensor(type) {}
	
	~FuelConsumptionSensor(){}

	void initSensor(float injectionFlowRate, uint16_t numCylindres){
		
		m_injectionFlowRate = injectionFlowRate;

		m_numCylindres = numCylindres;
	}

	// Продолжительность одного цикла в секундах (два оборота для четырехтактного двигателя)
	float getEngineCycleDuration(float rpm) {
		return 60.0f / (rpm / 2.0f); 
	}
	
	// Функция для расчета общего времени впрыска за минуту для всех 6 цилиндров
	float getTotalInjectionTimePerMinute(float dutyCycle, float rpm) {
		float cycleDuration = getEngineCycleDuration(rpm);
		float cyclesPerMinute = rpm / 2.0f;
		auto result = (dutyCycle / 100.0f) * cycleDuration * cyclesPerMinute * m_numCylindres ;
		return result;
	}
	
	// Функция для расчета общего расхода топлива в литрах за час
	float getFuelConsumptionPerHour(float dutyCycle, float rpm, float flowRate) {
		float totalInjectionTimePerMinute = getTotalInjectionTimePerMinute(dutyCycle, rpm); // seconds
		float totalFlowPerMinute = totalInjectionTimePerMinute / 60.0f * flowRate; 
		return (totalFlowPerMinute / 1000.0f) * 60.0f / m_fuelDensity; // Переводим в литры в час
	}

	SensorResult get() const final override {

		float rpm = Sensor::getOrZero(SensorType::Rpm);

		float dutyCycle = getInjectorDutyCycle(rpm);

		float fuelConsumption = getFuelConsumptionPerHour(dutyCycle, rpm, m_injectionFlowRate);

		return fuelConsumption;
	}

	void showInfo(const char*) const final override {} 


private:

	float m_injectionFlowRate = 0.0f; // cc/min

	uint16_t m_numCylindres = 0;

	constexpr float m_fuelDensity = 0.72;

};