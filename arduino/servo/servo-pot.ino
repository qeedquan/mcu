#include <Servo.h>

enum {
  POT = A0,
  SERVO = 9,
};

Servo servo;
int lastPot;

void setup() {
  lastPot = analogRead(POT);
  servo.attach(SERVO);
  Serial.begin(9600);
}

void loop() {
  int pot;
  
  pot = analogRead(POT);
  if (abs(lastPot - pot) >= 5) {
    lastPot = pot;
    Serial.println(pot);
    servo.write(map(pot, 0, 1023, 0, 179));
    delay(15);
  }
}
