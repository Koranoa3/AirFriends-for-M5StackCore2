#pragma once
#include <Adafruit_BME680.h>
#include <Wire.h>

class BME680Sensor
{
public:
    BME680Sensor(uint8_t i2c_addr = 0x77, TwoWire *wire = &Wire)
        : _bme(wire), _i2c_addr(i2c_addr) {}

    bool begin()
    {
        if (!_bme.begin(_i2c_addr))
            return false;
        _bme.setTemperatureOversampling(BME680_OS_8X);
        _bme.setHumidityOversampling(BME680_OS_2X);
        _bme.setPressureOversampling(BME680_OS_4X);
        _bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
        _bme.setGasHeater(320, 150);
        return true;
    }

    bool read() { return _bme.performReading(); }

    float temperature() const { return _bme.temperature; }
    float humidity() const { return _bme.humidity; }
    float pressure() const { return _bme.pressure / 100.0; }
    float gas() const { return _bme.gas_resistance / 1000.0; }

private:
    Adafruit_BME680 _bme;
    uint8_t _i2c_addr;
};
