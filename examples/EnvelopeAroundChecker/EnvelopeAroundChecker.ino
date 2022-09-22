// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// Show how an envelope can be placed around a widget

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

// The UI is a frame around the whole display, then a border of 10 pixels
// on the left and right and 5 pixels on top and bottom and then a
// checkerboard.

// This is a static example. It just defines this interface and draws it.
// After that, nothing more happens. If you want to have an example
// with some interaction, use EnvelopedCheckerboardAndButtons
// where you can enable or disable the presentation of the checkerboard.

// *** Define the UI ***

// As usual, we define the UI widget from bottom to top.

// We draw the outer frame out of four lines. This is the bottom one.
UIHorizontalLine bottomLine=UIHorizontalLine();

// ...and here is the right one. Note that these lines are
// totally unrelated, they will be connected via the groups later on.
UIVerticalLine rightLine=UIVerticalLine();

// Now comes the checkerboard which likes to get as large as possible.
UICheckerboard checkerboard=UICheckerboard(UISize(UISize::MAX_LEN,UISize::MAX_LEN));
// Note: This should work, too. But for me in my ESP32 development environment,
// this line triggers a linker error.
//UICheckerboard checkerboard=UICheckerboard(UISize::MAX_SIZE);

// It gets an envelope around it where it is placed in the center
UIEnvelope checkerEnv=UIEnvelope(UIExpansion::Both,UIAlignment::Center,UISize(10,5),&checkerboard,&rightLine);

// The lieft line of the outer border
UIVerticalLine leftLine=UIVerticalLine(&checkerEnv);

// The inner group: Connecting left line, enveloped checkerboard and right line
UIColumns centerWidgets=UIColumns(&leftLine,&bottomLine);

// The top line, the last line still missing
UIHorizontalLine topLine=UIHorizontalLine(&centerWidgets);

// The outer group: Connecting top line, inner group and bottom line
UIRows fullScreen=UIRows(&topLine);

// Display representation taking the row group as display-filling child.
UIDisplay displayManager=UIDisplay(&fullScreen);


// Now set it up.
void setup() {
  Serial.begin(115200);  // For debugging purposes only, not used by sketch otherwise

  // Initialize the display
  u8g2.begin();  // Init U8g2, this is done outside of UiUiUi
  displayManager.init(&u8g2);  // Initialize the display manager
  
}

// Each run of loop() draws the checkerboard with a different setting of first and follow update tiles.
void loop() {
  delay(1000);
}

// end of file
