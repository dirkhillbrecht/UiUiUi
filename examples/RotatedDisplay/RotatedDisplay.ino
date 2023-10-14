// SPDX-License-Identifier: BSD-2-Clause
// (C) 2023 Dirk Hillbrecht

// Use a 90 degrees rotated display.

// This example uses a rotated display (90°) to show the time since the last reboot.
// It is based on the SimpleTimerLoop example program and only modifies the output.


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
// The "U8G2_R1" constant as first parameter actually declares the orientation of the display.
// This must be used also for all other display definitions.
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R1, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// *** UI definition ***

// Now, the UI is defined. This also happens statically and globally.
// We have a very simple UI here: Apart from the display manager, it consists of only one element

// The UI is a bit different as the one from SimpleTimerLoop: We use three labels for hour, minute and second.
UITextLine secondsLine=UITextLine(u8g2_font_crox3cb_tf);
UITextLine minutesLine=UITextLine(u8g2_font_crox3cb_tf,&secondsLine);
UITextLine hoursLine=UITextLine(u8g2_font_crox3cb_tf,&minutesLine);

// We stack tghe three labels above each other in three rows (as the display is "longer than wide")
UIRows theTimeLines=UIRows(&hoursLine);

// We use an UIEnvelope to center the output in both directions on the display.
UIEnvelope mainEnvelope=UIEnvelope(UIExpansion::Both,UIAlignment::Center,&theTimeLines);

// And finally, we bring everything together in the display instance.
UIDisplay displayManager=UIDisplay(&mainEnvelope);


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
  hour=(minute/60)%60;
  minute=minute%60;
  second=second%60;
}

// Tiem is stored in three separate Strings
char hoursString[3]="00";
char minutesString[3]="00";
char secondsString[3]="00";

// Convert one part of the time (hour, minute or second) into its respective String
void convertTimePart(int timePart,char* timePartString) {
  itoa(timePart,timePartString,10); // Perform the conversion using the standard library
  if (timePartString[1]=='\0') { // and prepend a leading zero if necessary
    timePartString[2]=timePartString[1];
    timePartString[1]=timePartString[0];
    timePartString[0]='0';
  }
}

// Convert the complete time
void convertTime() {
  convertTimePart(hour,hoursString);
  convertTimePart(minute,minutesString);
  convertTimePart(second,secondsString);
}


// ********************
// *** Main control ***
// ********************

/* Setup function: Called once after boot */
void setup() {
  u8g2.begin();  // Init U8g2, this is done outside of UiUiUi
  displayManager.init(&u8g2);  // Now init the interface including layouting
  hoursLine.setText("Up");  // Set an initial text on the upper two text lines...
  minutesLine.setText("since");  // This is already quite wide.
  displayManager.render(&u8g2,true);   // ...and render it.
  delay(2000);  // Wait 2 seconds so that the initial text is shown
  hoursLine.setText(hoursString);  // and change the text of the fields...
  minutesLine.setText(minutesString);  // to the static time storage strings.
  secondsLine.setText(secondsString);
  // No render here means: The changed string is not shown so far
}

/* Loop function: Called again and again until the device is turned off. */
void loop() {
  int oldhour=hour;  // Store current settings so that rendering can be suppressed for unchanged texts
  int oldminute=minute;
  setTime();  // Set the time
  convertTime();  // Convert the int-based stored time into parts of the time string
  if (oldhour!=hour)
    hoursLine.updateText();  // Inform about update only if the respective String actually changed.
  if (oldminute!=minute)
    minutesLine.updateText();
  secondsLine.updateText();  // Seconds have always changed by program design
  displayManager.render(&u8g2);  // Render the UI: Updates text

  // Wait until the next full second is over
  delay(1010-(millis()%1000));  // always wait at least 10 ms
}

// end of file
