#define LAZR1 14  // pins of the photoresistors
#define LAZR2 13
#define LAZR3 17
#define LAZR4 18
#define LAZR5 21
#define LAZR6 16
#define LAZR7 19
#define LAZR8 15
#define NUM_LAZR 8

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL4zQckm-w9"
#define BLYNK_TEMPLATE_NAME "Sommerprojekt 07"
#define BLYNK_AUTH_TOKEN "uWHTXzitvEQQiPCWWlOe7e1aywhxUhYZ"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <string>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Groschenzaehler Inc";
char pass[] = "pringles69";

long lastActivation[NUM_LAZR] = { 0, 0, 0, 0, 0, 0, 0, 0 };
long money[8] = {0,0,0,0,0,0,0,0};

BlynkTimer t;

long lastBlynkUpdate = 0;

int lazr[NUM_LAZR] = { LAZR1, LAZR2, LAZR3, LAZR4, LAZR5, LAZR6, LAZR7, LAZR8 };

char *tiers[] = {"Freibier", "Center Shock", "Bayrisch Creme", "Pringles", "ESP32", "Döner", "Kinoticket", "Zelda: TotK", "10k Robux", "Fallschirmsprung", "Semesterbeitrag", "PS5"};
double tiersBarriers[12] = {0.0, 0.05, 0.6, 2.49, 2.79, 6.0, 8.99, 59.99, 119.99, 249.99, 399.82, 529.0};

double totalMoney = 0.0;

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  t.setInterval(420, updateBlynk);
}

//loop through all essential components as fast as possible
void loop() {
  t.run();
  sense();
  Blynk.run();

}

//update the virtual pins every 420ms
void updateBlynk(){
  Blynk.virtualWrite(V3, calcMoney() / 100.0);
  Blynk.virtualWrite(V1, dropTier());
}

//Terminal virtual pin
BLYNK_WRITE(V2) {
  String value = param.asString();
  readTerminalInput(value);
}

//check the lightbarrier
void sense() {
  for (int i = 0; i < 8; i++) {
    if (millis() - lastActivation[i] > 69) {  //nice, ensures, that noch coins are countet multiple times
      if (digitalRead(lazr[i]) == 0) {
          money[i] += 1;
          lastActivation[i] = millis();
          Serial.write(i);
      }
    }
  }
}

//convert money array to cents
long calcMoney(){
  return money[0]*1 + money[1]*2 + money[2]*5 + money[3]*10 + money[4]*20 + money[5]*50 + money[6]*100 + money[7]*200;
}

//Reset virtual pin
BLYNK_WRITE(V0) {
  for (int i = 0; i < 8; i++){
    money[i] = 0;
  }
  Serial.write(69);
}

//determines the current tier
char * dropTier() {
  char* tier = "";
  if(calcMoney() >= 52900) {
    tier = tiers[11];
  }

  for (int i = 0; i < 12; i++) {
     if(calcMoney() / 100.0 >= tiersBarriers[i]) {
      if(calcMoney() / 100.0 < tiersBarriers[i+1]) {
        tier = tiers[i];
      }
    } 
  }
  return tier;
}

//Helper functions to determine what the user typed in the Terminal
void readTerminalInput(String input) {
  int spacePos = input.indexOf(" ");
  String parameter = input.substring(spacePos + 1);
  convertToNumber(parameter);
}

void convertToNumber(String input) {
  String validInput = "";
  for (int i = 0; i < input.length(); i++) {
    char digit = input.charAt(i);
    if (isDigit(digit)) {
      validInput += digit;
    }
  }
  if (validInput.equals("")) {
    validInput = "0";
  }
  int number = validInput.toInt();
  while (number > 0){
    if(number >= 200){
      number -= 200;
      money[7] += 1;
      Serial.write(15);
    } else if(number >= 100){
      number -= 100;
      money[6] += 1;
      Serial.write(14);
    } else if(number >= 50){
      number -= 50;
      money[5] += 1;
      Serial.write(13);
    } else if(number >= 20){
      number -= 20;
      money[4] += 1;
      Serial.write(12);
    } else if(number >= 10){
      number -= 10;
      money[3] += 1;
      Serial.write(11);
    } else if(number >= 5){
      number -= 5;
      money[2] += 1;
      Serial.write(10);
    } else if(number >= 2){
      number -= 2;
      money[1] += 1;
      Serial.write(9);
    } else if(number >= 1){
      number -= 1;
      money[0] += 1;
      Serial.write(8);
    }
    delay(10);
  }
}
