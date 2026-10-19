#ifndef TCLOG_PICO_PRINTER_H
#define TCLOG_PICO_PRINTER_H

#include "PrintCompat.h"
#include <string.h>
#include <ctype.h>
#include <pico/time.h>
#include <hardware/uart.h>
#ifdef BUILD_PICO_FORCE_UART
class PrintfLogger : public Print {
private:
public:
    size_t write(uint8_t ch) override {
        uart_putc_raw(uart0, ch);
        return 1;
    }

    size_t write(const char* sz) override {
        auto len = strlen(sz);
        for(int i=0;i<len;i++) uart_putc_raw(uart0, sz[i]);
        return len;
    }
};
#define IOLOG_START_SERIAL uart_init(uart_get_instance(0), 115200);gpio_set_function(1, GPIO_FUNC_UART);gpio_set_function(0, GPIO_FUNC_UART);
#else
class PrintfLogger : public Print {
public:
    size_t write(uint8_t ch) override {
        putchar_raw(ch);
        return 1;
    }

    size_t write(const char* sz) override {
        auto len = strlen(sz);
        for(int i=0;i<len;i++) putchar_raw(sz[i]);
        return len;
    }
};
#define IOLOG_START_SERIAL stdio_init_all();
#endif
unsigned long millis();
unsigned long micros();
extern PrintfLogger LoggingPort;
#define F(x) x
#define IOLOG_MBED_PORT_IF_NEEDED(tx, rx)

#endif
