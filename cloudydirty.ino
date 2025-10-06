#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorPin = A0;
const int RELAY_PIN = 2;

const int LED_CLEAR = 7;
const int LED_CLOUDY = 8;
const int LED_DIRTY = 9;

bool pumpActive = false;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_CLEAR, OUTPUT);
  pinMode(LED_CLOUDY, OUTPUT);
  pinMode(LED_DIRTY, OUTPUT);

  lcd.print("Turbidity System");
  delay(2000);
  lcd.clear();
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  // Adjust if necessary based on actual sensor behavior
  int turbidity = map(sensorValue, 300, 750, 100, 0);  // sensorValue of 300 = dirty (100%), 750 = clean (0%)
  turbidity = constrain(turbidity, 0, 100);

  // Debug print
  Serial.print("Analog: ");
  Serial.print(sensorValue);
  Serial.print(" | Turbidity: ");
  Serial.println(turbidity);

  lcd.setCursor(0, 0);
  lcd.print("Turbidity:     ");
  lcd.setCursor(10, 0);
  lcd.print(turbidity);
  lcd.print("   "); // overwrite leftovers

  // Main turbidity condition logic
  if (turbidity < 20) { // CLEAR
    pumpActive = false;

    digitalWrite(LED_CLEAR, HIGH);
    digitalWrite(LED_CLOUDY, LOW);
    digitalWrite(LED_DIRTY, LOW);
    lcd.setCursor(0, 1);
    lcd.print(" It's CLEAR    ");
  } 
  else if (turbidity >= 20 && turbidity <= 40) { // CLOUDY
    pumpActive = true;

    digitalWrite(LED_CLEAR, LOW);
    digitalWrite(LED_CLOUDY, HIGH);
    digitalWrite(LED_DIRTY, LOW);
    lcd.setCursor(0, 1);
    lcd.print(" It's CLOUDY   ");
  } 
  else { // DIRTY
    pumpActive = true;

    digitalWrite(LED_CLEAR, LOW);
    digitalWrite(LED_CLOUDY, LOW);
    digitalWrite(LED_DIRTY, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(" It's DIRTY    ");
  }

  // Control relay: LOW = ON (active-low)
  digitalWrite(RELAY_PIN, pumpActive ? LOW : HIGH);

  delay(1000);
}

