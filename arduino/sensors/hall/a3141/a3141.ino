// https://components101.com/sites/default/files/component_datasheet/A3144%20Hall%20effect%20Sensor.pdf
// https://makersportal.com/blog/2018/10/3/arduino-tachometer-using-a-hall-effect-sensor-to-measure-rotations-from-a-fan
enum {
  HALLPIN = 2,
};

void setup() {
  // if no strong magnetic field read back high
  pinMode(HALLPIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // sensitive to magnetic field, generally used for rotation
  // sensor, to do that for a digital pin, use a loop and measure
  // how many time the hall sensor detects something in a certain
  // period as rotational things repeats itself so we can calculate
  // the speed of the rotation based on that
  int val = digitalRead(HALLPIN);
  Serial.println(val);
  delay(100);
}
