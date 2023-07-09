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

bool block0[3] = { false, false, false };
bool block1[7] = { false, false, false, false, false, false, false };

void setup() { 
  lcd.begin(16, 2);
  lcd.print("hello, world!");
  Serial.begin(9600);
  pinMode(speaker, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(calcMoney());

  if (Serial.available()) {
      int x = Serial.read();
      if(x>=0 && x<=7 && (millis() > 1000 || bugfix)){
        money[x] += 1;
        waitingSounds += 1;
        bugfix = true;
      }
   }
  playsound();
}

long calcMoney(){
  return money[0]*1 + money[1]*2 + money[2]*5 + money[3]*10 + money[4]*20 + money[5]*50 + money[6]*100 + money[7]*200
}

uint32_t intToMoney(int i) {
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