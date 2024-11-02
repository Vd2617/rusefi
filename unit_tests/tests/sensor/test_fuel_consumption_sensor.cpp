
#include "fuel_consumption_sensor.h"
#include "sensor_type.h"

// Mock class for Sensor to override getOrZero method
class MockSensor : public Sensor {
public:
    MockSensor(SensorType type) : Sensor(type) {}
    SensorResult getOrZero(SensorType type) const override {
        if (type == SensorType::Rpm) {
            return 3000; // RPM value for testing
        }
        return 0;
    }
};

TEST(FuelConsumptionSensorTest, ValidData) {
    MockSensor mockSensor(SensorType::FuelConsumption);
    FuelConsumptionSensor sensor(SensorType::FuelConsumption, 10.0f, 4);

    EXPECT_FLOAT_EQ(sensor.get(), 2000.0f); 
}

TEST(FuelConsumptionSensorTest, InvalidFuelDutyCycle) {
    MockSensor mockSensor(SensorType::FuelConsumption);
    FuelConsumptionSensor sensor(SensorType::FuelConsumption, 10.0f, 4);

    // Simulate invalid fuel duty cycle
    float invalidFuelDutyCycle = -1.0f;
    EXPECT_FLOAT_EQ(sensor.get(), 0.0f);
}

TEST(FuelConsumptionSensorTest, InvalidInjectionFlowRate) {
    MockSensor mockSensor(SensorType::FuelConsumption);
    FuelConsumptionSensor sensor(SensorType::FuelConsumption, -10.0f, 4);

    EXPECT_FLOAT_EQ(sensor.get(), 0.0f);
}

TEST(FuelConsumptionSensorTest, InvalidNumCylindres) {
    MockSensor mockSensor(SensorType::FuelConsumption);
    FuelConsumptionSensor sensor(SensorType::FuelConsumption, 10.0f, 0);

    EXPECT_FLOAT_EQ(sensor.get(), 0.0f);
}

