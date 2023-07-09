#include <LiquidCrystal.h>

#define speaker 12

const int rs = 10, en = 11, d0 = 2, d1 = 3, d2 = 4, d3 = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);

int count = 0;
int waitingSounds = 0;
long last = 0;
int limit = 10;
bool bugfix = false;
int lastInput = 0;
long money[8] = {0,0,0,0,0,0,0,0};
double moneyAmount = 0;
char* currentTier = "";

char *tiers[] = {"Freibier", "Center Shock", "Bayrisch Creme", "Pringles", "ESP32", "Döner", "Kinoticket", "Zelda: TotK", "10k Robux", "Fallschirmsprung", "Semesterbeitrag", "PS5"};
double tiersBarriers[12] = {0.0, 0.05, 0.6, 2.49, 2.79, 6.0, 8.99, 59.99, 119.99, 249.99, 399.82, 529.0};

bool block0[3] = { false, false, false };
bool block1[7] = { false, false, false, false, false, false, false };

void setup() { 
  lcd.begin(16, 2);
  lcd.print("hello, world!");
  Serial.begin(9600);
  pinMode(speaker, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print(currentTier);

  lcd.setCursor(0, 1);
  lcd.print(moneyAmount);
  lcd.print(" Euro");

  if (Serial.available()) {
      // amount of cent is read
      int moneyTmp = Serial.read();

      // change from cent to euro
      moneyAmount = moneyTmp / 100;

      // string currentTier gets set accpoding to the current tier string o.O
      currentTier = dropTier(moneyAmount);
   }
  playsound();
}


void playsound() {
  if (count < limit - 1 && waitingSounds > 0) {
    if (!block0[0] && (millis() - last >= 500)) {
      block0[0] = true;
      tone(speaker, 988, 100);
      last = millis();
    } else if (millis() - last >= 100 && (waitingSounds > 0) && !block0[1] && block0[0]) {
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
  } else if (count >= limit - 1 && waitingSounds > 0) {
    if (!block1[0] && (millis() - last >= 880)) {
      block1[0] = true;
      tone(speaker, 1319, 125);
      last = millis();
    } else if (millis() - last >= 130 && (waitingSounds > 0) && !block1[1] && block1[0]) {
      block1[1] = true;
      tone(speaker, 1568, 125);
    } else if (millis() - last >= 260 && (waitingSounds > 0) && !block1[2] && block1[1]) {
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
      count = 0;
      for (int i = 0; i < 7; i++) {
        block1[i] = false;
      }
    }
  }
}

// returns tier string according to current money amount
char * dropTier(int money) {
  double currentMoney = money / 100;
  // to prevent array index out of bounds exception in following loop
  if(currentMoney >= 529.0) {
    return tiers[11];
  }

  for (int i = 0; i < 12; i++) {
     if(currentMoney >= tiersBarriers[i]) {
      if(currentMoney < tiersBarriers[i+1]) {
        return tiers[i];
      }
    } 
  }
}
