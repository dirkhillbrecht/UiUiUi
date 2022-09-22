// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// Complex example of UiUiUi use: A mock weather station with bluetooth and WLAN connectivity.
// This example does not need any additional hardware (apart from the display).
//
//   +----------------------------------+
//   +                                  +
//   +                          |       +
//   +            (A)           |  (B)  +
//   +                          |       +
//   +                          |       +
//   + -------------------------------- +
//   +  (C)                             +
//   +----------------------------------+

// The screen has three parts: The main part (A) shows weather data and is updated every four seconds (with mock data).
// It has two different presentation modes: Tiny font, more lines and large font, less lines which also change periodically.
// Additionally, on the right (B) two icons present Bluetooth and WLAN connectivity.
// In this example, both links disconnect after a few seconds and will reconnect then (of course, this is also mocked).
// A status line (C) at the bottom informs about these inconveniences.
// The three parts of the screen are separated by a horizontal and a vertical line.

// This example uses a task manager and event for the different actions.
// Actually, a sketch this complex can hardly be implemented using the setup/loop/delay paradigm.

// The usual imports
#include <TaskManagerIO.h>
#include <U8g2lib.h>
#include <UiUiUi.h>

// The U8g2 display, replace this with the one for your hardware.
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// *** Here comes the definition of the interface ***

// This is a rather complex definition.
// But it follows the basic UiUiUi rule: Designed from tiop left to bottom right, declared the other way around.
// So we start with the components on the right on at the bottom.

// **** Bottom: Separator and status line ****

// We start with the status line at the very bottom
UITextLine statusTextLine = UITextLine(u8g2_font_6x10_tf, UIAlignment::CenterLeft);

// Then comes the horizontal line above the status line.
UIHorizontalLine horizontalLine = UIHorizontalLine(&statusTextLine);

// The element above the horizontal line is a complex widget consisting of a number of other widgets. We start at the...

// **** Right: Connectivity icons

// The lower icon is the bluetooth connectivity indicator. We start with that one.
UITextIcon btIcon = UITextIcon(u8g2_font_streamline_interface_essential_circle_triangle_t, UISize(23, 23));

// At the top, the WLAN connectivity icon is placed. It gets the bluetooth icon as its successor.
UITextIcon wlanIcon = UITextIcon(u8g2_font_streamline_interface_essential_wifi_t, UISize(23, 23), &btIcon);

// Both icons are placed one above the other. A rows group does the job.
UIRows iconsGroup = UIRows(&wlanIcon);

// Finally, the rows should be placed in the center of the available area.
// For this, an envelope is put around expanding the requested vertical space.
UIEnvelope iconsEnvelope = UIEnvelope(UIExpansion::Vertical, UIAlignment::Center, &iconsGroup);

// That's it with the icons. We can define the vertical line left to it (with its successor being the just-defined envelope).
UIVerticalLine verticalLine = UIVerticalLine(&iconsEnvelope);

// **** Main screen ****

// The main screen has two alternatively shown sub screens: One with the tiny lines, one with the larger lines.

// We start with the larger lines. Lower line...
UITextLine largeTextLine2 = UITextLine(u8g2_font_crox3cb_tf);
// ...upper line...
UITextLine largeTextLine1 = UITextLine(u8g2_font_crox3cb_tf, true, &largeTextLine2);
// ...and rows group to bundle them.
UIRows largeTextScreen = UIRows(&largeTextLine1);

// Now the second sub screen with three lines. Again, bottommost first...
UITextLine smallTextLine3 = UITextLine(u8g2_font_6x10_tf, UIAlignment::CenterLeft);
// ...then the one in the middle...
UITextLine smallTextLine2 = UITextLine(u8g2_font_6x10_tf, UIAlignment::CenterLeft, &smallTextLine3);
// ...and finally the line at the top. Note that these lines are aligned to the left (and not centered as the larger ones).
UITextLine smallTextLine1 = UITextLine(u8g2_font_6x10_tf, UIAlignment::CenterLeft, true, &smallTextLine2);
// Another row group for these lines
UIRows smallTextScreen = UIRows(&smallTextLine1, &largeTextScreen);

// Now comes the cards group which allows to show either of the two defined main screens.
UICards mainScreens = UICards(&smallTextScreen);

// And finally we surround this with another envelope which grows in both directions so that the main screen gets all remaining space on the display
// Note that this envelope has the vertical line as successor so that the upper interface parts are glued together.
UIEnvelope mainEnvelope = UIEnvelope(UIExpansion::Both, UIAlignment::Center, &mainScreens, &verticalLine);

// **** Top level ***

// We do now group the main screen, the vertical line, and the icons group columnwise with the appropriate group.
// Note that this group gets the horizontal line (which we defined many lines above) as successor to prepare the next larger widget group.
UIColumns topColumns = UIColumns(&mainEnvelope, &horizontalLine);

// Upper part, horizontal line and status line are arranged by another rows group.
UIRows fullScreen = UIRows(&topColumns);

