// https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf
// https://www.pjrc.com/teensy/td_libs_OneWire.html
// http://sheepdogguides.com/lut/dstl2hello_ds18b20.htm
// we are using a presolder header sensor with resistor/capacitor
// the image is not indicative but its a one that preexist in fritzing

#include <OneWire.h>

#define nelem(x) (sizeof(x)/sizeof(x[0]))

enum {
  THERMOPIN = 10,
};

OneWire ds(THERMOPIN);
int ndevs = 0;

void setup(void) {
  Serial.begin(115200);
}

void loop(void) {
  byte i;
  byte present, resolution;
  byte data[12];
  byte addr[8];
  float celcius;

  if (!ds.search(addr)) {
    if (ndevs == 0)
      Serial.println("No devices found.");
    ds.reset_search();
    ndevs = 0;
    delay(250);
    return;
  }

  ndevs++;
  Serial.print("D=");
  for (i = 0; i < nelem(addr); i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.print("CRC is not valid!\n");
    return;
  }

  if (addr[0] != 0x28) {
    Serial.println("Device not DS18B20, continuing");
    return;
  }

  ds.reset();

  // initiate temperature conversion
  // datasheet specifies ~750ms for conversion
  // of 12 bit resolution, sleep more to be safe
  ds.select(addr);
  ds.write(0x44,1);
  delay(1000);

  // read scratchpad
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);
  
  for (i = 0; i < nelem(data); i++) {
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }

  resolution = (data[4]>>5) & 3;
  celcius = d2c(data);
  Serial.print("C=");
  Serial.print(celcius);
  Serial.print(" ");
  Serial.print("F=");
  Serial.print(c2f(celcius));
  Serial.print(" ");
  Serial.print(" ");
  Serial.print("R=");
  Serial.print(resolution);
  Serial.print(" ");
  Serial.print("P=");
  Serial.print(present,HEX);
  Serial.print(" ");
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();
}

float d2c(byte data[]) {
  float tab[] = { 0.05f, 0.25f, 0.125f, 0.0625f };
  int16_t val, res;

  res = (data[4]>>5) & 3;
  val = data[0] | (data[1]<<8);
  return val * tab[res];
}

float c2f(float c) {
  return c*9.0f/5.0f + 32;
}
