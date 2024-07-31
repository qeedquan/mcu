enum {
  CTRL1 = 2,
  CTRL2 = 3,
  ENABLE = 9,
  DIRECTION = 4,
  ON_OFF = 5,
  POT = A0,
};

int lastOnOff;
int lastDir;
int motorDir;
int motorEnabled;

void setup() {
  Serial.begin(38400);
  pinMode(DIRECTION, INPUT);
  pinMode(ON_OFF, INPUT);
  pinMode(CTRL1, OUTPUT);
  pinMode(CTRL2, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, LOW);
}

void loop() {
  char buf[80];
  int onoff, dir;
  int speed;

  onoff = digitalRead(ON_OFF);
  delay(1);
  
  dir = digitalRead(DIRECTION);
  speed = analogRead(POT)/4;

  if (onoff != lastOnOff && onoff == HIGH) {
    motorEnabled = !motorEnabled;
  }
  
  if (dir != lastDir && dir == HIGH) {
    motorDir = !motorDir;
  }
  
  snprintf(buf, sizeof(buf), "onoff %d dir %d speed %d motor_enabled %d motor_dir %d",
    onoff, dir, speed, motorEnabled, motorDir);
  Serial.println(buf);

  digitalWrite(CTRL1, (motorDir) ? HIGH : LOW);
  digitalWrite(CTRL2, (motorDir) ? LOW : HIGH);
  analogWrite(ENABLE, (motorEnabled) ? speed : 0);
  
  lastOnOff = onoff;
  lastDir = dir;
}
