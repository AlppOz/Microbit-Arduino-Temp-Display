/*
Displays the temperature in the serial and on the LED Matrix
Made exclusively for Uno R4 Wifi model as it uses its unique LED Matrix
*/

#include <Wire.h>
#include <ArduinoR4DigitDisplay.h>
#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

//Modify these constants as you need before embedding
const int SENTINEL = -1; //choose a value that you never expect to recieve from the microbit
const int SERIAL_CHANNEL = 115200; //display the serial monitor on this channel
const int IC = 13; //I2C slave address
const int READING_TIME = 1000; //time between each reading in ms. Needs to match with the microbit code

volatile int received_value = SENTINEL;
volatile bool new_data_status = false;

void youve_got_mail(int doesnt_matter); //I2C receive callback: triggered automatically when Microbit sends data
void reset_frame(); //resets the frame to default position:

uint8_t frame[8][12] = { //Default frame always showing the degrees Celsius symbol
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void setup() {
  Serial.begin(SERIAL_CHANNEL); 
  matrix.begin();
  Wire.begin(IC);
  Wire.onReceive(youve_got_mail);
}

void loop() {
  if (new_data_status) { //entering this path signifies the function fired fully. i.e. the wire is available
    if (received_value == SENTINEL) { //here the function fired but the value from the microbit couldn't be recieved
      Serial.println("Wire available but reading failed. Abort! Abort!");
    }
    else { //everything's fine yippie:
      Serial.println(received_value);

      drawNumber(received_value, frame);
      matrix.renderBitmap(frame, 8, 12);
      delay(READING_TIME);
      reset_frame(); //resetting for the next loop
    }
    new_data_status = false; //resetting for the next loop
  }
}

void youve_got_mail(int doesnt_matter) {
  if (Wire.available()) {
    received_value = Wire.read();
    new_data_status = true;
  }
}

void reset_frame() {
  matrix.renderBitmap(frame, 8, 12);
  for (int x = 0; x <= 7; x++) { //up to the Celsius symbol
    for (int y = 0; y <= 7; y++) {
      frame[y][x] = 0;
    }
  }
}
