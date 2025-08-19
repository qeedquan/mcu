/* 

https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/

*/

/*

A rotary encoder is a knob that outputs a signal when it is turned.
The knob can also be pressed as a button.

There are two types of rotary encoders, absolute and incremental.
Absolute gives the exact position of the knob (0-360 degrees)
Incremental gives the delta between the turns (+-1 every time the knob is turned).
The knobs wraps around instead of having a ending as in a potentiometer.

The way an incremental rotary encoder works is that there is a slotted disk connected to a common ground pin denoted as C.
There is two contact pins that are slotted on the disk, as the disk is turned the contact pin A and B comes into contact with pin C.
Whether or not it comes into the contact with the ground pin makes the output 1 or 0.
So this generates a square wave.

A and B pins are placed in such a way that they are 90 degree out of phase.
This allows us to figure out the direction we moved CW/CCW direction.

The change in either state tells us whether or not the knob has turned and the sameness of the value A and B tells us the direction we moved in.

The code below implements a incremental rotary encoder.

These are the standard PIN names (CLK/DT can be denoted as A or B but they are mutually exclusive):
CLK - Pin A/B
DT  - Pin A/B
SW  - Button for pressed (needs a pullup resistor, but we can use the internal pullup)
VCC - 5V
GND - GND

*/

enum {
  CLK = 2,
  DT = 3,
  SW = 4,
};

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() { 
  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  // Setup Serial Monitor
  Serial.begin(115200);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
}

void loop() {
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter--;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      counter++;
      currentDir ="CW";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Read the button state
  int button = digitalRead(SW);

  // If we detect LOW signal, button is pressed
  if (button == LOW) {
    // if 50ms have passed since last LOW pulse, it means that the
    // button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);
}
