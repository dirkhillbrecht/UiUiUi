// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"


/** Horizontal line with optional empty border on top and bottom, always spreads as wide as possible to the left and right. */
class UIHorizontalLine : public UIWidget {

  public:

    /** Create a horizontal line.
     *
     * Defaults to an empty border on top and bottom (default 0),
     * a tiny line width (defaults to 1) and an optional successor. */
    UIHorizontalLine(uint16_t border=0,uint16_t lineWidth=1,UIWidget* next=nullptr);

    /** Create a horizontal line of width 1 with no border on top and bottom. */
    UIHorizontalLine(UIWidget* next);

    /** A line is only rendered if rendering is forced, otherwise it never changes. */
    UIArea* render(U8G2 *display,bool force);

  protected:

    /** The preferred size of a line is 1+2*border in height and "as wide as possible" */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** Width of the border. */
    uint16_t border;

    /** Width of the line. */
    uint16_t lineWidth;

};

// end of file
