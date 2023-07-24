#include "MQ131.h"

int enablePin = 9;  
float potval =0 ;
int TareVal=0;
void setup() {
  Serial.begin(9600);


  MQ131.begin(8,A2, LOW_CONCENTRATION, 1000000);  
  MQ131.calibrate();
  Serial.print(MQ131.getR0());
  Serial.print(MQ131.getTimeToRead());
  pinMode(A5, INPUT);
  pinMode(enablePin, OUTPUT);
  delay(3000);
  digitalWrite(enablePin, HIGH);
  TareVal=analogRead(A3);
}

void loop() {
  MQ131.sample();
  potval=MQ131.getO3(MG_M3);
  if(potval){
  Serial.print("Sent Value= ");
  Serial.println(potval);   
  delay(9600);
  }
  if(analogRead(A3)<TareVal-10){
    Serial.println(9);
  }
  else
  Serial.println(10);
}
