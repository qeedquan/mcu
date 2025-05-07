// https://wiki.keyestudio.com/Ks0068_keyestudio_37_in_1_Sensor_Kit_for_Arduino_Starters#Project_29:_ADXL345_Three_Axis_Acceleration
#include <Wire.h>

#define ADXL345_ADDRESS       (0xA6 >> 1)
#define ADXL345_REGISTER_XLSB  0x32

void i2c_write(int address, byte reg, byte data) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void i2c_read(int address, byte reg, int count, byte *data) {
  int i = 0;
  
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.requestFrom(address, count);
  while (Wire.available()) {
    char c = Wire.read();
    data[i++] = c;
  }
  Wire.endTransmission();
}

void init_adxl345() {
  byte data = 0;

  // 13-bit mode +- 16g
  i2c_write(ADXL345_ADDRESS, 0x31, 0x0B);

  // power up
  i2c_write(ADXL345_ADDRESS, 0x2D, 0x08);

  // X Y Z
  i2c_write(ADXL345_ADDRESS, 0x1E, 0x00);
  i2c_write(ADXL345_ADDRESS, 0x1F, 0x00);
  i2c_write(ADXL345_ADDRESS, 0x20, 0x05);

  // validate
  i2c_read(ADXL345_ADDRESS, 0x00, 1, &data);
  if (data == 0xE5)
    Serial.println("Successfully initialized sensor");
  else
    Serial.println("Failed to initialize sensor");
}

void read_adxl345(int xyz[3]) {
  byte bytes[6] = {0};

  // Read 6 bytes from the ADXL345
  i2c_read(ADXL345_ADDRESS, ADXL345_REGISTER_XLSB, 6, bytes);
  for (int i = 0; i < 3; i++) {
    xyz[i] = bytes[2*i] | (bytes[2*i + 1]<<8);
  }
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  init_adxl345();
}

void loop() {
  const float scale = 3.9f/1000.0f;
  char buf[128];
  int xyz[3];

  read_adxl345(xyz);
  Serial.print("ACCEL: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(xyz[i]*scale);
    Serial.print(" ");
  }
  Serial.print("\n");
  delay(100);
}
