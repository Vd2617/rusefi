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

		float fuelDutyCycle  = getInjectorDutyCycle(Sensor::getOrZero(SensorType::Rpm));

		return fuelDutyCycle;

		/*auto  decimalFuelDuty = fuelDutyCycle / 100.0 ;

		float realInjectionFlowRate = m_injectionFlowRate * decimalFuelDuty;

		float injectionLitresPerHour = (realInjectionFlowRate * fuelDensity * 60.0f) / (fuelDensity * 1000.0f);

		float fuelLitresHour = injectionLitresPerHour * m_numCylindres ; 
		
		return (fuelLitresHour < 0.0f) ? 0.0f : fuelLitresHour;*/
	}

	void showInfo(const char*) const final override {} 


private:

	float m_injectionFlowRate = 0.0f; // cc/min

	uint16_t m_numCylindres = 0;

};