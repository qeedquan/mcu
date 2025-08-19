struct RGB {
  int r, g, b;
};

enum {
  LED_BLUE = 9,
  LED_GREEN = 10,
  LED_RED = 11,

  LDR_RED = A0,
  LDR_GREEN = A1,
  LDR_BLUE = A2,
};

double C[][4][3] = {
  {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, {0.00, 0.33, 0.67}},
  {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, {0.00, 0.10, 0.20}},
  {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, {0.30, 0.20, 0.20}},
  {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {1.0, 1.0, 0.5}, {0.80, 0.90, 0.30}},
  {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {1.0, 0.7, 0.4}, {0.00, 0.15, 0.20}},
  {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {2.0, 1.0, 0.0}, {0.50, 0.20, 0.25}},
  {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {2.0, 1.0, 1.0}, {0.00, 0.25, 0.25}},
};

unsigned long lastTime;
double tick;
int pal;
RGB color;

#define nelem(x) (sizeof(x)/sizeof(x[0]))

void setup() {
  Serial.begin(9600);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  lastTime = millis();
}

void loop() {
  double dt;
  
  dt = timer();
  update(dt);

  analogWrite(LED_RED, color.r);
  analogWrite(LED_GREEN, color.g);
  analogWrite(LED_BLUE, color.b);
}

double timer() {
  unsigned long now;
  double dt;
  
  now = millis();
  dt = (now - lastTime) / 1000.0;
  lastTime = now;
  return dt;
}

void update(double dt) {
  double a0, b0, c0, d0;
  double a1, b1, c1, d1;
  double a2, b2, c2, d2;
  double r, g, b;
  long s0, s1, s2;
  char buf[32];
  int i;
  
  tick += dt*0.1;
  if (tick > 1.0) {
    tick -= 1.0;
    pal = (pal + 1) % nelem(C);
    sprintf(buf, "Palette %d", pal);
    Serial.println(buf);
  }

  i = pal;
  a0 = C[i][0][0], b0 = C[i][1][0], c0 = C[i][2][0], d0 = C[i][3][0];
  a1 = C[i][0][1], b1 = C[i][1][1], c1 = C[i][2][1], d1 = C[i][3][1];
  a2 = C[i][0][2], b2 = C[i][1][2], c2 = C[i][2][2], d2 = C[i][3][2];
  r = a0 + b0*cos(2*M_PI*(c0*tick + d0));
  g = a1 + b1*cos(2*M_PI*(c1*tick + d1));
  b = a2 + b2*cos(2*M_PI*(c2*tick + d2));

  s0 = analogRead(LDR_RED) / 4;
  s1 = analogRead(LDR_GREEN) / 4;
  s2 = analogRead(LDR_BLUE) / 4;

  color.r = r * 255 + s0;
  color.g = g * 255 + s1;
  color.b = b * 255 + s2;

  color.r = clamp(color.r, 0, 255);
  color.g = clamp(color.g, 0, 255);
  color.b = clamp(color.b, 0, 255);
}

int clamp(int x, int a, int b) {
  if (x < a)
    x = a;
  if (x > b)
    x = b;
  return x;
}
