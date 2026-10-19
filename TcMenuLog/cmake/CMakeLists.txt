cmake_minimum_required(VERSION 3.13)

add_library(TcMenuLog 
        ../src/IoLogging.cpp
        ../src/TextUtilities.cpp
)

if (DEFINED BUILD_FOR_STM32CUBE)
    target_compile_definitions(TcMenuLog
            PUBLIC BUILD_FOR_STM32CUBE_CMAKE=1 IO_LOGGING_DEBUG=1
    )
elseif (BUILD_NATIVE_TESTS)
    target_compile_definitions(TcMenuLog
            PUBLIC BUILD_FOR_NATIVE_PLATFORM=1 IO_LOGGING_DEBUG=1
    )
else ()
    target_compile_definitions(TcMenuLog
            PUBLIC BUILD_FOR_PICO_CMAKE=1 BUILD_PICO_FORCE_UART=1 IO_LOGGING_DEBUG=1
    )
endif ()

target_include_directories(TcMenuLog PUBLIC
        ../src
)

if(TARGET pico_stdlib AND TARGET pico_sync)
    message("Build is pico, adding libs")
    target_link_libraries(TcMenuLog PUBLIC pico_stdlib pico_sync)
elseif (TARGET stm32cubemx)
    message("Build is stm32cube adding stm32cubemx")
    target_link_libraries(TcMenuLog PUBLIC stm32cubemx)
endif ()

if (BUILD_NATIVE_TESTS)
    get_unity_if_needed()
    add_executable(test_text_utils ../test/test_text_utils/test_text_utils.cpp)
    target_link_libraries(TcMenuLog PUBLIC)
    target_link_libraries(test_text_utils PRIVATE unity TcMenuLog)
    add_test(NAME test_text_utils COMMAND test_text_utils)
endif ()
