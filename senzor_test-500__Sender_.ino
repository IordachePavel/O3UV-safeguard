#include "MQ131.h"

int enablePin = 9;  
float potval =0 ;

void setup() {
  Serial.begin(9600);

  MQ131.begin(8,A2, LOW_CONCENTRATION, 1000000);  
  MQ131.calibrate();
  Serial.print("This is a test");
  Serial.print("44.4");
  Serial.print(MQ131.getR0());
  Serial.print(MQ131.getTimeToRead());
  pinMode(enablePin, OUTPUT);
  delay(3000);
  digitalWrite(enablePin, HIGH);  
}

void loop() {
  MQ131.sample();
  potval=MQ131.getO3(MG_M3);
  Serial.print("Sent Value= ");
  Serial.println(potval);  
  delay(10000);
}
