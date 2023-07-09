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

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL4zQckm-w9"
#define BLYNK_TEMPLATE_NAME "Sommerprojekt 07"
#define BLYNK_AUTH_TOKEN "uWHTXzitvEQQiPCWWlOe7e1aywhxUhYZ"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

long lastActivation[NUM_LAZR] = { 0, 0, 0, 0, 0, 0, 0, 0 };
long money[8] = {0,0,0,0,0,0,0,0};

int lazr[NUM_LAZR] = { LAZR1, LAZR2, LAZR3, LAZR4, LAZR5, LAZR6, LAZR7, LAZR8 };

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  sense();
}

//TODO: Blynkapp schauen, Zusammenbauen, Präsi

void sense() {
  for (int i = 0; i < 8; i++) {
    if (millis() - lastActivation[i] > 69) {  //nice
      if (digitalRead(lazr[i]) == 0) {
          money[i] += 1;
          lastActivation[i] = millis();
          long totalAmount = calcMoney();
          double totalAmountAsDecimal = totalAmount / 100;
          Blynk.virtualWrite(V4, totalAmountAsDecimal);
          Serial.write(totalAmount);
      }
    }
  }
}

long calcMoney(){
  return money[0]*1 + money[1]*2 + money[2]*5 + money[3]*10 + money[4]*20 + money[5]*50 + money[6]*100 + money[7]*200;
}
