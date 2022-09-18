// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIWidgetGroup.h"


/** Widget group for widgets organized in columns one next to the other.
 *
 * The sub-widgets are layouted one next to the other from left to right
 * in the area the UIColumns widgets group gets from the higher layout level.
 *
 * The preferred size is the sum of the preferred widths of all sub-widgets
 * and the maximum of the heights of the sub-widgets.
 *
 * Sub-widgets can request either a fixed width or "as wide as possible" by
 * returning UISize::MAX_LEN as preferred width.
 * In this case in the layout step the remainder of the assigned widget widths
 * and all fixed widths of subwidgets
 * is split evenly between all "as large as possible" widths.
 */
class UIColumns: public UIWidgetGroup {

  public:

    /** Create a group of column-wise arranged sub-widgets */
    UIColumns(UIWidget* firstChild,UIWidget* next=nullptr);

    /** Layout all the sub-widgets one next to the other. */
    void layout(U8G2* display,UIArea* dim);

  protected:

    /** Preferred size of a columns group is computed by attaching the sub-widgets appropriately. */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** Number of sub-widgets which want to be "as high as possible" */
    uint8_t extendingChildrenCount;

    /** Sum of all fixed height preferences (not "as high as possible") of the sub-widgets. */
    uint16_t fixedWidth;

};

// end of file
