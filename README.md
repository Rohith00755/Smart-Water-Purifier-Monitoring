# Smart Water Purifier Monitoring System

An ESP32-S3-oriented water-quality monitoring prototype that reads TDS sensor inputs and temperature, calculates a simple TDS reduction estimate, and displays the values on a 128×64 OLED. The project is intended as an embedded-systems learning prototype for sensor acquisition, calculations, and local display.

> **Important implementation note:** In the current checked-in firmware, pH and dissolved oxygen (DO) values are simulated in software and are not read from physical pH/DO sensors. The displayed quality score is a prototype heuristic, not a certified water-safety assessment. Do not use this project alone to determine whether water is safe to drink.

## Current Features

- Reads two analog TDS inputs representing inlet and outlet measurements.
- Reads a DS18B20 temperature sensor over OneWire.
- Calculates approximate TDS values from ADC-derived voltage using a polynomial formula.
- Estimates TDS reduction efficiency from inlet and outlet values.
- Computes a simple 0–100 heuristic score and a text label.
- Displays inlet TDS, outlet TDS, pH, DO, temperature, estimated efficiency, and score on an SSD1306 OLED.
- Prints selected readings to the Serial Monitor for debugging.

## Hardware in the Current Sketch

| Component | Purpose | Firmware configuration |
|---|---|---|
| ESP32-compatible board | Main controller and ADC readings | Select the exact board in Arduino IDE |
| TDS sensor module (inlet) | Inlet TDS analog signal | GPIO 4 |
| TDS sensor module (outlet) | Outlet TDS analog signal | GPIO 5 |
| DS18B20 temperature sensor | Temperature reading | GPIO 8 (OneWire) |
| SSD1306 OLED, 128×64, I²C | Local display | I²C SDA GPIO 10, SCL GPIO 11; address 0x3C |
| pH sensor | Not currently read by this sketch | Future integration |
| DO sensor | Not currently read by this sketch | Future integration |

**Verify pin availability for your exact ESP32-S3 board.** ADC, I²C, and OneWire pin assignments can vary with board design. Use suitable sensor-interface circuits and confirm voltage compatibility before wiring.

## Repository Structure

```text
Smart-Water-Purifier-Monitoring/
├── Arduino_Code/
│   └── water_purifier.ino
├── Circuit_Diagram/
├── Images/
└── README.md
```

## Software Requirements

- Arduino IDE
- Arduino core for the target ESP32 board
- Adafruit GFX Library
- Adafruit SSD1306 Library
- OneWire library
- DallasTemperature library

The sketch also uses Wire, which is provided by the Arduino platform.

## Setup

1. Clone or download this repository.
2. Open Arduino_Code/water_purifier.ino in Arduino IDE.
3. Install the libraries listed above through Library Manager if needed.
4. Select the correct ESP32 board and serial port.
5. Wire the TDS modules, DS18B20, and OLED according to the pin table and each module's documentation.
6. Upload the sketch.
7. Open Serial Monitor at **115200 baud**.
8. Check the OLED readings against known test conditions and calibrate sensor modules before relying on any numeric values.

## Calculation Notes

### ADC-to-voltage conversion

The sketch assumes a 12-bit ADC range of 0–4095 and a 3.3 V reference:

```cpp
voltage = adc * 3.3 / 4095.0;
```

The TDS polynomial in the sketch converts that voltage into an estimated value. Actual TDS modules may require temperature compensation and calibration against a known reference solution. Confirm the sensor module's specifications and the formula's suitability for your hardware.

### TDS reduction estimate

When inlet TDS is above zero, the sketch estimates:

```text
efficiency (%) = ((inlet TDS - outlet TDS) / inlet TDS) × 100
```

The result is clamped to 0–100%. This is a simple comparison of the two readings; it does not measure overall purifier performance or guarantee contaminant removal.

### Prototype quality score

The firmware starts at 100 points and subtracts points based on outlet TDS and the pH/DO variables, then assigns EXCELLENT, GOOD, FAIR, or POOR. Since pH and DO are currently simulated, and the thresholds are hard-coded, this score is for demonstrating embedded logic only. It must not be interpreted as an official drinking-water standard or safety certification.

## Current Limitations

- pH and DO are generated using small random changes in software; physical pH and DO sensor drivers and calibration are not implemented in this sketch.
- The TDS conversion has not been documented here as calibrated against a reference solution.
- The quality score is a heuristic and is not a validated water-quality model.
- No cloud dashboard or remote telemetry is implemented in the checked-in sketch.
- The project does not establish that the measured water is microbiologically or chemically safe to drink.

## Suggested Next Steps

1. Integrate and calibrate physical pH and DO sensors with suitable signal conditioning.
2. Validate TDS readings against reference solutions and add temperature compensation if supported by the sensor.
3. Add sensor-disconnection and invalid-reading handling, including the DS18B20 disconnected-sensor case.
4. Document the exact board model, sensor modules, power requirements, and a verified wiring diagram.
5. Test with controlled samples and record measured results, test conditions, and limitations.
6. If remote monitoring is added, document the communication protocol and distinguish measured values from estimates.

## Author

**Rohith S** · [GitHub](https://github.com/Rohith00755) · [LinkedIn](https://www.linkedin.com/in/rohith-sathish-971052327)
