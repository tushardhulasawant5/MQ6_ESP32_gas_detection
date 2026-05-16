#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <U8g2lib.h>

// Initialize the 1.3" OLED using hardware I2C (SH1106 driver)
// F means Full frame buffer mode (best performance)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Pin Configurations
const int MQ6_PIN = 34; 

// Alert Trigger Level 
// (If your clean air baseline is ~180, setting this to 600 or 800 is a good safety limit)
const int ALERT_THRESHOLD = 700; 

void setup() {
    // Start serial communication for computer debugging
    Serial.begin(115200);
    
    // Explicitly configure ESP32's I2C pins: SDA=21, SCL=22
    Wire.begin(21, 22); 
    
    // Configure U8g2 to target your specific I2C address (0x3C * 2 = 0x78)
    u8g2.setI2CAddress(0x3C * 2); 
    
    // Wake up and initialize the display hardware
    u8g2.begin();
    
    // Configure ESP32 Analog-to-Digital Converter to 12-bit (0 - 4095)
    analogReadResolution(12);
    pinMode(MQ6_PIN, INPUT);

    // Boot Up Welcome Screen
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB10_tr); // Bold font
    u8g2.drawStr(10, 25, "MQ-6 DETECTOR");
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(15, 48, "Warming up sensor...");
    u8g2.sendBuffer();
    
    delay(3000); // Give the user a moment to read the screen
}

void loop() {
    // 1. Read the analog gas values
    int rawValue = analogRead(MQ6_PIN);
    float voltage = (rawValue / 4095.0) * 3.3;

    // 2. Output backup logs to the computer Serial Monitor
    Serial.print("Raw ADC: "); Serial.print(rawValue);
    Serial.print(" | Voltage: "); Serial.print(voltage, 2);
    Serial.println("V");

    // 3. Clear the display memory buffer to draw a fresh frame
    u8g2.clearBuffer();

    // --- Header ---
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "====================");
    u8g2.drawStr(12, 22, "GAS MONITOR ACTIVE");
    u8g2.drawStr(0, 28, "====================");

    // --- Data Readouts ---
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(0, 43, "Gas Level:");
    u8g2.setCursor(75, 43);
    u8g2.print(rawValue);

    u8g2.drawStr(0, 55, "Voltage  :");
    u8g2.setCursor(75, 55);
    u8g2.print(voltage, 2);
    u8g2.print(" V");

    // --- Dynamic Alarm Status Footer ---
    if (rawValue > ALERT_THRESHOLD) {
        // Invert the bottom strip or draw prominent warning text
        u8g2.setFont(u8g2_font_6x12_tf);
        u8g2.drawStr(0, 64, "!!! GAS LEAK ALERT !!!");
    } else {
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(0, 64, "Status: Safe");
    }

    // 4. Push the final completed drawing frame to the physical OLED glass
    u8g2.sendBuffer();

    // Sample data once every second
    delay(1000); 
}