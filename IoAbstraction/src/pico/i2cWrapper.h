
#ifndef TCCLIBS_I2CWRAPPER_H
#define TCCLIBS_I2CWRAPPER_H

#include <IoAbstraction.h>
#include "hardware/i2c.h"
#include "hardware/spi.h"

class PicoI2cWrapper {
private:
    i2c_inst_t* nativeI2c = nullptr;
public:
    bool isValid() { return nativeI2c != nullptr; }
    void init(i2c_inst_t* i2c) {
        nativeI2c = i2c;
    }

    bool wireRead(uint8_t addr, uint8_t *dst, size_t len);
    bool wireWrite(uint8_t addr, const uint8_t *dst, size_t len, int retries, bool sendStop);
};

#define TC_SPI_WRITE_AVAILABLE

class SPIWithSettings {
private:
    spi_inst_t* spiBus;
    uint32_t speed;
    pinid_t csPin = 0;
    bool initializedYet = false;
public:
    SPIWithSettings(spi_inst_t* bus, pinid_t cs) : spiBus(bus), csPin(cs), speed(10000000) {}
    SPIWithSettings(spi_inst_t* bus, pinid_t cs, uint32_t speed) : spiBus(bus), speed(speed), csPin(cs) {}
    SPIWithSettings(const SPIWithSettings&) = default;
    SPIWithSettings& operator=(const SPIWithSettings&)=default;

    void init() {
        internalDigitalDevice().pinMode(csPin, OUTPUT);
        internalDigitalDevice().digitalWrite(csPin, HIGH);
        initializedYet=true;
    }

    void waitABit() {
        asm volatile("nop \n nop \n nop");
    }

    void waitAndActiveCS();

    void waitAndDeactivateCS() {
        waitABit();
        internalDigitalDevice().digitalWrite(csPin, HIGH);
        waitABit();
    }

    bool write(const uint8_t* data, size_t size) {
        waitAndActiveCS();
        int written = spi_write_blocking(spiBus, data, size);
        waitAndDeactivateCS();
        return written == size;
    }

    bool transferSPI(uint8_t* rdwr, size_t len) {
        waitAndActiveCS();
        int written = spi_write_read_blocking(spiBus, rdwr, rdwr, len);
        waitAndDeactivateCS();
        return written == len;
    }
};

#endif //TCCLIBS_I2CWRAPPER_H
