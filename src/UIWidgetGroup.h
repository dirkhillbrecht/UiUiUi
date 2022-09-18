// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIParent.h"

/** Main abstraction of a group of interface widgets.
 *
 * A widget group organizes a number of widgets in a certain way.
 * These organized widgets form a linked list via their "next" pointers.
 * The widget group gets the first of these widgets as their "first widget".
 *
 * As a widget group itself is also a widget, it has itself potentially a successor
 * which allows forming another widget group on a "higher level".
 *
 * Widget groups are the key to create complex interfaces with UiUiUi.
 * Be sure to understand the concept behind them.
 *
 * As with the UIWidget class, UIWidgetGroup itself only implements a tiny portion
 * of the business logic. The derived classes actually create a semantic onto the
 * grouped widgets, e.g. aligning them horizontally or vertically.
 */
class UIWidgetGroup: public UIWidget, public UIParent {

  public:

    /** Create a widget group with a linked list of sub-widgets and a potential successor. */
    UIWidgetGroup(UIWidget* firstChild,UIWidget* next=nullptr);

    /** Default implementation of rendering a widget group is to render all of its sub-widgets. */
    UIArea* render(U8G2* display,bool force);

    /** Called by any child of this group to indicate that it wants to be rendered. */
    virtual void childNeedsRendering(UIWidget *child);

  protected:

    /** The first of the widgets grouped by this widget group. */
    UIWidget* firstChild;

    /** Flag whether any child of this group needs to be rendered. */
    bool renderChildren;
    // Note: This is protected as UICards handles this on its own.
    // With a better abstraction, this could be made private.

  private:

    /** Space where the rendering information of all sub-widgets is collected. */
    UIArea rendered;

};

// end of file
