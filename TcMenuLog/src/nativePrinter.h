
#ifndef TCLOG_MBED_PRINTER_H
#define TCLOG_MBED_PRINTER_H

#define LOGGING_USES_STDOUT_NATIVE
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "PrintCompat.h"

//
// On mbed you create an instance of this class called LoggingPort in your main class.
// see the mbed example.
//
class NativeLogger : public Print {
public:

    size_t write(uint8_t ch) override {
        putchar(ch);
        return 1;
    }

    size_t write(const char* sz) override {
        const auto len = strlen(sz);
        fwrite(sz, 1, len, stdout);

        return len;
    }
};
extern NativeLogger LoggingPort;
// a couple of definitions here to avoid including headers, F() macro not needed on mbed

#define F(x) x
#define IOLOG_MBED_PORT_IF_NEEDED(tx, rx)
#define IOLOG_START_SERIAL
unsigned long millis();
unsigned long micros();

#endif
