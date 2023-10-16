// SPDX-License-Identifier: BSD-2-Clause
// (C) 2023 Dirk Hillbrecht, Andrew Burks

// Demonstrate drawing of bitmaps
// This example defines a bitmap, creates four widgets from it and spreads them evenly over the display

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

// This is the bitmap data. Note that it must be placed in PROGMEM due to the underlying U8g2 functions.
// It is formatted as inline XBM image, see https://en.wikipedia.org/wiki/X_BitMap
#define smiley_width 20
#define smiley_height 20
static const unsigned char smiley_bits[] U8X8_PROGMEM = {
   0x80, 0x1f, 0x00, 0xe0, 0x7f, 0x00, 0x78, 0xe0, 0x01, 0x1c, 0x80, 0x03,
   0x0c, 0x00, 0x03, 0x06, 0x00, 0x06, 0x06, 0x00, 0x06, 0xc3, 0x30, 0x0c,
   0xc3, 0x30, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c,
   0x43, 0x20, 0x0c, 0xc6, 0x79, 0x06, 0x86, 0x1f, 0x06, 0x0c, 0x06, 0x03,
   0x1c, 0x80, 0x03, 0x78, 0xe0, 0x01, 0xe0, 0x7f, 0x00, 0x80, 0x1f, 0x00 };


// ** First subpanel **

// We define the four smiley bitmap widgets separatele but pointing to the same bitmap data.
UIBitmap smileyA1 = UIBitmap(smiley_width, smiley_height, smiley_bits);
UIBitmap smileyA2 = UIBitmap(smiley_width, smiley_height, smiley_bits);
UIBitmap smileyA3 = UIBitmap(smiley_width, smiley_height, smiley_bits);
UIBitmap smileyA4 = UIBitmap(smiley_width, smiley_height, smiley_bits);

// The bitmaps get envelopes which only add a one pixel border around them
UIEnvelope margA1 = UIEnvelope(UIExpansion::None, UIAlignment::Center, UISize(1,1), &smileyA1);
UIEnvelope margA2 = UIEnvelope(UIExpansion::None, UIAlignment::Center, UISize(1,1), &smileyA2, &margA1);
UIEnvelope margA3 = UIEnvelope(UIExpansion::None, UIAlignment::Center, UISize(1,1), &smileyA3);
UIEnvelope margA4 = UIEnvelope(UIExpansion::None, UIAlignment::Center, UISize(1,1), &smileyA4, &margA3);

// Two rows of bitmaps
UIRows rowsA1 = UIRows(&margA2);
UIRows rowsA2 = UIRows(&margA4,&rowsA1);

// ...combined in two columns
UIColumns colsA1 = UIColumns(&rowsA2);

// An envelope which centers the whole 2x2 block
UIEnvelope panelA = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(), &colsA1);

// *** Second subpanel

// Four other bitmaps
UIBitmap smileyB1 = UIBitmap(smiley_width, smiley_height, smiley_bits);
UIBitmap smileyB2 = UIBitmap(smiley_width, smiley_height, smiley_bits);
UIBitmap smileyB3 = UIBitmap(smiley_width, smiley_height, smiley_bits);
UIBitmap smileyB4 = UIBitmap(smiley_width, smiley_height, smiley_bits);

// The bitmaps get envelopes which expand in both directions and center the bitmap.
UIEnvelope margB1 = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(1,1), &smileyB1);
UIEnvelope margB2 = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(1,1), &smileyB2, &margB1);
UIEnvelope margB3 = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(1,1), &smileyB3);
UIEnvelope margB4 = UIEnvelope(UIExpansion::Both, UIAlignment::Center, UISize(1,1), &smileyB4, &margB3);

// Rows...
UIRows rowsB1 = UIRows(&margB2);
UIRows rowsB2 = UIRows(&margB4,&rowsB1);

// ...and columns as in the first subpanel. As the envelopes expand, no expanding envelope is needed here
UIColumns panelB = UIColumns(&rowsB2,&panelA);

// Main panel is a cards pannel with both subpanels.
UICards mainpanel = UICards(&panelB);

// Put main panel into display.
UIDisplay display = UIDisplay(&mainpanel);


// Now set it up.
void setup() {
  Serial.begin(115200);  // For debugging purposes only, not used by sketch otherwise

  // Initialize the display
  u8g2.begin();  // Init U8g2, this is done outside of UiUiUi
  display.init(&u8g2);  // Initialize the display manager  
}

// Perform indefinitely
void loop() {
  mainpanel.setVisibleWidget(&panelA);  // Make first subpanel visible
  display.render(&u8g2);  // Render screen
  delay(2000);  // Wait 2 seconds
  mainpanel.setVisibleWidget(&panelB);  // Make second subpanel visible
  display.render(&u8g2);  // Render screen
  delay(2000);  // Wait 2 seconds
}

// end of file
