// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIWidgetGroup.h"


/** Widget group for widgets organized in rows one below the other.
 *
 * The sub-widgets are layouted one below the other from top to bottom
 * in the area the UIRows widgets group gets from the higher layout level.
 *
 * The preferred size is the sum of the preferred heights of all sub-widgets
 * and the maximum of the widths of the sub-widgets.
 *
 * Sub-widgets can request either a fixed height or "as high as possible"
 * by returning UISize::MAX_LEN as preferred height.
 * In this case in the layout step the remainder of the assigned widget height
 * and all fixed heights of subwidgets
 * is split evenly between all "as large as possible" heights.
 */
class UIRows: public UIWidgetGroup {

  public:

    /** Create a group of row-wise arranged sub-widgets */
    UIRows(UIWidget* firstChild,UIWidget* next=nullptr);

    /** Layout all the sub-widgets one below the other. */
    void layout(U8G2* display,UIArea* dim);

  protected:

    /** Preferred size of a rows group is computed by stacking the sub-widgets appropriately. */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** Number of sub-widgets which want to be "as high as possible" */
    uint8_t extendingChildrenCount;

    /** Sum of all fixed height preferences (not "as high as possible") of the sub-widgets. */
    uint16_t fixedHeight;

};

// end of file
