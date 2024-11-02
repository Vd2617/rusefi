#pragma once

/* This is a class for an instantaneous fuel consumption sensor in liters per hour
 * Has init file in init/sensors 
 * 
 */

#include "sensor.h"
#include "sensor_type.h"
#include <cstdint>

class FuelConsumptionSensor final : public Sensor{

public:	
	FuelConsumptionSensor(SensorType type,float injectionFlowRate,uint8_t numCylindres)
	: Sensor(type),m_injectionFlowRate(injectionFlowRate),m_numCylindres(numCylindres) {}
	
	~FuelConsumptionSensor(){}

	SensorResult get() const final override {

		float fuelDutyCycle  = 300; //for code test replace real value
		
		auto  decimalFuelDuty = fuelDutyCycle / 100.0 ;

		auto rpm = Sensor::getOrZero(SensorType::Rpm);

		return decimalFuelDuty * m_injectionFlowRate * m_numCylindres * rpm / 60.0;
	}

	void showInfo(const char*) const final override {} 


private:

	float m_injectionFlowRate; // cc/min

	uint8_t m_numCylindres;

};