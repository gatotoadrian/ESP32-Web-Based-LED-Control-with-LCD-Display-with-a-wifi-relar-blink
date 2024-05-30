#include <WiFi.h>
#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

const char* ssid     = "910";
const char* password = "Bl3ss3d@20"; // WiFi password*

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config expander chip

// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 4;

WiFiServer server(80);
int ledpin = 13;

void setup() {
    Serial.begin(115200);
    pinMode(ledpin, OUTPUT); // set the LED pin mode
    delay(10);

    lcd.begin(LCD_COLS, LCD_ROWS);
    // Print a message to the LCD.
    lcd.backlight();
    lcd.setCursor(3, 0);
    lcd.print("Connected");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    
    server.begin();
}

void loop() {
    WiFiClient client = server.available(); // listen for incoming clients

    if (client) { // if you get a client,
        Serial.println("New Client."); // print a message out the serial port
        String currentLine = ""; // make a String to hold incoming data from the client
        bool currentLineIsBlank = true; // flag to detect empty lines
        int blinkCount = 0;

        while (client.connected()) { // loop while the client's connected
            if (client.available()) { // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c); // print it out the serial monitor

                if (c == '\n' && currentLineIsBlank) {
                    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                    // and a content-type so the client knows what's coming, then a blank line:
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println();

                    // the content of the HTTP response follows the header:
                    client.println("<!DOCTYPE html><html>");
                    client.println("<head><title>ESP32 LED Control</title></head>");
                    client.println("<body><h1>ESP32 LED Control</h1>");
                    client.println("<form action=\"/blink\" method=\"GET\">");
                    client.println("Number of blinks: <input type=\"number\" name=\"count\"><br>");
                    client.println("<input type=\"submit\" value=\"Blink LED\">");
                    client.println("</form></body></html>");

                    // The HTTP response ends with another blank line:
                    client.println();
                    break;
                } else if (c == '\n') { // if you got a newline, then clear currentLine
                    currentLineIsBlank = true;
                    currentLine = "";
                } else if (c != '\r') { // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                    currentLineIsBlank = false;
                }

                // Check to see if the client request was "GET /blink?count=X":
                if (currentLine.startsWith("GET /blink?count=")) {
                    blinkCount = currentLine.substring(17).toInt();
                    Serial.print("Blink count: ");
                    Serial.println(blinkCount);
                    blinkLED(blinkCount);
                    break;
                }

                // Handle favicon.ico request
                if (currentLine.endsWith("GET /favicon.ico")) {
                    client.println("HTTP/1.1 204 No Content");
                    client.println("Connection: close");
                    client.println();
                    break;
                }
            }
        }
        // close the connection:
        client.stop();
        Serial.println("Client Disconnected.");
    }
}

// Function to blink the LED a specified number of times
void blinkLED(int count) {
    for (int i = 0; i < count; i++) {
        digitalWrite(ledpin, HIGH);
        delay(500);
        digitalWrite(ledpin, LOW);
        delay(500);
    }
}
