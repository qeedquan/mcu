#include <oseppRobot.h>

OseppTBMotor whl(12, 11, LOW);
OseppTBMotor whr(8, 3, HIGH);
OseppRangeFinder rnf(2);

void setup() {
  Serial.begin(115200);
}

void loop() {
  int val = rnf.ping();
  Serial.println(val);

  whr.forward(0);
  whl.forward(255);
  delay(1000);
  
  whl.forward(0);
  whr.forward(255);
  delay(1000);
}
