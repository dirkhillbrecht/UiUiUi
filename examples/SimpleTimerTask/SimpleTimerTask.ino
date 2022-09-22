// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// Simple example: One line which may show the times since reboot of the device, asynchronous version

// This example shows the most basic principles of UiUiUi:
//
// - Define the GUI in a really simple example
// - Initialize the GUI on screen
// - Update the contents of the GUI
// - Render the GUI on the display

// This example works asynchronously: In setup(), the initialisation is done and the initial events are fired up.
// These events fire either regulary or register a follow-up event.
// In loop(), only the handler of the event manager is called.


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
  hour=(minute/24)%24;
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

// Task ids for the asynchronous tasks (not needed in the example sketch but generally useful)
taskid_t timerTaskId,renderTaskId;

// Task to show the "Up since" text
// This task is only scheduled once from the setup() function.
void upSinceTask() {
  theTimeLine.setText("Up since");  // Set an initial text on the text line (8 chars max!)...
  timerTaskId = taskManager.scheduleOnce(2000, [] {
    initialTimeTask();
  });  
}

// Task to switch from "Up since" to the actual time presentation
// This task is only scheduled once from the upSinceTask() method
void initialTimeTask() {
  theTimeLine.setText(timeString);  // and change the time line text to the static time storage string.
  updateTimeTask();
}

// Task to update 
void updateTimeTask() {
  setTime();  // Set the time
  convertTime();  // Convert the int-based stored time into parts of the time string
  theTimeLine.updateText();  // Inform the text line that its content has changed (in situ)

  // Reschedule the task for repeated operation

  // First, cancel any task running under this timer task's id.
  // This prevents multiple schedules of updateTimeTask() if it is called for any reason from somewhere else.
  // This is recommended as the task reschedules itself via TaskManagerIO::scheduleOnce.
  // For tasks scheduled reguraly with TaskManagerIO::scheduleFixedRate, this is not needed (and most probably wrong).
  taskManager.cancelTask(timerTaskId);
  
  // Then, schedule a new run on updateTimeTask() some 10 milliseonds after the current second is over.
  timerTaskId = taskManager.scheduleOnce(1010-(millis()%1000), [] {
    updateTimeTask();
  });
}

/* Setup function: Called once after boot */
void setup() {
  u8g2.begin();  // Init U8g2, this is done outside of UiUiUi
  displayManager.init(&u8g2);  // Now init the interface including layouting

  // Install the UiUiUi rendering task, approximately every 200 milliseconds (i.e. 4 to 5 times per second)
  // This will update the display regulary. If nothing has to be changed, it will finish rather quickly (about 20 milliseconds).
  // Otherwise, the display is updated which will need some time depending on the hardware (up to 200-300 milliseconds for a SSD1306 I2C display)
  renderTaskId = taskManager.scheduleFixedRate(200, [] {
    displayManager.render(&u8g2);
  });

  // Somehow start the operation of the sketch by registering one or more tasks.
  // This sketch only issues one task once. If anything else should happen afterwards, it must be issues from within this task handler
  // (spoiler: it does...)
  timerTaskId = taskManager.scheduleOnce(10, [] {
    upSinceTask();
  });
}

/* Loop function: Just call the task manager to handle the tasks. */
void loop() {
  taskManager.runLoop();
}

// end of file
