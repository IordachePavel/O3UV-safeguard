//def pin use
#include <Servo.h>

#include <SoftwareSerial.h>
SoftwareSerial bt(4, 5);



Servo myservo;

const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 83;
int pos[5] = { 1, 1, 83, 166, 249 }, currentPos = 1, number = 1, targetPos = 1;
int servoPos = 90, servoVector[2] = { 0, 180 }, servoTarget = 90;
String numberStr;
String Test;

void setup() {
  // def output pin
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  bt.begin(9600);
  Serial.begin(9600);

  //motor pos start set
  digitalWrite(dirPin, HIGH);
  for (int x = 0; x < 250; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  digitalWrite(dirPin, LOW);
  myservo.attach(6);
  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);
}

int translatorBt(int bt) {
  if (bt == 49)
    return 1;
  if (bt == 50)
    return 2;
  if (bt == 51)
    return 3;
  if (bt == 52)
    return 4;
  if (bt == 53)
    return 5;
  if (bt == 54)
    return 6;
  else return 0;
}

void servo900() {
  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);

  for (int servoPos = 90; servoPos >= 0; servoPos--) {
    myservo.write(servoPos);
    delay(15);
  }
  delay(1000);

  for (int servoPos = 0; servoPos <= 90; servoPos++) {
    myservo.write(servoPos);
    delay(15);
  }
  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);
}

void servo90180() {
  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);
  for (int servoPos = 90; servoPos <= 180; servoPos++) {
    myservo.write(servoPos);
    delay(15);
  }
  delay(500);
  for (int servoPos = 180; servoPos >= 90; servoPos--) {
    myservo.write(servoPos);
    delay(15);
  }
  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);
}

void PosUpdate() {
  if (currentPos != targetPos) {

    digitalWrite(dirPin, HIGH);

    if (targetPos > currentPos) {

      digitalWrite(dirPin, LOW);

      for (int i = currentPos; i <= targetPos; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
      }
      currentPos = targetPos;
    } else {
      digitalWrite(dirPin, HIGH);

      for (int i = currentPos; i >= targetPos; i--) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
      }
      currentPos = targetPos;
    }
  }
  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);
}

void loop() {

  while (Serial.available() > 0) {
    number = Serial.parseInt();
    if (number == 5) {
      servo900();
      Serial.println(number);
    } else if (number == 6) {
      servo90180();
      Serial.println(number);
    } else {
      if (number > 4 || number < 0)
        number = 0;
      if (number != 0)
        targetPos = pos[number];
      PosUpdate();
      Serial.println(targetPos);
    }
  }



  if (bt.available() > 0) {
    numberStr = bt.read();
    if (numberStr == "VENT ON") {
      Serial.println(9);
    } else if (numberStr == "VENT OFF") {
      Serial.println(10);
    } else {
      number = translatorBt(numberStr.toInt());
      if (number == 1) {
        targetPos = pos[2];
        PosUpdate();
        servo900();
      }

      if (number == 2) {
        targetPos = pos[2];
        PosUpdate();
        servo90180();
      }

      if (number == 3) {
        targetPos = pos[3];
        PosUpdate();
        servo900();
      }

      if (number == 4) {
        targetPos = pos[3];
        PosUpdate();
        servo90180();
      }

      if (number == 5) {
        targetPos = pos[4];
        PosUpdate();
        servo900();
      }

      if (number == 6) {
        targetPos = pos[4];
        PosUpdate();
        servo90180();
      }
    }
  }
}
