# ESP32 Web-Based LED Control with LCD Display

This project demonstrates how to control an LED on an ESP32 board using a web interface. The ESP32 connects to a Wi-Fi network, hosts a web server, and displays the IP address on an I2C LCD. Users can specify the number of times they want the LED to blink through a web form.

## Requirements

- ESP32 board (NodeMCU-32S or similar)
- I2C LCD display (16x2 or 20x4)
- Arduino IDE with ESP32 board support
- WiFi network

## Libraries

Make sure to install the following libraries via the Arduino Library Manager:

- WiFi
- Wire
- hd44780
- hd44780ioClass

## Hardware Setup

1. Connect the I2C LCD display to the ESP32:
   - SDA to GPIO 21
   - SCL to GPIO 22
   - VCC to 3.3V
   - GND to GND
2. Connect an LED to GPIO 13 with a suitable resistor (e.g., 220Ω).
