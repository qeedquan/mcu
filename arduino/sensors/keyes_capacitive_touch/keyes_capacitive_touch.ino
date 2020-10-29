// https://wiki.keyestudio.com/Ks0068_keyestudio_37_in_1_Sensor_Kit_for_Arduino_Starters#Project_14:_Capacitive_Touch
enum {
  CAP = 2,
  LED = 13,
};

void setup() {
  pinMode(CAP, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int val = digitalRead(CAP);
  digitalWrite(LED, val);
  Serial.println(val);
  delay(100);
}
