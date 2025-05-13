void setup() {
  Serial.begin(9600);
}

#define P(x) do { sprintf(buf, "%s: %u", #x, sizeof(x)); Serial.println(buf); } while (0);

void loop() {
  char buf[32];

  P(char);
  P(short);
  P(int);
  P(long);
  P(long long);
  P(float);
  P(double);
  P(void *);
  for (;;)
    ;
}
