/*

https://www.igorkromin.net/index.php/2013/02/15/converting-a-snes-controller-to-connect-to-a-nes-controller-port/
https://www.raphnet.net/electronique/snes_nes_usb/index_en.php
https://www.pagetable.com/?p=1365

To read a raw SNES controller, buy one of these:
https://www.aliexpress.com/item/32829318813.html
https://www.adafruit.com/product/3255

The 7 PIN connector can attach to the SNES controller while exposing a outer set of pins that we can hook
alligator clips to and connect that with a male connector to the Arduino header.

The SNES controller operates at 5V and uses the 4021 Parallel to Serial Shift register to read back the data.
The SNES PINOUT diagram is as follows:
|OOOO|OOO|
| 5V CLOCK LATCH DATA | UNC UNC GND

Arduino connection to SNES 7 PIN connector are as follows:
5V  -> 5V pin
GND -> GND pin
CLOCK/LATCH/DATA -> Digital pin

Toggle the CLOCK and LATCH to notify the controller to send data out via DATA. The DATA outputs are active low.
The delay between the toggles are lenient, but the original SNES had a 6-12 us delay.
We can delay up to milliseconds and it will read back the data fine.

*/

#define nelem(x) (sizeof(x) / sizeof(x[0]))

enum {
  CLOCK = 6,
  LATCH = 7,
  DATA  = 12,
};

uint32_t state;

void setup() {
  Serial.begin(115200);

  pinMode(CLOCK, OUTPUT);
  digitalWrite(CLOCK, HIGH);

  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);

  pinMode(DATA, OUTPUT);
  digitalWrite(DATA, HIGH);
  pinMode(DATA, INPUT);
}

void loop() {
  uint32_t newstate = readController();
  if (state != newstate) {
    state = newstate;
    printController(state);
  }
}

uint32_t readController() {
  digitalWrite(LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(LATCH, LOW);
  delayMicroseconds(6);

  uint32_t state = 0xfff;
  for (int i = 0; i < 16; i++) {
    digitalWrite(CLOCK, LOW);
    delayMicroseconds(6);
    
    uint32_t button = digitalRead(DATA);
    if (button == 0)
      state &= ~(1ul << i);

    digitalWrite(CLOCK, HIGH);
    delayMicroseconds(6);
  }
  return state;
}

void printController(uint32_t state) {
  static const char *buttons[] = {
    "B", "Y", "SELECT", "START", "UP", "DOWN", "LEFT", "RIGHT", "A", "X", "L", "R",
  };

  Serial.print("State: ");
  Serial.print(state);
  Serial.print(" [");
  for (int i = 0; i < nelem(buttons); i++) {
    if ((state&(1<<i)) == 0) {
      Serial.print(buttons[i]);
      if (i+1 < nelem(buttons))
        Serial.print(" ");
    }
  }
  Serial.println(" ]");
}
