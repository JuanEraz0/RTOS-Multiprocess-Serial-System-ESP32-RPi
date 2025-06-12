# RTOS-Multiprocess-Serial-System-ESP32-RPi
# RTOS Multiprocess Serial System (ESP32 ↔ Raspberry Pi)

This project implements a FreeRTOS-based embedded system on an ESP32 that communicates with a Raspberry Pi via UART. The system integrates multitasking, temperature monitoring, touch-based motor and servo control, and real-time UI updates using an OLED display with LVGL.

## Features

- Touch pad interface for servo and DC motor control
- ADC-based temperature acquisition with periodic and interrupt-driven UART reporting
- UART serial communication with Raspberry Pi (formatted data exchange)
- LVGL-powered UI on OLED for real-time feedback
- FreeRTOS-based multitasking (timers, tasks, queues, ISR)
- Modular structure with custom components: `adc`, `pwm`, `ssd1306`, `touch`, `uart`

##  Hardware Requirements

- ESP32(esp32s3) (with FreeRTOS support)
- Raspberry Pi (for serial data reception and display)
- OLED Display (SSD1306 or compatible)
- Touch pads (connected to TOUCH_PAD_NUM2–7)
- DC Motor + Driver (e.g., L298N or transistor-based)
- Servo Motor
- Temperature sensor via ADC input
- USB/Serial bridge for UART communication

## System Architecture

| Component | Description |
|----------|-------------|
| **FreeRTOS** | Manages multitasking: ADC reading, display update, touch interaction, UART transmission |
| **UART** | Sends data to Raspberry Pi: `(temperature, servoAngle, motorSpeed)` |
| **Touch Pads** | Control motor speed (up/down/off) and servo angle (left/right/reset) |
| **LVGL** | Displays temperature, servo position, and motor status on OLED |
| **ADC** | Reads and queues temperature every 100 ms |
| **Timer + ISR** | Sends all queued temperature data every 30s or when an external button is pressed |

## Installation

1. **Clone this repository**:
   ```bash
   git clone https://github.com/JuanEraz0/RTOS-Multiprocess-Serial-System-ESP32-RPi.git
   cd RTOS-Multiprocess-Serial-System-ESP32-RPi
   ```

2. **Configure your ESP-IDF environment**

3. **Build and flash to ESP32**:
   ```bash
   idf.py build
   idf.py -p /dev/ttyUSB0 flash
   ```

4. **Connect to Raspberry Pi UART (or USB-Serial)** and read data at `115200` baud.

##  UART Communication Format

- On PWM update:  
  ```
  (temperature,servoAngle,motorSpeed)\n
  ```

- On 30s interval or button press:
  ```
  TEMP_DATA_START:N\n
  <temperature1>\n
  ...
  <temperatureN>\n
  TEMP_DATA_END\n
  ```

##  Usage

- Interact using the 6 touch pads:
  - Servo Control: Reset / Rotate Left / Rotate Right
  - Motor Control: Increase Speed / Decrease Speed / Turn Off
- Push button triggers temperature queue send
- OLED displays live data using LVGL
- Raspberry Pi reads and parses UART data for visualization or logging

## Output Example

- OLED:
  ```
  Temp: 31 ºC
  Servo: -30°
  Motor: 330 PWM
  ```

- UART output:
  ```
  (31,-30,330)
  ```

## Dependencies

- ESP-IDF framework
- LVGL graphics library
- Custom components: `adc.c`, `touch.c`, `uart.c`, `pwm.c`, `ssd1306.c`

## License

This project is licensed under the MIT License.
