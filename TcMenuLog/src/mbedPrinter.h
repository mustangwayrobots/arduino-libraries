#ifndef TCLOG_MBED_PRINTER_H
#define TCLOG_MBED_PRINTER_H

#define LOGGING_USES_MBED
#include <mbed.h>

#include "PrintCompat.h"
#include <FileHandle.h>
//
// On mbed you create an instance of this class called LoggingPort in your main class.
// see the mbed example.
//
class MBedLogger : public Print {
private:
    FileHandle& serial;
public:
    explicit MBedLogger(FileHandle& serialName) : serial(serialName) {}

    size_t write(uint8_t ch) override {
        serial.write(&ch, 1);
        return 1;
    }

    size_t write(const char* sz) override {
        auto len = strlen(sz);
        serial.write(sz, len);
        return len;
    }
};
extern MBedLogger LoggingPort;
// a couple of definitions here to avoid including headers, F() macro not needed on mbed
unsigned long millis();
#define F(x) x
#define IOLOG_MBED_PORT_IF_NEEDED(tx, rx) BufferedSerial serPort(tx, rx);MBedLogger LoggingPort(serPort);
#define IOLOG_START_SERIAL serPort.set_baud(115200);
unsigned long millis();
unsigned long micros();

#endif
