// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

/** \mainpage UiUiUi - A User interface library for micro controllers.
 *
 * UiUiUi is a library for user interfaces.
 * It is aimed at micro controller platforms like Arduino, ESP32 or ESP8266.
 * It uses U8g2 as foundation for accessing the display hardware
 * so it runs on a vast number of displays.
 *
 * You incorporate UiUiUi into your project by one of two alternative ways:
 *
 * 1. Include `UiUiUi.h` to access the whole library and all widget types.
 *    Use this if you design a complex interface and if compile time is not of that imporance.
 * 2. In smaller projects, you can simply include the header files of the classes
 *    you need directly. It makes no difference and all subsequently needed files
 *    will be loaded internally anyway.
 *
 * For more documentation, read through the widgets or visit the <a href="https://github.com/dirkhillbrecht/UiUiUi">project page on Github</a>.
 */

// Basic helper classes and definitions
#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"

// Simple widgets
#include "UIWidget.h"
#include "UITextLine.h"
#include "UITextIcon.h"
#include "UIHorizontalLine.h"
#include "UIVerticalLine.h"
#include "UICheckerboard.h"
#include "UIEnvelope.h"
#include "UIProgressBar.h"

// Widget groups
#include "UIWidgetGroup.h"
#include "UIRows.h"
#include "UIColumns.h"
#include "UICards.h"

// Topmost class
#include "UIDisplay.h"
