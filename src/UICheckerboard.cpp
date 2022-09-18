// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UISize.h"
#include "UIWidget.h"
#include "UICheckerboard.h"


/** Create a checkerboard widget with the given preferred size and a potential successor. */
UICheckerboard::UICheckerboard(UISize preferredSize,UIWidget* next):
UIWidget(next),preferredSize(preferredSize) {}

/** Create a checkerboard widget with the given successor. */
UICheckerboard::UICheckerboard(UIWidget* next): UICheckerboard(UISize(),next) {}

/** Render the checkerboard pattern. */
UIArea* UICheckerboard::render(U8G2* display,bool force) {
  if (force) {
    clip(display);
    display->setDrawColor(1);
    for (uint16_t y=dim.top;y<dim.bottom;y+=16)
      for (uint16_t x=dim.left;x<dim.right;x+=16)
        display->drawXBM(x,y,16,16,checkerboard_bits);
    return &dim;
  }
  else
    return &UIArea::EMPTY;
}

/** Generate the preferred size from the font size. */
void UICheckerboard::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  preferredSize->set(&(this->preferredSize));
}

unsigned char UICheckerboard::checkerboard_bits[] = {
   0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
   0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
   0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55 };


// end of file
