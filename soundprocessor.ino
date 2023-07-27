#include <LiquidCrystal.h>

#define speaker 12 //speaker pin

const int rs = 10, en = 11, d0 = 2, d1 = 3, d2 = 4, d3 = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7); //initialize LCD

int count = 0;
int waitingSounds = 0;
long last = 0;
int limit = 10;
bool bugfix = false;
int lastInput = 0;
long money[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
long moneyAmount = 0.0;
char* currentTier = "Freibier";
char* lastTier = "Freibier";

char* tiers[] = { "Freibier", "Center Shock", "Bayrisch Creme", "Pringles", "ESP32", "Doener", "Kinoticket", "Zelda: TotK", "10k Robux", "Fallschirmsprung", "Semesterbeitrag", "PS5" };
long tiersBarriers[12] = { 0, 50, 60, 249, 279, 600, 899, 5999, 11999, 24999, 39982, 52900 };

bool block0[3] = { false, false, false };
bool block1[7] = { false, false, false, false, false, false, false };

void setup() {
  lcd.begin(16, 2);
  lcd.print("Groschenzaehler");
  Serial.begin(9600);
  pinMode(speaker, OUTPUT);
}

void loop() {
  if (Serial.available()) {  //check the serial port as often as possible
    if (millis() > 5000 || bugfix) {
      bugfix = true;
      // amount of cent is read
      int moneyTmp = Serial.read();
      if (moneyTmp == 69) {
        moneyAmount = 0;
      } else if (moneyTmp > 7) {
        moneyAmount += calcCoin(moneyTmp - 8);
      } else {
        moneyAmount += calcCoin(moneyTmp);
        waitingSounds += 1;
      }
      currentTier = dropTier(moneyAmount);  // string currentTier gets set according to the current tier string o.O
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(currentTier);

      lcd.setCursor(0, 1);
      lcd.print(moneyAmount / 100.0);
      lcd.print(" Euro");
    }
  }
  playsound();  //"async" playsound
}

//plays sounds if a coin was tossed in and if a tier upgrade happened
void playsound() {
  if (lastTier == currentTier && waitingSounds > 0) {
    if (!block0[0] && (millis() - last >= 500)) {  //checks if the last activation was at least 500ms ago
      block0[0] = true;
      tone(speaker, 988, 100);
      last = millis();
    } else if (millis() - last >= 100 && (waitingSounds > 0) && !block0[1] && block0[0]) { //blocking is necessary to ensure the Sound is played back correctly
      block0[1] = true;
      tone(speaker, 1319, 250);
    } else if (millis() - last >= 350 && (waitingSounds > 0) && !block0[2] && block0[1]) {
      block0[2] = true;
      noTone(speaker);
      waitingSounds -= 1;
      count += 1;
      for (int i = 0; i < 3; i++) {
        block0[i] = false;
      }
    }
  } else if (lastTier != currentTier && waitingSounds > 0) {
    if (!block1[0] && (millis() - last >= 880)) {
      block1[0] = true;
      tone(speaker, 1319, 125);
      last = millis();
      
    } else if (millis() - last >= 130 ) {
      block1[1] = true;
      tone(speaker, 1568, 125);
    } 
    
    else if (millis() - last >= 260 && (waitingSounds > 0) && !block1[2] && block1[1]) {
      block1[2] = true;
      tone(speaker, 2637, 125);
    } else if (millis() - last >= 390 && (waitingSounds > 0) && !block1[3] && block1[2]) {
      block1[3] = true;
      tone(speaker, 2093, 125);
    } else if (millis() - last >= 520 && (waitingSounds > 0) && !block1[4] && block1[3]) {
      block1[4] = true;
      tone(speaker, 2349, 125);
    } else if (millis() - last >= 650 && (waitingSounds > 0) && !block1[5] && block1[4]) {
      block1[5] = true;
      tone(speaker, 3136, 125);
    } else if (millis() - last >= 780 && (waitingSounds > 0) && !block1[6] && block1[5]) {
      block1[6] = true;
      noTone(speaker);
      waitingSounds -= 1;
      lastTier = currentTier;
      count = 0;
      for (int i = 0; i < 7; i++) {
        block1[i] = false;
      }
    }
  }
}

//helper function. Converts the received integer to the according money value
uint32_t calcCoin(int i) {
  uint32_t out;
  switch (i) {
    case 0:
      out = 1;
      break;
    case 1:
      out = 2;
      break;
    case 2:
      out = 5;
      break;
    case 3:
      out = 10;
      break;
    case 4:
      out = 20;
      break;
    case 5:
      out = 50;
      break;
    case 6:
      out = 100;
      break;
    case 7:
      out = 200;
      break;
  }
  return out;
}

// returns tier string according to current money amount
char* dropTier(long currentMoney) {

  // to prevent array index out of bounds exception in following loop
  if (currentMoney >= 52900) {
    return tiers[11];
  }
  for (int i = 0; i < 12; i++) {
    if (currentMoney >= tiersBarriers[i]) {
      if (currentMoney < tiersBarriers[i + 1]) {
        return tiers[i];
      }
    }
  }
}
