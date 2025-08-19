/*

https://learn.adafruit.com/using-an-infrared-library/hardware-needed
https://www.adafruit.com/product/157
https://www.digikey.com/en/maker/blogs/2021/understanding-the-basics-of-infrared-communications

*/

/*

An IR sensor usually has 3 PINs

VCC (5V)
GND
Output

The Output PIN will generate a pulse train whenever the sensor detects IR.
The IR remotes all send out pulses but they have different code to button mappings to distinguish themselves.
The same button on one remote does not necessary map to the same pulse signature on another remote.

IR wavelength is 780 nm to 1 mm.

Since alot of things emit IR, need to design a protocol to distinguish data from noise.
Some popular ones:
NEC:            38 khz carrier frequency
RC5 (Phillips): 36 khz carrier frequency

The protocol defines how long to send the pulses separated by delay.
This basically creates a message frame that software can use to decode it.
One message frame can take up to ~25 ms to transmit due to the pulse delays to distinguish against noise,
so it is a low speed protocol meant for small amount of data.

*/

#include "IRLibAll.h"

IRrecvPCI irrcv(2);
IRdecode  irdec;

void setup() {
  Serial.begin(9600);
  delay(2000);
  irrcv.enableIRIn(); 
  Serial.println("Ready to receive IR signals");
}

void loop() {
  if (irrcv.getResults()) {
    irdec.decode();
    irdec.dumpResults(false);
    irrcv.enableIRIn();
  }
}