// And this group is then passed to the display manager to be rendered on the display.
UIDisplay displayManager = UIDisplay(&fullScreen);

// *** End of menu definition ***

// The important part of the main sketch for this example is how the display is updated.

// The sketch holds a number of flags describing the current state of the interface elements and the displayed state of the elements
bool btCurr, btDisp;  // bluetooth current state and displayed state
bool wlanCurr, wlanDisp;  // WLAN current state and displayed state
int screenCurr, screenDisp;  // main screen current state and displayed state

// Actual update method: Writes data to the screen widgets and requests an update.
void updateDisplay() {

  // For the status line, both bluetooth and WLAN state are evaluated
  if (btCurr != btDisp || wlanCurr != wlanDisp)
    statusTextLine.setText(btCurr ? (wlanCurr ? "" : "Connecting WLAN") : (wlanCurr ? "Conn. Bluetooth" : "Conn. WLAN & BT"));

  // Then the bluetooth icon is set or removed if the bluetooth state has changed
  if (btCurr != btDisp) {
    btIcon.setText(btCurr ? "5" : "");  // "5" is the bluetooth icon in the selected font, see U8g2 font reference
    btDisp = btCurr; // Only now the display state is set to the current state
  }

  // Same thing with the WLAN icon
  if (wlanCurr != wlanDisp) {
    wlanIcon.setText(wlanCurr ? "0" : "");  // Again, "0" is a WLAN icon in the selected font.
    wlanDisp = wlanCurr;
  }

  // Finally the main screen.
  // This is a giant mock, of course. A real implementation would set the information independently
  if (screenCurr != screenDisp) {
    // We switch the complete screen as it is needed
    switch (screenCurr) {
      case 0: mainScreens.setVisibleWidget(&smallTextScreen);
        smallTextLine1.setText("Temp.:    Check");
        smallTextLine2.setText("Humidity: Check");
        smallTextLine3.setText("Wind:     Check");
        break;
      case 1: mainScreens.setVisibleWidget(&smallTextScreen);
        smallTextLine1.setText("Temp.:    27 °C");
        smallTextLine2.setText("Humidity: 75 %");
        smallTextLine3.setText("Wind:      3 Bft");
        break;
      case 2: mainScreens.setVisibleWidget(&largeTextScreen);
        largeTextLine1.setText("27 °C");
        largeTextLine2.setText("75 %");
        break;
      case 3: mainScreens.setVisibleWidget(&largeTextScreen);
        largeTextLine1.setText("3 Bft");
        largeTextLine2.setText("");
        break;
    }
    screenDisp = screenCurr;
  }

  // Finally, updateDisplay() calls displayManager.render() ALWAYS.
  // It is important that this call is unconditionally as there might have been other changes
  // to the display and the render process must therefore be run even if nothing has changed here.
  displayManager.render(&u8g2);
}

// The rest of the sketch is mockup code which simulates that something is happening in the weather station.

// Four tasks are registered with the task manager.
int renderTaskId, btTaskId, wlanTaskId, screenTaskId;

// The bluetooth connection will break down after 8 to 12 seconds and will be restored after 4 to 6 seconds
void toggleBT() {
  btCurr = !btCurr;
  btTaskId = taskManager.scheduleOnce((btCurr ? random(8000, 12000) : random(4000, 6000)), [] {
    toggleBT();
  });
}

// The WLAN connection will break down after 10 to 15 seconds and will be restored after 5 to 7 seconds
void toggleWLAN() {
  wlanCurr = !wlanCurr;
  wlanTaskId = taskManager.scheduleOnce((btCurr ? random(10000, 15000) : random(5000, 7000)), [] {
    toggleWLAN();
  });
}

// The main screen is changed periodically and iterates through all 4 possible presentations.
void switchScreen() {
  screenCurr = (screenCurr + 1) % 4;
}

// Bring it all together
void setup() {
  Serial.begin(115200); // Only for testing and debugging
  u8g2.begin();

  // Initializes the interface and performs initial rendering
  displayManager.init(&u8g2);

  // Initialize the internal state...
  btCurr = false; btDisp = true;
  wlanCurr = false; wlanDisp = true;
  screenCurr = 0; screenDisp = 1;
  // ...and perform initial rendering
  updateDisplay();

  // Start the bluetooth action (will be retriggered within the handler)
  btTaskId = taskManager.scheduleOnce(random(4000, 6000), [] {
    toggleBT();
  });

  // And start the WLAN action (will also be retriggered within the handler)
  wlanTaskId = taskManager.scheduleOnce(random(5000, 7000), [] {
    toggleWLAN();
  });

  // Switch the screen at a fixed rate of four seconds
  screenTaskId = taskManager.scheduleFixedRate(4000, [] {
    switchScreen();
  });

  // Update and render the display every 100 ms.
  // This will be very quick if nothing is to be done so it can be called often.
  renderTaskId = taskManager.scheduleFixedRate(100, [] {
    updateDisplay();
  });

}

// Main loop only calls task manager's handling loop
void loop() {
  taskManager.runLoop();
}

// end of file
