//#include "pch.h"
#include "fuel_consumption_sensor.h"
#include "init.h"

const float FUEL_INJECTION_FLOW_RATE  = 192 ; // BOSH value for test change real value

static FuelConsumptionSensor fuelConsumptionSensor(SensorType::FuelConsumptionLh,FUEL_INJECTION_FLOW_RATE,engineConfiguration->cylindersCount);

void initFuelConsumption() {
	
	if (Sensor::hasSensor(SensorType::FuelConsumptionLh)) {
		return;
	}
	
	fuelConsumptionSensor.Register();
	
}

void deInitFuelConsumption(){

	fuelConsumptionSensor.unregister();

}