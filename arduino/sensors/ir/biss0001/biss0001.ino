// http://www.ladyada.net/media/sensors/BISS0001.pdf
// https://wiki.keyestudio.com/Ks0052_keyestudio_PIR_Motion_Sensor
// https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor/how-pirs-work
// https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor
enum {
  PIRPIN = 2,
};

int state = LOW;

void setup() {
  pinMode(PIRPIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // everything emits some IR, PIR will be high when it detects
  // a change in that motion
  int val = digitalRead(PIRPIN);
  if (val == HIGH) {
    if (state == LOW) {
      Serial.println("Detected motion");
      state = HIGH;
    }
  } else {
    if (state == HIGH) {
      Serial.println("Motion ended");
      state = LOW;
    }
  }
}
