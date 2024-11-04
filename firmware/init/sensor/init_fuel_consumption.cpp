#include "pch.h"
#include "fuel_consumption_sensor.h"
#include "init.h"

const float FUEL_INJECTION_FLOW_RATE  = 192 ; // BOSH value for test change real value

static FuelConsumptionSensor fuelConsumptionSensor(SensorType::FuelConsumptionLh);

void initFuelConsumption() {
		
	fuelConsumptionSensor.initSensor(FUEL_INJECTION_FLOW_RATE,engineConfiguration->cylindersCount);

	fuelConsumptionSensor.Register();
	
}

void deInitFuelConsumption(){

	fuelConsumptionSensor.unregister();

}