// https://wiki.keyestudio.com/Ks0068_keyestudio_37_in_1_Sensor_Kit_for_Arduino_Starters#Project_27:_LM35_Linear_Temperature
// https://www.ti.com/lit/ds/symlink/lm35.pdf?ts=1602876969529&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLM35

void setup() {
  Serial.begin(9600);
}

void loop() {
  float c, f;

  c = 500.0f/1024.0f * analogRead(A0);
  f = c2f(c);
  Serial.print("Temp:");
  Serial.print(c);
  Serial.print(" C ");
  Serial.print(f);
  Serial.print(" F");
  Serial.println();
  
  delay(100);
}

float c2f(float c) {
  return c*9.0f/5.0 + 32.0f;
}
