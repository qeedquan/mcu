#define nelem(x) (sizeof(x) / sizeof(x[0]))

int pins[] = {2, 3, 4, 5};
int curpin;
unsigned long lastms;

void printState() {
  int i;
  Serial.print(curpin);
  Serial.print(" | ");
  for (i = 0; i < nelem(pins); i++) {
    Serial.print(digitalRead(pins[i]));
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  int i;

  // by default, if we hook up the probes to a logic analyzer
  // the input can be floating since we didn't have any pullups/pulldowns
  // or a known state if the probes to the logic analyzer internally pulls it using
  // internal shunt resistor, for our case the analyzer pulls it low
  for (i = 0; i < nelem(pins); i++) {
    pinMode(pins[i], INPUT);
  }
  
  Serial.begin(38400);
}

void loop() {
  unsigned long ms;
  
  // when we print the state the default is either floating, pulled high or low
  // it is not until we update use the input pullup that forces to be high
  printState();
  
  // toggle the input mode to use the internal pullup resistor in the arduino
  // we should see it go high if the value was low before
  ms = millis();
  if ((ms - lastms) > 5000 && curpin < nelem(pins)) {
    pinMode(pins[curpin++], INPUT_PULLUP);
    delay(100);
    lastms = ms;
  }
}
