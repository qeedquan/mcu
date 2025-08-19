enum {
  SENSOR = A0,
  PIEZO = 8,
};

int notes[] = {262, 294, 330, 349};

void setup() {
  Serial.begin(9600);
}

void loop() {
  const int duration = 100;

  int value;

  value = analogRead(SENSOR);
  Serial.println(value);

  if (value == 1023) {
    tone(PIEZO, notes[0], duration);
  } else if (990 <= value && value <= 1010) {
    tone(PIEZO, notes[1], duration);
  } else if (500 <= value && value <= 600) {
    tone(PIEZO, notes[2], duration);
  } else if (10 <= value && value <= 20) {
    tone(PIEZO, notes[3], duration);
  }
}
