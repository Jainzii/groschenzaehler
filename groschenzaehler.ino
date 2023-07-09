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
long money[8] = {0,0,0,0,0,0,0,0};

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
          money[i] += 1;
          lastActivation[i] = millis();
          Serial.write(i);
      }
    }
  }
}

long calcMoney(){
  return money[0]*1 + money[1]*2 + money[2]*5 + money[3]*10 + money[4]*20 + money[5]*50 + money[6]*100 + money[7]*200;
}