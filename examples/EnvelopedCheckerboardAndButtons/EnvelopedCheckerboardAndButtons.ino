// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// Enable and disable a checkerboard area by pressing two buttons

// The sketch needs two buttons and an LED connected to some GPIO pins.
// Pressing one button turns the LED on, the other button turns if off again.
// The user interface on the display switches a checkerboard on and off
// together with the LED. The checkerboard is put into a frame and
// a border around it.

// This example works asynchronously: In setup(), the initialisation is done and the initial events are fired up.
// These events fire either regulary or register a follow-up event.
// In loop(), only the handler of the event manager is called.

// You might want to have a look at the CheckerboardAndButtons
// example program which uses the same program logic but has a simpler
// display (no border around the checkerboard).


// **************************
// *** Include statements ***
// **************************

// Asynchronous operation requires a task manager library. We use TaskManagerIO, others work also 
#include <TaskManagerIO.h>

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

// UICards to hide or show the checkerboard
UICards checkerboardCard=UICards(&checkerboard);

// It gets an envelope around it where it is placed in the center
UIEnvelope checkerEnv=UIEnvelope(UIExpansion::Both,UIAlignment::Center,UISize(10,5),&checkerboardCard,&rightLine);

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


// Define the pins where the buttons and the LED are connected.
// All devices are connected against ground. Don't forget the resistor for the LED (some 220 ohms).
#define B1 17
#define B2 18
#define LED 2

// Variables to represent the current state and the displayed state for each element
bool ledCurrentState,ledDisplayedState;

// Task ids for the button check task and the rendering task
int buttonTaskId,displayTaskId;

// The main business logic.
// It checks the buttons, turns the LED on and off accordingly and updates the display content
// Note that it DOES NOT render the interface. That's done in a seperate thread controlled by the task manager.
void checkButtons() {

  // Read the buttons and update the _current_ state of buttons and LED
  if (digitalRead(B1)==LOW) {
    ledCurrentState=true;
  }
  else {
    if (digitalRead(B2)==LOW) {
      ledCurrentState=false;
    }
  }

  // If LED state has changed, switch LED and update LED indicator on the screen.
  if (ledCurrentState!=ledDisplayedState) {    
    digitalWrite(LED,(ledCurrentState?HIGH:LOW));
    if (ledCurrentState)
      checkerboardCard.showFirstWidget();
    else
      checkerboardCard.hide();
    ledDisplayedState=ledCurrentState;
  }
}

// Now set it up.
void setup() {
  Serial.begin(115200);  // For debugging purposes only, not used by sketch otherwise

  // Initialize the hardware
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(LED,OUTPUT);

  // Initialize the display
  u8g2.begin();  // Init U8g2, this is done outside of UiUiUi
  displayManager.init(&u8g2);  // Now init the interface including layouting

  // Initialize the internal state.
  // We initialize the display state different from the (assumed) current state
  // so that all indicators will update themselves on the first run of checkButtons().
  ledCurrentState=false;ledDisplayedState=true;

  // Finally start the tasks. First the rendering task
  displayTaskId = taskManager.scheduleFixedRate(80, [] {
    // Enable the lines below to get debug output on how fast the display updates are performed
//    unsigned long start=millis();
    displayManager.render(&u8g2);
//    Serial.print("render needed ");
//    Serial.print(millis()-start);
//    Serial.println(" ms");
  });

  // ...and the task for evaluating and updating the hardware state.
  buttonTaskId = taskManager.scheduleFixedRate(50, [] {
    checkButtons();
  });
}

// loop() only calls the task manager's loop function and does nothing more.
void loop() {
  taskManager.runLoop();
}

// end of file
