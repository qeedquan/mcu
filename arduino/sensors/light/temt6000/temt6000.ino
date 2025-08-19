enum {
  LIGHTPIN = A0
};

void setup() {
  // if no connection read back a large value
  pinMode(LIGHTPIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // sensitive to light, more lux means the value will be larger
  float val = analogRead(LIGHTPIN);
  Serial.println(val);
  delay(100);
}
