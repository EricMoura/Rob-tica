#include "Adafruit_VL53L0X.h"
#include <Servo.h>
#include <SoftwareSerial.h> 

#define sv 13
#define PIN_IN1 5 
#define PIN_IN2 6
#define PIN_IN3 9  
#define PIN_IN4 10
#define PIN_ENA 3 
#define PIN_ENB 11

#define rxPin 14
#define txPin 15

SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

Servo s;
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int posSensor = 30;
int range = 0;
int x = 15;
int lect = -1;

void Comando(int dist, int pos) {
  //Serial.println(dist);
  if (dist < 250  ) {
    if (dist > 100){
      if (pos < 70) {
        dMotor(0);
        mySerial.println(0);
        delay(300);
        dMotor(6);
        mySerial.println(6);
        delay(500);
        dMotor(0);
      }
      else if (pos > 110) {
        dMotor(0);
        mySerial.println(0);
        delay(300);
        dMotor(4);
        mySerial.println(4);   
        delay(500);
        dMotor(0);
      }
      else if (pos >= 70 && pos <= 110) {
        dMotor(0);
        mySerial.println(0);
        delay(300);
        dMotor(2);
        mySerial.println(2);    
        delay(200);
        dMotor(0);
      }
    }
    else{
        dMotor(0);
        mySerial.println(0);
        delay(100);
        dMotor(2);
        mySerial.println(2);    
        delay(200);

      if (pos <= 90) {
        dMotor(6);
        mySerial.println(6);
        delay(500);
        dMotor(0);
      }
      else if (pos > 90) {
        dMotor(4);
        mySerial.println(4);   
        delay(500);
        dMotor(0);
      }
    }
  }
  else{
    dMotor(8);
    mySerial.println(8);
    delay(100);
  }
}

void dMotor(int direcao) {
  if (direcao == 0) {  //Parar
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
    //Serial.println("Parar");
  } else if (direcao == 8) {  //Frente
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    //Serial.println("Frente");
  } else if (direcao == 2) {  //Trás
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    //Serial.println("Trás");
  } else if (direcao == 6) {  //Direita
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    //Serial.println("Direita");
  } else if (direcao == 4) {  //Esquerda
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    //Serial.println("Esquerda");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  mySerial.begin(38400);
  s.attach(sv);
  s.write(90);
  
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  analogWrite(PIN_ENA,160);
  analogWrite(PIN_ENB,160);

  while (!lox.begin()) {
    Serial.println("VL53L0X não conectado!");
    delay(1000);
  }
  
  while(lect == -1){
    lect = mySerial.read(); 
    Serial.println(lect);
  }

  delay(3000);
  dMotor(8);
  mySerial.println(8);  
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  for(posSensor; posSensor <= 150; posSensor+=x){
    s.write(posSensor);
    if(lox.begin()){
      lox.rangingTest(&measure, false);
      if (measure.RangeStatus != 4) {
        Serial.println(measure.RangeMilliMeter);
        Comando(measure.RangeMilliMeter, posSensor);
      }
    }
  }
  for(posSensor; posSensor >= 30; posSensor-=x){
    s.write(posSensor);
    if(lox.begin()){    
      lox.rangingTest(&measure, false);
      if (measure.RangeStatus != 4) {
        Serial.println(measure.RangeMilliMeter);
        Comando(measure.RangeMilliMeter, posSensor);
      }
    }
  }
    
}

