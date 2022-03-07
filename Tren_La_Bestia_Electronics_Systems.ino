#include <IRremote.hpp>

const int IRpin = 3;
const int PROXpin = 2;

const int BRIGHTpin = 12;
const int GLOWpin = 7;

const int M1speedPin = 5;
const int M1fwd = 9;
const int M1rev = 8;

const int M2speedPin = 6;
const int M2fwd = 11;
const int M2rev = 10;

int IRin;
int controlState;
int proxState;
int prevState;
int awdToggle;

void setup() {
  pinMode(IRpin, INPUT);
  pinMode(PROXpin, INPUT);

  pinMode(M1fwd, OUTPUT);
  pinMode(M1rev, OUTPUT);
  pinMode(M2fwd, OUTPUT);
  pinMode(M2rev, OUTPUT);
  pinMode(BRIGHTpin, OUTPUT);

	Serial.begin(9600);
  IrReceiver.begin(IRpin, ENABLE_LED_FEEDBACK);
}

void single_dir_control() {
  switch (IRin) {
  case 19356:
    digitalWrite(M1fwd, HIGH);
    digitalWrite(M1rev, LOW);
    break;
  case 19355:
    digitalWrite(M1fwd, LOW);
    digitalWrite(M1rev, HIGH);
    break;
  case 19353:
    digitalWrite(M1fwd, LOW);
    digitalWrite(M1rev, LOW);
  }
}

void dual_dir_control() {
  switch (IRin) {
  case 19356:
    digitalWrite(M1fwd, HIGH);
    digitalWrite(M1rev, LOW);
    digitalWrite(M2fwd, HIGH);
    digitalWrite(M2rev, LOW);
    break;
  case 19355:
    digitalWrite(M1fwd, LOW);
    digitalWrite(M1rev, HIGH);
    digitalWrite(M2fwd, LOW);
    digitalWrite(M2rev, HIGH);
    break;
  case 19353:
    digitalWrite(M1fwd, LOW);
    digitalWrite(M1rev, LOW);
    digitalWrite(M2fwd, LOW);
    digitalWrite(M2rev, LOW);
  }
}

void single_speed_control() {
  analogWrite(M2speedPin, 0);
  switch (IRin) {
  case 128:
    analogWrite(M1speedPin, 28);
    break;
  case 129:
    analogWrite(M1speedPin, 56);
    break;
  case 130:
    analogWrite(M1speedPin, 84);
    break;
  case 131:
    analogWrite(M1speedPin, 112);
    break;
  case 132:
    analogWrite(M1speedPin, 140);
    break;
  case 133:
    analogWrite(M1speedPin, 168);
    break;
  case 134:
    analogWrite(M1speedPin, 196);
    break;
  case 135:
    analogWrite(M1speedPin, 224);
    break;
  case 136:
    analogWrite(M1speedPin, 255);
    break;
  case 137:
    analogWrite(M1speedPin, 0);
    break;
  }
}

void dual_speed_control() {
  switch (IRin) {
  case 128:
    analogWrite(M1speedPin, 28);
    analogWrite(M2speedPin, 28);
    break;
  case 129:
    analogWrite(M1speedPin, 56);
    analogWrite(M2speedPin, 56);
    break;
  case 130:
    analogWrite(M1speedPin, 84);
    analogWrite(M2speedPin, 84);
    break;
  case 131:
    analogWrite(M1speedPin, 112);
    analogWrite(M2speedPin, 112);
    break;
  case 132:
    analogWrite(M1speedPin, 140);
    analogWrite(M2speedPin, 140);
    break;
  case 133:
    analogWrite(M1speedPin, 168);
    analogWrite(M2speedPin, 168);
    break;
  case 134:
    analogWrite(M1speedPin, 196);
    analogWrite(M2speedPin, 196);
    break;
  case 135:
    analogWrite(M1speedPin, 224);
    analogWrite(M2speedPin, 224);
    break;
  case 136:
    analogWrite(M1speedPin, 255);
    analogWrite(M2speedPin, 255);
    break;
  case 137:
    analogWrite(M1speedPin, 0);
    analogWrite(M2speedPin, 0);
    break;
  }
}

void loop() {
  IRin = (IrReceiver.decodedIRData.decodedRawData);
  proxState = (digitalRead(PROXpin));

  if (IrReceiver.decode()) {
      Serial.println(IRin);
      IrReceiver.resume();
  }

  //RED-GREEN System Switch Buttons (Full Stop)
  switch (IRin) {
  case 460:
    controlState = 0;
    break;
  case 461:
    controlState = 1;
    break;
  }

  //Dual-Single Motor Switch Buttons
  switch (IRin){
  case 187:
    awdToggle = 1;
    break;
  case 157:
    awdToggle = 0;
    break;
  }

  //Proximity Sensor Status
  if (proxState == 0) {
    controlState = 0;
  }

  //Headlights Switch
  switch (IRin) {
  case 462:
    digitalWrite (BRIGHTpin, HIGH);
    break;
  case 463:
    digitalWrite (BRIGHTpin, LOW);
    break;
  }

  //Main Logic Tree
  if (controlState == 1) {
    if (awdToggle == 0){
      single_dir_control();
      single_speed_control();
    }
    else if (awdToggle == 1){
      dual_dir_control();
      dual_speed_control();
    }
  }
  else if (controlState == 0) {
    analogWrite(M1speedPin, 0);
    analogWrite(M2speedPin, 0);
  }
}
