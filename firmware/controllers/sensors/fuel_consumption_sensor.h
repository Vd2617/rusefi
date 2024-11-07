#pragma once

/* This is a class for an instantaneous fuel consumption sensor in liters per hour
 * Has init file in init/sensors 
 * 
 */

#include "sensor.h"
#include "sensor_type.h"
#include "fuel_math.h"
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

	SensorResult get() const final override {

		auto rpm = Sensor::getOrZero(SensorType::Rpm);

		float fuelDutyCycle  = getInjectorDutyCycle(rpm);

		auto  decimalFuelDuty = fuelDutyCycle / 100.0 ;

        float result = decimalFuelDuty * m_injectionFlowRate * m_numCylindres * rpm * 60.0;
		
		if(result < 0)
			return 0.0f;
		
		return result;
	}

	void showInfo(const char*) const final override {} 


private:

	float m_injectionFlowRate = 0.0f; // cc/min

	uint16_t m_numCylindres = 0;

};