#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCDs
const LiquidCrystal_I2C lcd[] = {LiquidCrystal_I2C(0x27, 20, 4), LiquidCrystal_I2C(0x26, 20, 4)};

// Button pins
const uint8_t btnPinUp[4] PROGMEM = {2, 5, 8, 11};
const uint8_t btnPinDown[4] PROGMEM = {3, 6, 9, 12};
const uint8_t btnPinSelector[4] PROGMEM = {4, 7, 10, 13};

// Life and Counters
uint8_t counter[4] = {40, 40, 40, 40};
uint8_t poisonCounter[4] = {0, 0, 0, 0};
uint8_t commandDamageFront[4] = {0, 0, 0, 0};
uint8_t commandDamageSide[4] = {0, 0, 0, 0};
uint8_t commandDamageDiagonal[4] = {0, 0, 0, 0};

// 0 life counter | 1 poison counter | 2 command damage front | 3 command damage side | 4 command damage diagonal
// Selector and life showing on lcd
const char counterTitle[5][16] PROGMEM = {"   Life Total:  ", "Poison Counters:", "   CDmg Front:  ", "   CDmg Side:   ", " CDmg Diagonal: "};
uint8_t selectorState[4] = {0, 0, 0, 0};
uint8_t currentCounter[4] = {0, 0, 0, 0};

// Button States
uint8_t btnStateUp[4] = {HIGH, HIGH, HIGH, HIGH};
uint8_t btnStateDown[4] = {HIGH, HIGH, HIGH, HIGH};
uint8_t btnStateSelector[4] = {HIGH, HIGH, HIGH, HIGH};
uint8_t lastBtnStateSelector[4] = {HIGH, HIGH, HIGH, HIGH};

// FUNCTION displays the changes on the lcd of the choosen player
void displayChange(LiquidCrystal_I2C lcd, uint8_t player) {
  char displayCounter[3];
  sprintf(displayCounter, "%03d", currentCounter[player]);

  if (currentCounter[player] < 100) {
    displayCounter[0] = displayCounter[1];
    displayCounter[1] = displayCounter[2];
    displayCounter[2] = ' ';
  }

  if (currentCounter[player] < 10) {
    displayCounter[0] = displayCounter[1];
    displayCounter[1] = ' ';
  }

  lcd.setCursor(7, 1);
  lcd.print(displayCounter);
}

// FUNCTION
void displaySelector(LiquidCrystal_I2C lcd, uint8_t player) {

  selectorState[player]++;
  if (selectorState[player] > 4) {
    selectorState[player] = 0;
  }

  lcd.setCursor(0, 0);

  switch (selectorState[player]) {
    case 0:
      lcd.print(counterTitle[0]);
      commandDamageDiagonal[player] = currentCounter[player];
      currentCounter[player] = counter[player];
      break;

    case 1:
      lcd.print(counterTitle[1]);
      counter[player] = currentCounter[player];
      currentCounter[player] = poisonCounter[player];
      break;

    case 2:
      lcd.print(counterTitle[2]);
      poisonCounter[player] = currentCounter[player];
      currentCounter[player] = commandDamageFront[player];
      break;

    case 3:
      lcd.print(counterTitle[3]);
      commandDamageFront[player] = currentCounter[player];
      currentCounter[player] = commandDamageSide[player];
      break;

    case 4:
      lcd.print(counterTitle[4]);
      commandDamageSide[player] = currentCounter[player];
      currentCounter[player] = commandDamageDiagonal[player];
      break;
  }
  displayChange(lcd, ++player);
}

void setup() {
  // initialize buttons and lcd
  for (uint8_t i = 0; i < 4; i++) {
    lcd[i].init();
    lcd[i].backlight();
    lcd[i].setCursor(3, 0);
    lcd[i].print("Life Total:");
    lcd[i].setCursor(7, 1);
    lcd[i].print("40");
    currentCounter[i] = counter[i];

    pinMode(btnPinUp[i], INPUT_PULLUP);
    pinMode(btnPinDown[i], INPUT_PULLUP);
    pinMode(btnPinSelector[i], INPUT_PULLUP);
  }
}

void loop() {
  bool madeChanges;
  //Add delay to all button presses so you can keep button pressed to change life
  delay(150);

  // Read button states
  for (uint8_t i; = 0; i < 4; i++) {
    madeChanges = false;

    btnStateUp[i] = digitalRead(btnPinUp[i]);
    btnStateDown[i] = digitalRead(btnPinDown[i]);
    btnStateSelector[i] = digitalRead(btnPinSelector[i]);

    if (btnStateUp[i] == LOW) {
      currentCounter[i]++;
      madeChanges = true;
    }

    if (btnStateDown[i] == LOW) {
      currentCounter[i]--;
      madeChanges = true;
    }

    if (madeChanges) {
      displayChange(lcd[i], i);
    }

    if (btnStateSelector[i] == LOW && lastBtnStateSelector[i] == HIGH) {
      displaySelector(lcd[i], i);
    }

    lastBtnStateSelector[i] = btnStateSelector[i];
  }

}
