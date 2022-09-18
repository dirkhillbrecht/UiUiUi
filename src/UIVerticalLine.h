// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"

/** Vertical line with optional empty border left and right, always spreads as wide as possible to top and bottom. */
class UIVerticalLine : public UIWidget {

  public:

    /** Create a vertical line.
     *
     * Defaults are set to an empty border on left and right (default 0),
     * a line width (defaults to 1) and an optional successor. */
    UIVerticalLine(uint16_t border=0,uint16_t lineWidth=1,UIWidget* next=nullptr);

    /** Create a vertical line of width 1 with no border on left and right. */
    UIVerticalLine(UIWidget* next);

    /** A line is only rendered if rendering is forced, otherwise it never changes. */
    UIArea* render(U8G2 *display,bool force);

  protected:

    /** The preferred size of a line is 1+2*border in width and "as high as possible" */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** Width of the border. */
    uint16_t border;

    /** Width of the line. */
    uint16_t lineWidth;

};

// end of file
