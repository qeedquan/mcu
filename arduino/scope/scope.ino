// hook analog pins to a oscilloscope and see the waveforms
// https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/

#define nelem(x) (sizeof(x)/sizeof(x[0]))

// channel order in scope capture
int pins[] = {9, 3, 5, 6};

unsigned long lastTime;
double tick;

void setup() {
  int i;
  for (i = 0; i < nelem(pins); i++)
    pinMode(pins[i], OUTPUT);

  analog1();
  tone1();
}

// test what waveform we get when we ramp
// the values written determine the duty cycle and pulse length
// smaller values lead to sharp pulses with longer time before
// the next pulse, while larger values are longer pulse but shorter
// time before the next pulse
// it should pulse between vcc and gnd, which is 5v and 0v respectively
void analog1() {
  int i;

  // we just need one write once to setup a PWM, it will be a stable set of square
  // waves until we change it again with another write
  for (i = 0; i < nelem(pins); i++)
    analogWrite(pins[i], 255 * i / nelem(pins));
  
  delay(1000);
  
  for (i = 0; i < nelem(pins); i++)
    analogWrite(pins[i], 0);

  // we can figure out how fast the PWM frequency is by looking at how long it takes for 1 pulse to cycle
  // for example if 1 pulse takes 2.0146 millisecond, then we have 1/(2.0146*1e-3) ~ 488 hz, which is close to
  // what arduino documents
  // the faster PWM runs at 1/(1.02464*1e-3) ~975 hz
  
  // to figure out the duty cycle look at how long the square wave is high before shutting down and rise
  // again in the next phase, so if we write a value 178, it will be 178/255 * 2.0146 millisecond, which will give
  // the time of how long the square wave is high in that window

  // analogWrite allows you to vary the window size of it being high/low, but it keeps the frequency to be the same
  // as the PWM frequency
  analogWrite(pins[0], 1);
  analogWrite(pins[1], 83);
  analogWrite(pins[2], 178);
  analogWrite(pins[3], 255);
}

void tone1() {
  // tone keeps the duty cycle at 50%, but it allows us to vary the frequency
  for (i = 0; i < nelem(pins); i++)
    tone(pins[i], 440);
	
  delay(1000);

  for (i = 0; i < nelem(pins); i++)
    tone(9, 880);

  delay(1000);
}

void loop() {
  unsigned long now;
  double dt, val;
  
  now = millis();
  dt = (now - lastTime) / 1e3;
  lastTime = now;

  tick += dt;
  if (tick > 1.0)
    tick -= 1.0;

  val = 255*sin(2*M_PI*tick);
  analogWrite(pins[2], val);
}
