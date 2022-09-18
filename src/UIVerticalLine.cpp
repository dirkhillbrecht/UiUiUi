// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIVerticalLine.h"

/* Create a vertical line with an empty border on left and right (defaults to 0),
 * a line width (defaults to 1) and an optional successor. */
UIVerticalLine::UIVerticalLine(uint16_t border,uint16_t lineWidth,UIWidget* next):
  UIWidget(next),border(border) {}

/* Create a vertical line of width 1 with no border on left and right. */
UIVerticalLine::UIVerticalLine(UIWidget* next): UIVerticalLine(0,1,next) {}

/* A line is only rendered if rendering is forced, otherwise it never changes. */
UIArea* UIVerticalLine::render(U8G2 *display,bool force) {
  if (force) {
    clip(display);
    clearFull(display);
    display->setDrawColor(1);
    uint16_t x=(dim.left+dim.right)/2; // cannot overflow due to MAX_LEN
    display->drawLine(x,dim.top,x,dim.bottom);
    return &dim;
  }
  else
    return &UIArea::EMPTY;
}

/* The preferred size of a line is 1+2*border in width and "as high as possible" */
void UIVerticalLine::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  preferredSize->set(2*border+1,UISize::MAX_LEN);
}

// end of file
