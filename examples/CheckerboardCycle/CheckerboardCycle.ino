// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// Restricted tiles update example: How screen updates can be split over several render() calls.

// This example shows how different settings of "first update tiles" and "follow update tiles" change the display update scheme.

// The example program draws a checkerboard over the whole display. It uses different settings of first and follow update
// tiles and shows these settings in a status line at the bottom of the screen.

// Note that this program uses a classic loop/delay approach. You can, of course, also use the tiles settings in an event-based
// sketch structure and that will be the usual way of doing things. I use this classic synchronous approach for the sake
// of brevity and determinism.

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

// As usual, we define the UI widget from bottom to top.

// Status line showing first and follow setting
UITextLine statusLine=UITextLine(u8g2_font_6x10_tf);

// Horizontal line between status line and checkerboard area.
UIHorizontalLine hLine=UIHorizontalLine(1,1,&statusLine);

// Checkerboard getting all the remaining space on the display
UICheckerboard checkerboard=UICheckerboard(UISize(UISize::MAX_LEN,UISize::MAX_LEN));

// A UICards is used to quickly make the checkerboard visible or invisible
UICards checkerboardCard=UICards(&checkerboard,&hLine);

// Row group for the widgets
UIRows fullScreen=UIRows(&checkerboardCard);

// Display representation taking the row group as display-filling child.
UIDisplay displayManager=UIDisplay(&fullScreen);


// The maximum update tiles. first and follow intermingled
uint16_t tiles[]={0xffff,0xffff, 3,3, 4,8, 8,16, 0,7, 0,16, 32,32, 32,64, 64,64, 0,0xffff};

// Count of tiles derived from the array definition as "standard C trick"
int tilesLen=sizeof(tiles)/sizeof(tiles[0]);

// Index count
int tilesIdx=0;

// Space in the data segment for the content of the status line
char statusText[30];

// Now set it up.
void setup() {
  Serial.begin(115200);  // For debugging purposes only, not used by sketch otherwise

  // Initialize the display
  u8g2.begin();  // Init U8g2, this is done outside of UiUiUi
  displayManager.init(&u8g2,true,false);  // Initialize the display manager but do not perform the initial rendering step

  // Initialize status line and checkerboard
  statusText[0]='\0';
  statusLine.setText(statusText);
  checkerboardCard.hide();
  displayManager.render(&u8g2,true); // Render only now after the widgets contents have been initialized
}

// Each run of loop() draws the checkerboard with a different setting of first and follow update tiles.
void loop() {

  // If we have reached the end of the tiles amount array, we start again at the beginning
  if (tilesIdx>=tilesLen)
    tilesIdx=0;

  // First render step: Clear checkerboard and write current tiles settings in the status line
  displayManager.setUpdateTiles(0xffff,0xffff);  // Clear all update tiles restrictions
  statusText[0]='\0';  // Clear status line;
  statusLine.updateText();  // Signal the status line widget that its content has changed
  displayManager.render(&u8g2);  // Render the cleared status line
  
  checkerboardCard.hide();  // Now clear the rest and write the new content in the status line
  uint16_t firstUpdate=tiles[tilesIdx];
  uint16_t followUpdate=tiles[tilesIdx+1];
  char itoaTarget[6];  // space for the integer-to-string conversion for the tiles count
  strcpy(statusText,"1st: ");  // construct the status line using C standard library functions
  strcat(statusText,(firstUpdate==0xffff?"max.":itoa(firstUpdate,itoaTarget,10)));
  strcat(statusText,", rest: ");
  strcat(statusText,(followUpdate==0xffff?"max.":itoa(followUpdate,itoaTarget,10)));
  statusLine.updateText();  // Signal the status line widget that its content has changed
  displayManager.render(&u8g2);  // Render the cleared checkerboard pattern together with the status line for the upcoming rendering step
  delay(500);  // Wait some time

  // Second step: Actually demonstrate the update tiles limit on the display
  displayManager.setUpdateTiles(firstUpdate,followUpdate);  // Set first and follow tiles update limit
  checkerboardCard.showFirstWidget();  // Re-visiblize the checkerboard
  displayManager.render(&u8g2);  // Render the stuff
  while (displayManager.isUpdatingDisplay()) {  // ...and while UiUiUi has not finished updating the display...
    delay(300);  // Wait some time to make clear what happens
    displayManager.render(&u8g2);  // Call render() again to continue the display update
  }
  delay(3000); // When everything is drawn, wait some time
  tilesIdx+=2; // Move to next pair of first and follow update limits
} // Continue in next loop() call

// end of file
