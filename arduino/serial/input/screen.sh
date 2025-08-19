#!/bin/sh

# Connect to the serial port via device
# screen will redirect the input to the arduino for it to process
screen /dev/ttyACM0 9600,echo
