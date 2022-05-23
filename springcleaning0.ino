#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCDs
LiquidCrystal_I2C lcd = {LiquidCrystal_I2C(0x27, 20, 4), LiquidCrystal_I2C(0x26, 20, 4)};

// Button pins
const int  btnPinUp[] = {2, 5};
const int  btnPinDown[] = {3, 6};
const int  btnPinSelector[] = {4, 7};

// Life and Counters
int counter[] = {40, 40, 40, 40};
int poisonCounter[] = {0, 0, 0, 0};
int commandDamageFront[] = {0, 0, 0, 0};
int commandDamageSide[] = {0, 0, 0, 0};
int commandDamageDiagonal[] = {0, 0, 0, 0};

// 0 life counter | 1 poison counter | 2 command damage front | 3 command damage side | 4 command damage diagonal
// Selector and life showing on lcd ALL players
int selectorState[] = {0, 0, 0, 0};
int currentCounter[] = {0, 0, 0, 0};

// Button States
int btnStateUp[] = {HIGH, HIGH, HIGH, HIGH};
int btnStateDown[] = {HIGH, HIGH, HIGH, HIGH};
int btnStateSelector[] = {HIGH, HIGH, HIGH, HIGH};
int lastBtnStateSelector[] = {HIGH, HIGH, HIGH, HIGH};

// FUNTION displays the changes on the lcd of the choosen player
char displayCounter[3];
void displayChange(LiquidCrystal_I2C lcd, int player) {

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
void displaySelector(LiquidCrystal_I2C lcd, int player) {

  selectorState[player]++;
  if (selectorState[player] > 4)
    selectorState[player] = 0;

  lcd.clear();
  lcd.setCursor(0, 0);

  switch (selectorState[player]) {
    case 0:
      lcd.print("  Life Total:");
      commandDamageDiagonal[player] = currentCounter[player];
      currentCounter[player] = counter[player];
      break;

    case 1:
      lcd.print("Poison Counters:");
      counter[player] = currentCounter[player];
      currentCounter[player] = poisonCounter[player];
      break;

    case 2:
      lcd.print("    CD Front:");
      poisonCounter[player] = currentCounter[player];
      currentCounter[player] = commandDamageFront[player];
      break;

    case 3:
      lcd.print("    CD Side:");
      commandDamageFront[player] = currentCounter[player];
      currentCounter[player] = commandDamageSide[player];
      break;

    case 4:
      lcd.print("  CD Diagonal:");
      commandDamageSide[player] = currentCounter[player];
      currentCounter[player] = commandDamageDiagonal[player];
      break;
  }
  displayChange(lcd, ++player);
}

void setup() {
  // initialize buttons and lcd
  for (int i = 0; i < 4; i++) {
    lcd[i].init();
    lcd[i].backlight();
    lcd[i].setCursor(3, 0);
    lcd[i].print("Life Total:");
    lcd[i].setCursor(7, 1);
    lcd[i].print("40");
    currentCounter[i] = counter[i];

    pinMode(btnPinUp[i], INPUT);
    pinMode(btnPinDown[i], INPUT);
    pinMode(btnPinSelector[i], INPUT);
  }
}

bool madeChanges;
void loop() {
  //Add delay to all button presses so you can keep button pressed to change life
  delay(150);

  // Read button states
  for (int i = 0; i < 4; i++) {
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

    if (madeChanges) 
      displayChanges(lcd[i], i);
    
    if (btnStateSelector[i] == LOW && lastBtnStateSelector[i] == HIGH) 
      displaySelector(lcd[i], i);
    
    lastBtnStateSelector[i] = btnStateSelector[i];
  }

}
