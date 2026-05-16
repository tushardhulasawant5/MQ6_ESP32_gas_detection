

https://github.com/user-attachments/assets/452731f6-34ca-4f17-93f1-a308296c677f



# 🚀 ESP32 MQ-6 Gas Detector with 1.3" OLED Display

A professional, real-time embedded safety system built on the **ESP32 (38-pin NodeMCU-32S)** architecture. This project interfaces an **MQ-6 Semiconductor Sensor** to detect LPG, Butane, and Propane leaks, processing the telemetry locally and rendering live instrumentation data on a **1.3-inch SH1106 I2C OLED display**.

Developed using **VS Code + PlatformIO** utilizing the hardware-optimized **U8g2 graphics framework** and the Arduino ecosystem.

---

## 📌 Features

- **High-Precision Telemetry:** Utilizes the ESP32's 12-bit Analog-to-Digital Converter (ADC) for fine-grained voltage scaling ($0.00\text{V} - 3.30\text{V}$).
- **Hardware-Isolated Driver:** Configured specifically for the 1.3-inch **SH1106** I2C controller using full frame-buffer rendering.
- **Dynamic Alarm System:** Features an automated threshold interrupt layout that triggers an instant visual `!!! GAS LEAK ALERT !!!` display state.
- **Fail-Safe Serial Logging:** Simultaneous UART serial streaming at `115200 baud` for field calibration and diagnostics.
- **Production-Ready Configuration:** Zero compilation warnings, pre-packaged with strict `.gitignore` patterns to hide compilation artifacts (`.pio`).

---

## 🔌 Hardware Architecture & Wiring

### Circuit Topology
The MQ-6 sensor requires a **5V VCC** rail to power its internal chemical heating element. Because the ESP32 GPIO pins operate strictly on **3.3V logic**, ensure your MQ-6 breakout module features an integrated onboard voltage divider, or insert a hardware resistor divider network on the `AO` line to step the maximum voltage down to 3.3V before routing to GPIO 34.

| Peripheral | Peripheral Pin | ESP32 GPIO Pin | Description |
| :--- | :--- | :--- | :--- |
| **MQ-6 Gas Sensor** | VCC | **5V / VIN** | System Power (Heater Element) |
| **MQ-6 Gas Sensor** | GND | **GND** | Common System Ground |
| **MQ-6 Gas Sensor** | AO (Analog Out) | **GPIO 34 (ADC1_CH6)** | Safe Analog Telemetry Channel |
| **SH1106 OLED** | VCC | **3V3** | Display Logic Power |
| **SH1106 OLED** | GND | **GND** | Common System Ground |
| **SH1106 OLED** | SCL | **GPIO 22** | Hardware I2C Clock Line |
| **SH1106 OLED** | SDA | **GPIO 21** | Hardware I2C Data Line |

---

## 🔬 Calibration & Validation Protocol
### Preheating Target
The internal MQ-6 chemical heater requires a continuous burn-in phase of 24–48 hours for professional accuracy upon its first deployment. For daily operation, allow a 5–10 minute warm-up window for telemetry readings to settle into a flat baseline.

### Establishing Baseline
Observe the stable Raw ADC value on the OLED display within clean, ambient air. Under nominal conditions, this baseline typically ranges between 150 and 250.

### Controlled Threshold Validation
Introduce a localized source of unlit hydrocarbon gas (such as a standard disposable pocket lighter) directly near the physical sensor head for 1 second (do not ignite a flame). The ADC metric should scale rapidly past 1000+, verifying that the dynamic interrupt alarm states trigger correctly.

### ⚠️ Safety Notice
Perform all gas threshold validations in a well-ventilated space. Avoid prolonged exposure of the sensor head to concentrated gas to prevent saturating or damaging the internal chemical element.

---

## 📄 License
This project is open-source software maintained under the MIT License. Feel free to fork, modify, and integrate this architecture into your custom IoT automation and industrial safety systems.
