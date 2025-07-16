// https://www.mantech.co.za/Datasheets/Products/KS0008-190803A.pdf
// https://wiki.keyestudio.com/Ks0068_keyestudio_37_in_1_Sensor_Kit_for_Arduino_Starters#Project_34:_Joystick_Module
enum {
  JX = 0,
  JY = 1,
  JB = 3,
};

void setup() {
  pinMode(JB, INPUT);
  Serial.begin(9600);
}

void loop() {
  int x, y, b;
  char buf[80];
  
  x = analogRead(JX);
  y = analogRead(JY);
  b = digitalRead(JB);
  sprintf(buf, "(%d, %d) %d\n", x, y, b);
  Serial.print(buf);
  delay(100);
}
