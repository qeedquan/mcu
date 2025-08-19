// https://www.sparkfun.com/products/12938
// FM tuner, hook it up to a standard headphone jack to listen

#include <SparkFunSi4703.h>

enum {
  RST = 2,
  SDIO = A4,
  SCLK = A5,
  SCT = 3,
};

Si4703_Breakout radio(RST, SDIO, SCLK, SCT);
int channel;
int volume;

int rdstimeout;
char rds[10];

void usage() {
  Serial.println("h   | Display this message");
  Serial.println("+ - | Volume (0-15)");
  Serial.println("u d | Seek up/down");
  Serial.println("r   | Listen for RDS data (15 sec timeout");
  Serial.println("Input a command");
}

void setup() {
  Serial.begin(9600);

  usage();
  
  volume = 10;
  rdstimeout = 15000;
  radio.powerOn();
  radio.setVolume(volume);
}

void loop() {
  if (!Serial.available()) {
    return;
  }

  char ch = Serial.read();
  switch (ch) {
    case 'h':
      usage();
      return;
    case 'u':
      channel = radio.seekUp();
      break;
    case 'd':
      channel = radio.seekDown();
      break;
    case '+':
      if (++volume > 15)
        volume = 15;
      radio.setVolume(volume);
      break;
    case '-':
      if (--volume < 0)
        volume = 0;
      radio.setVolume(volume);
      break;
    case 'r':
      Serial.println("RDS: ");
      radio.readRDS(rds, rdstimeout);
      Serial.println(rds);
      return;
  }
  show();
}

void show() {
  Serial.print("Channel: ");
  Serial.print(channel);
  Serial.print(" Volume: ");
  Serial.println(volume);
}
