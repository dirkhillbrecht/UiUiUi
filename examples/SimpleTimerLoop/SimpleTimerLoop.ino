// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// Simple example: One line which may show the times since reboot of the device

// This example shows the most basic principles of UiUiUi:
//
// - Define the GUI in a really simple example
// - Initialize the GUI on screen
// - Update the contents of the GUI
// - Render the GUI on the display

// This example uses a synchronous scheme: The code is in the usual setup() and loop() methods,
// the loop() method contains a delay() so that it is called at the appropriate times.


// **************************
// *** Include statements ***
// **************************

// You need the U8g2 library
#include <U8g2lib.h>

// And we load the complete UiUiUi library with the combining header file
#include <UiUiUi.h>


// **************************
// *** Global definitions ***
// **************************

// As usual, the representation of the display hardware is defined globally and statically at the begin

// Define your display here, this is for a Heltec WiFi LoRa 32
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// *** UI definition ***

// Now, the UI is defined. This also happens statically and globally.
// We have a very simple UI here: Apart from the display manager, it consists of only one element

// Line which shows the time, will render itself centered on its space
UITextLine theTimeLine=UITextLine(u8g2_font_crox3cb_tf);

// As the time line is the only UI element, just put it into the display representation.
// It will get the complete space and render itself centered in both directions.
UIDisplay displayManager=UIDisplay(&theTimeLine);


// *******************************
// *** The actual program code ***
// *******************************

// Variables holding hour, minute, and second
int hour,minute,second;

/* Set the time: This implementation just derives it from the millis(),
 * so the program show the time since the last boot of the device.
 * Another implementation could read a time from some source (NTP server, DCF-77 receiver...)
 * and show it. Then, this would be a real clock.
 */
void setTime() {
  second=millis()/1000;
  minute=second/60;
  hour=(minute/60)%24;
  minute=minute%60;
  second=second%60;
}

// This is where the time string is stored
char timeString[9]="00:00:00";

// Convert one part of the time (hour, minute or second) into a String and write it into the timeString variable
void convertTimePart(int timePart,int startIdx) {
  char timePartString[3];
  itoa(timePart,timePartString,10); // Convert the timePart integer into a String stored in timePartString
  if (timePartString[1]=='\0') {
    timeString[startIdx]='0';
    timeString[startIdx+1]=timePartString[0];
  }
  else {
    timeString[startIdx]=timePartString[0];
    timeString[startIdx+1]=timePartString[1];
  }
}

// Convert the complete time
void convertTime() {
  convertTimePart(hour,0);
  convertTimePart(minute,3);
  convertTimePart(second,6);
}


// ********************
// *** Main control ***
// ********************

/* Setup function: Called once after boot */
void setup() {
  u8g2.begin();  // Init U8g2, this is done outside of UiUiUi
  displayManager.init(&u8g2);  // Now init the interface including layouting
  theTimeLine.setText("Up since");  // Set an initial text on the text line (8 chars max!)...
  displayManager.render(&u8g2,true);   // ...and render it.
  delay(2000);  // Wait 2 seconds so that the initial text is shown
  theTimeLine.setText(timeString);  // and change the time line text to the static time storage string.
  // No render here means: The changed string is not shown so far
}

/* Loop function: Called again and again until the device is turned off. */
void loop() {
  setTime();  // Set the time
  convertTime();  // Convert the int-based stored time into parts of the time string
  theTimeLine.updateText();  // Inform the text line that its content has changed (in situ)
  displayManager.render(&u8g2);  // Render the UI: Updates text

  // Wait until the next full second is over
  delay(1010-(millis()%1000));  // always wait at least 10 ms
}

// end of file
