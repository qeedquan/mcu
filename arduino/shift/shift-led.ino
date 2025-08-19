// https://www.arduino.cc/en/Tutorial/ShftOut11
// A shift register allows one to expand the number of PINs
// we can connect to, the shift register itself contains more output PINs
// and then we can latch our output to those PINs and therefore be able to talk
// to more devices
enum {
  LATCH = 8,
  CLOCK = 12,
  DATA = 11
};

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int i;

  for (i = 0; i < 256; i++) {
    // set the latch pin low, this will make it "buffer" the bits we send
    // so the updates don't happen until we set it high again
    digitalWrite(LATCH, LOW);
    
    // shift out the bits, this will move the data into the buffer
    shiftOut(DATA, CLOCK, MSBFIRST, i);  

    // take the latch pin high, means that we are done writing and the
    // buffer should contain what we wrote so far
    digitalWrite(LATCH, HIGH);

    Serial.println(i);
    
    // pause before next value:
    delay(500);
  }
}
