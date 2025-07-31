#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64  

#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int batteryPin = 36; // Analog pin connected to voltage divider
float batteryVoltage = 0.0;

void setup() {
  Serial.begin(115200);

  // OLED Init
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  batteryVoltage = readBatteryVoltage();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Portable Charger");
  display.print("Battery: ");
  display.print(batteryVoltage, 2);
  display.println("V");

  drawBatteryIcon(batteryVoltage);

  display.display();
  delay(1000);
}

float readBatteryVoltage() {
  int raw = analogRead(batteryPin); 
  float v = (raw / 4095.0) * 3.3; 
  float batteryV = v * ((200.0 + 100.0) / 100.0); // Scale for voltage divider
  return batteryV;
}

void drawBatteryIcon(float voltage) {
  int x = 100, y = 10, width = 20, height = 10;
  display.drawRect(x, y, width, height, SSD1306_WHITE);
  display.drawRect(x + width, y + 3, 2, 4, SSD1306_WHITE); // battery tip

  int fill = map(voltage * 100, 300, 420, 0, width - 2); // map from 3.0–4.2V
  fill = constrain(fill, 0, width - 2);

  display.fillRect(x + 1, y + 1, fill, height - 2, SSD1306_WHITE);
}