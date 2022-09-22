// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// This example shows a slightly more complex interface which is handled in asynchronous tasks using the TaskManagerIO library.

// The sketch needs two buttons and an LED connected to some GPIO pins.
// Pressing one button turns the LED on, the other button turns if off again.
// The user interface on the display consists of three elements dubbing the action:
//
// +-----------------------------+
// |                             |
// |             (A)             |  <- label "Off" or "On" dubbing the LED's current state
// |                             |
// | --------------------------- |  <- horizontal line
// |      (B)           (C)      |  <- One indicator per button: "x": Pressed, "-": Released
// +-----------------------------+
//
// The sketch works asynchronously, i.e. testing the buttons and drawing the interface
// happens in tasks scheduled periodically by the task manager.

// We start including the needed headers
#include <TaskManagerIO.h>
#include <U8g2lib.h>
#include <UiUiUi.h>

// Then we define the display, put in your U8g2 display here
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// Now we define the interface.
// Rule of thumb: The tree structure represents the interface from top left to bottom right.
// But as everything has to be defined before it's used, you define it in the opposite direction:
// You start with the last element bottom right and "climb the tree up".

// At the bottom right is the indicator for button 2. We start with defining it.
UITextLine button2Indicator=UITextLine(u8g2_font_6x10_tf);

// Then comes the indicator for button 1. It forms a group with button 2's indicator,
// so that one is referenced as following widget in the constructor call.
UITextLine button1indicator=UITextLine(u8g2_font_6x10_tf,&button2Indicator);

// For the two indicators really to form a group, a widget group must be defined.
// We need the indicators next to each other, so we use the column-arranging widget group
UIColumns buttonIndicatorGroup=UIColumns(&button1indicator);

// Above the button indicators, we place a horizontal line.
// Note that it takes the button indicator group as successor.
UIHorizontalLine horizontalLine=UIHorizontalLine(&buttonIndicatorGroup);

// Now comes the LED indicator.
UITextLine ledIndicator=UITextLine(u8g2_font_crox3cb_tf);

// We want the LED indicator to take all of the remaining place on the display.
// So we put it into an expanding envelope.
// Note how this envelope takes the LED indicator as content (third parameter)
// and the horizontal line as successor (fourth parameter).
UIEnvelope ledIndicatorEnvelope=UIEnvelope(UIExpansion::Both,UIAlignment::Center,&ledIndicator,&horizontalLine);

// Now we have almost everything together.
// The three elements of the full screen (LED indicator, horizontal line, button indicator group)
// are stacked on top of each other, so we use the row-arranging widget group here.
UIRows displayGroup=UIRows(&ledIndicatorEnvelope);

// And finally we can define the manager for the full display and give it the main group as content.
UIDisplay displayManager=UIDisplay(&displayGroup);

// ...and we're done!

// Define the pins where the buttons and the LED are connected.
// All devices are connected against ground. Don't forget the resistor for the LED (some 220 ohms).
#define B1 17
#define B2 18
#define LED 2

// Variables to represent the current state and the displayed state for each element
bool ledCurrentState,ledDisplayedState,button1CurrentState,button1DisplayedState,button2CurrentState,button2DisplayedState;

// Task ids for the button check task and the rendering task
int buttonTaskId,displayTaskId;

// The main business logic.
// It checks the buttons, turns the LED on and off accordingly and updates the display content
// Note that it DOES NOT render the interface. That's done in a seperate thread controlled by the task manager.
void checkButtons() {

  // Read the buttons and update the _current_ state of buttons and LED
  if (digitalRead(B1)==LOW) {
    ledCurrentState=true;
    button1CurrentState=true;
  }
  else {
    button1CurrentState=false;
    if (digitalRead(B2)==LOW) {
      ledCurrentState=false;
      button2CurrentState=true;
    }
    else
      button2CurrentState=false;
  }

  // If LED state has changed, switch LED and update LED indicator on the screen.
  if (ledCurrentState!=ledDisplayedState) {    
    digitalWrite(LED,(ledCurrentState?HIGH:LOW));
    ledIndicator.setText(ledCurrentState?"On":"Off");
    ledDisplayedState=ledCurrentState;
  }

  // If one of the buttons states have changed, update display accordingly
  if (button1CurrentState!=button1DisplayedState) {
    button1indicator.setText(button1CurrentState?"x":"-");
    button1DisplayedState=button1CurrentState;
  }
  if (button2CurrentState!=button2DisplayedState) {
    button2Indicator.setText(button2CurrentState?"x":"-");
    button2DisplayedState=button2CurrentState;
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
  button1CurrentState=false;button1DisplayedState=true;
  button2CurrentState=false;button2DisplayedState=true;

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
