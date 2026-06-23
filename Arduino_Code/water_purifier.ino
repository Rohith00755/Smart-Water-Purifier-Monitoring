#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 10
#define OLED_SCL 11

#define TDS_IN_PIN 4
#define TDS_OUT_PIN 5

#define ONE_WIRE_BUS 8

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

float pH = 7.20;
float DO = 6.80;

void startupAnimation()
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(18, 0);
  display.println("SMART");

  display.setCursor(18, 20);
  display.println("WATER");

  display.setTextSize(1);
  display.setCursor(10, 50);
  display.println("PURIFIER SYSTEM");

  display.display();

  delay(2000);

  display.clearDisplay();

  display.setCursor(25, 15);
  display.println("Loading...");

  display.drawRect(10, 35, 108, 12, SSD1306_WHITE);

  for (int i = 0; i <= 104; i += 4)
  {
    display.fillRect(12, 37, i, 8, SSD1306_WHITE);
    display.display();
    delay(30);
  }

  delay(500);
}

void setup()
{
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  tempSensor.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED Failed");
    while (1);
  }

  startupAnimation();

  randomSeed(analogRead(1));
}

void loop()
{
  // Temperature
  tempSensor.requestTemperatures();
  float temperature = tempSensor.getTempCByIndex(0);

  // Read TDS ADC
  int adcIn = analogRead(TDS_IN_PIN);
  int adcOut = analogRead(TDS_OUT_PIN);

  float voltageIn = adcIn * 3.3 / 4095.0;
  float voltageOut = adcOut * 3.3 / 4095.0;

  // TDS Calculation
  float tdsIn =
      (133.42 * voltageIn * voltageIn * voltageIn
      - 255.86 * voltageIn * voltageIn
      + 857.39 * voltageIn) * 0.5;

  float tdsOut =
      (133.42 * voltageOut * voltageOut * voltageOut
      - 255.86 * voltageOut * voltageOut
      + 857.39 * voltageOut) * 0.5;

  // Simulated pH & DO
  pH += random(-2, 3) * 0.01;
  DO += random(-2, 3) * 0.01;

  pH = constrain(pH, 6.8, 7.5);
  DO = constrain(DO, 6.3, 7.2);

  // Efficiency
  float efficiency = 0;

  if (tdsIn > 0)
  {
    efficiency = ((tdsIn - tdsOut) / tdsIn) * 100.0;
  }

  efficiency = constrain(efficiency, 0, 100);

  // Water Quality Score
  int qualityScore = 100;

  if (tdsOut > 500)
    qualityScore -= 50;
  else if (tdsOut > 300)
    qualityScore -= 30;
  else if (tdsOut > 150)
    qualityScore -= 15;

  if (pH < 6.5 || pH > 8.5)
    qualityScore -= 20;

  if (DO < 5.0)
    qualityScore -= 20;

  qualityScore = constrain(qualityScore, 0, 100);

  String qualityText;

  if (qualityScore >= 90)
    qualityText = "EXCELLENT";
  else if (qualityScore >= 75)
    qualityText = "GOOD";
  else if (qualityScore >= 50)
    qualityText = "FAIR";
  else
    qualityText = "POOR";

  // Serial Monitor
  Serial.print("Temp: ");
  Serial.print(temperature);

  Serial.print("  IN:");
  Serial.print((int)tdsIn);

  Serial.print("  OUT:");
  Serial.print((int)tdsOut);

  Serial.print("  Score:");
  Serial.println(qualityScore);

  // OLED
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("IN:");
  display.print((int)tdsIn);
  display.print("ppm");

  display.setCursor(70, 0);
  display.print("OUT:");
  display.print((int)tdsOut);

  display.setCursor(0, 16);
  display.print("pH:");
  display.print(pH, 2);

  display.setCursor(70, 16);
  display.print("DO:");
  display.print(DO, 2);

  display.setCursor(0, 32);
  display.print("TMP:");
  display.print(temperature, 1);
  display.print("C");

  display.setCursor(70, 32);
  display.print("EFF:");
  display.print((int)efficiency);
  display.print("%");

  display.setCursor(0, 48);
  display.print("Q:");
  display.print(qualityScore);
  display.print("/100");

  display.setCursor(70, 48);
  display.print(qualityText);

  display.display();

  delay(1000);
}