#include <NewPing.h>

#define LAZR1 17  // Trigger and Echo Pin of Ultrasonic Sensor
#define LAZR2 18
#define LAZR3 13
#define LAZR4 14
#define LAZR5 15
#define LAZR6 16
#define LAZR7 19
#define LAZR8 21
#define MAX_DISTANCE 20
#define NUM_LAZR 8


long lastActivation[NUM_LAZR] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint32_t money = 0;

int lazr[NUM_LAZR] = { LAZR1, LAZR2, LAZR3, LAZR4, LAZR5, LAZR6, LAZR7, LAZR8 };

void setup() {
  Serial.begin(9600);
}

void loop() {
  sense();
}

//TODO: Blynkapp schauen, Zusammenbauen, Präsi

void sense() {
  for (int i = 0; i < 8; i++) {
    if (millis() - lastActivation[i] > 69) {  //nice
      if (digitalRead(lazr[i]) == 0) {
          //rausnehmen, wenn alles laser ausgerichtet sind
          money += intToMoney(i);
          lastActivation[i] = millis();
          Serial.write(i);
      }
    }
  }
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