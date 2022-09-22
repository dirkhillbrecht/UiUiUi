// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// Enable and disable a checkerboard area by pressing two buttons

// The sketch needs two buttons and an LED connected to some GPIO pins.
// Pressing one button turns the LED on, the other button turns if off again.
// The user interface on the display switches a checkerboard on and off
// together with the LED.

// This example works asynchronously: In setup(), the initialisation is done and the initial events are fired up.
// These events fire either regulary or register a follow-up event.
// In loop(), only the handler of the event manager is called.

// You might want to have a look at the EnvelopedCheckerboardAndButtons
// example program which uses the same program logic but has a slightly
// more sophisticated display (well, it has a border around the checkerboard...).


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

// Now, the UI is defined. This also happens statically and globally.
// We have a very simple UI here: Apart from the display manager, it consists of only one element

// Line which shows the time, will render itself centered on its space
UICheckerboard checkerboard=UICheckerboard();

// UICards to hide or show the checkerboard
UICards checkerboardCard=UICards(&checkerboard);

// As the time line is the only UI element, just put it into the display representation.
// It will get the complete space and render itself centered in both directions.
UIDisplay displayManager=UIDisplay(&checkerboardCard);


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
