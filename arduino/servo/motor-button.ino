enum {
  SWITCH = 2,
  MOTOR = 9,
};

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR, OUTPUT);
  pinMode(SWITCH, INPUT);
}

void loop() {
  int value;

  value = digitalRead(SWITCH);
  Serial.println(value);
  digitalWrite(MOTOR, value);
}
