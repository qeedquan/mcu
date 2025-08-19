enum {
  LED = 13
};

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  int ms = 1000;
  setled(HIGH, ms);
  setled(LOW, ms);
}

void setled(int state, int ms) {
  digitalWrite(LED, state);
  delay(ms);
}
