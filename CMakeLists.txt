# Set the minimum required version of cmake
cmake_minimum_required(VERSION 3.10)

# Define your project name and the language used
project(led_string_remote C CXX ASM)

# C++11 is required
set(CMAKE_CXX_STANDARD 11)

# Specify the type of microcontroller
set(MCU atmega32u4)

# Set the CPU frequency
set(F_CPU 16000000UL)

# Set any necessary C/C++ flags here
set(CMAKE_C_FLAGS "-g -Wall -mcall-prologues -Os -mmcu=${MCU} -DF_CPU=${F_CPU}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")

# Specify the name of the target
set(TARGET_NAME ${PROJECT_NAME}.elf)

# Add your source files here
set(SOURCE_FILES
    src/main.cpp
    src/ir.cpp
    src/ir.h
    src/led.cpp
    src/led.h
    # Add any additional source files here
)

# Add the executable based on the source files
add_executable(${TARGET_NAME} ${SOURCE_FILES})

## You might want to set the avr-gcc as the compiler, depending on your setup
#set(CMAKE_C_COMPILER avr-gcc)
#set(CMAKE_CXX_COMPILER avr-g++)

# Custom target for AVR programming, replace <your-programmer> and <your-port> with appropriate values
add_custom_target(upload
    COMMAND avrdude -p ${MCU} -c avrisp2 -U flash:w:${TARGET_NAME}
    DEPENDS ${TARGET_NAME}
)
