/*

https://www.amazon.com/SMAKN%C2%AE-AD9850-Module-Generator-0-40MHz/dp/B015K3BSYI
https://jontio.zapto.org/hda1/JDDS.html
https://www.analog.com/media/en/technical-documentation/data-sheets/ad9850.pdf
https://code.google.com/archive/p/ad9850-arduino/downloads

The library interfaces with the chip via serial mode, that is why only D7 is only needed to be connected rather than all data pins.
The premade board sold online does not wire the pin 2, 3, 4 to get serial mode on device startup so it needs to be set in software.
The library sets serial mode by pulsing (write high, then low) W_CLK and then FQ_UD on startup.
If the unconnected data pins are hooked to a scope on startup, they will shown to be oscillating (swinging from +-V).

The DDS can generate waves at a frequency band where we can watch it with a radio scanner.
Something like RTLSDR with GQRX can view the DDS generating data at the frequency.
Takes input from serial port to change bands and toggle on/off

There are sine/square wave output PINs that can be hooked to a logic analyzer/scope when running
The square wave outputs swings from 0 to +V volts at the frequency set

The sine wave is +-V

The sine wave output can be at a much higher frequency and the scope is able to capture (around double).
With a square wave, the limit frequency is alot lower.

*/

#include <AD9850.h>

// GND/VCC are hooked to GND/3.3V pins
enum {
  W_CLK = 5,
  FQ_UD = 6,
  D7    = 7,
  LED   = 13,
};

AD9850 dds(W_CLK, FQ_UD, D7);
int state;
double step;
double freq;
uint8_t phase;

void reset() {
  freq = 10e6;
  phase = 0;
  step = 1e6;
  state = 1;
  dds.setfreq(freq);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  reset();
}

void loop() {
  if (Serial.available() > 0) {
    event(Serial.read());
    digitalWrite(LED, HIGH);
  }
  digitalWrite(LED, LOW);
}

void event(int op) {
  char buf[64];
  
  switch (op) {
  case 'z':
    step /= 10;
    break;
  
  case 'x':
    step *= 10;
    break;
  
  case 'd':
    if (state != 0) {
      phase -= 1;
      dds.setphase(phase);
    }
    break;
  
  case 'f':
    if (state != 0) {
      phase += 1;
      dds.setphase(phase);
    }
    break;

  case 'a':
    if (state != 0) {
      freq -= step;
      dds.setfreq(freq);
    }
    break;
  
  case 's':
    if (state != 0) {
      freq += step;
      dds.setfreq(freq);
    }
    break;

  case 'w':
    reset();
    break;

  case 'q':
    state ^= 1;
    if (state == 0)
      dds.down();
    else {
      dds.setfreq(freq);
      dds.setphase(phase);
    }
    break;
  }

  snprintf(buf, sizeof(buf), "state: %s freq: %ld phase: %d freqstep: %ld", (state) ? "on" : "off", (long)freq, phase, (long)step);
  Serial.println(buf);
}
