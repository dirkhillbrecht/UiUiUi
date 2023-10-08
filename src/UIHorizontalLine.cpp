// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIHorizontalLine.h"


/* Create a horizontal line with an empty border on top and bottom (defaults to 0),
 * a line width (defaults to 1) and an optional successor. */
UIHorizontalLine::UIHorizontalLine(uint16_t border,uint16_t lineWidth,UIWidget* next):
  UIWidget(next),border(border) {}

/* Create a horizontal line of width 1 with no border on top and bottom. */
UIHorizontalLine::UIHorizontalLine(UIWidget* next): UIHorizontalLine(0,1,next) {}

/* A line is only rendered if rendering is forced, otherwise it never changes. */
UIArea* UIHorizontalLine::render(U8G2 *display,bool force) {
  if (force) {
    clip(display);
    clearFull(display);
    display->setDrawColor(1);
    uint16_t y=(dim.top+dim.bottom)/2; // cannot overflow due to MAX_LEN
    display->drawLine(dim.left,y,dim.right,y);
    return &dim;
  }
  else
    return &UIArea::EMPTY;
}

/* The preferred size of a line is 1+2*border in height and "as wide as possible" */
void UIHorizontalLine::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  preferredSize->set(0,2*border+1);
}

// end of file
