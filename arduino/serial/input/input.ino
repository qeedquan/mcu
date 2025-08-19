void setup() {
  Serial.begin(9600);
  Serial.println("READY");
}

void loop() {
  char buf[32];
  if (Serial.available() > 0) {
    int data = Serial.read();
    snprintf(buf, sizeof(buf), "DATA: %c", data);
    Serial.println(buf);
  }
}
