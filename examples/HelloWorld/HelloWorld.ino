// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// Most simplistic example: Print "Hello World" on the display

// This example shows the most basic principles of UiUiUi:
//
// - Define the user interface in a really simple example
// - Initialize the interface content
// - Render the interface on screen

// This example is neither dynamic nor interactive.
// It shows only the most basic concepts of UiUiUi.

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
UITextLine textLine=UITextLine(u8g2_font_6x10_tf);

// As the time line is the only UI element, just put it into the display representation.
// It will get the complete space and render itself centered in both directions.
UIDisplay displayManager=UIDisplay(&textLine);


// ********************
// *** Main control ***
// ********************

/* Setup function: Called once after boot */
void setup() {
  
  // Init U8g2, this is done outside of UiUiUi
  u8g2.begin();

  // Set an initial text on the text line.
  // As everything in the interface is static, this can even be done
  // before the interface itself is initialized.
  textLine.setText("Hello World");

  // Now init the interface including layouting and initial rendering
  displayManager.init(&u8g2);

}

/* Loop function: Nothing to be done here. */
void loop() {
  delay(1000);
}

// end of file
