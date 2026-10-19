#ifndef TCLOG_STM_CUBE_PRINTER_H
#define TCLOG_STM_CUBE_PRINTER_H

#include "PrintCompat.h"
#include <string.h>
#include <ctype.h>

#if defined(TCLOG_STM32_HAL_INCLUDE)
#include TCLOG_STM32_HAL_INCLUDE
#elif __has_include("main.h")
#include "main.h"
#else
#error "STM32Cube logging needs either TCLOG_STM32_HAL_INCLUDE or a visible main.h"
#endif

#define STM_CUBE_LOGGING_ON
#define F(x) x

extern UART_HandleTypeDef* loggingUart;

extern uint32_t millis();
extern uint32_t micros();
//
// On mbed you create an instance of this class called LoggingPort in your main class.
// see the mbed example.
//
class StmCubeLogger : public Print {
public:
    explicit StmCubeLogger() = default;
    ~StmCubeLogger() override = default;

    size_t write(uint8_t ch) override {
        HAL_UART_Transmit(loggingUart, &ch, 1, 100);
        return 1;
    }

    size_t write(const char* sz) override {
        const auto len = strlen(sz);
        HAL_UART_Transmit(loggingUart, reinterpret_cast<const uint8_t*>(sz), len, 100);
        return len;
    }
};

extern StmCubeLogger LoggingPort;

#endif
