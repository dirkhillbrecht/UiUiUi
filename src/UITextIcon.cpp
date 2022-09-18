// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UISize.h"
#include "UIWidget.h"
#include "UITextLine.h"
#include "UITextIcon.h"

UITextIcon::UITextIcon(const uint8_t* font,UISize preferredSize,UIWidget* next):
  UITextLine(font,UIAlignment::Center,next),myPreferredSize(preferredSize) {}

void UITextIcon::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  UITextLine::computePreferredSize(display,preferredSize); // to initialize textSize
  preferredSize->set(&myPreferredSize);
}

// end of file
