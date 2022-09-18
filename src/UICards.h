// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

// Basic helper classes and definitions
#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"

// Simple widgets
#include "UIWidget.h"
#include "UIWidgetGroup.h"


/** Widget group which shows nothing or exactly one of its sub-widgets
 *
 * UICards takes its widgets and shows exactly one of them covering the whole area.
 * This is like a card stack (therefore the name).
 * The widget to show is selected by setVisibleWidget().
 *
 * If the visible widget is not set or set to null, nothing is shown.
 *
 * Note that you can use UICards easily to make another widget visible or invisible:
 * Just add the widget in question as first (and only) sub-widget to UICards.
 * Then, if you make it the visible widget, it is shown, and if you set the visible widget
 * to null, it is hidden.
 *
 * The two methods `hide()` and `showFirstWidget()` act as syntactic sugar for this operation mode.
 */
class UICards: public UIWidgetGroup {

  public:

    /** Initialize the cards widget with the first widget and a potential successor on the same level. */
    UICards(UIWidget* firstChild,UIWidget* next=nullptr);

    /** Layout the cards widget. */
    void layout(U8G2* display,UIArea* dim);

    /** Set the widget to show on the area of the cards widget group.
     *
     * If nullptr is given, nothing is shown.
     * The parameter *must* be one of the sub-widgets of the group.
     * If another widget is passed, the results are undefined.
     *
     * Changing the visible widget leads to the cards widget group being completely re-rendered.
     */
    void setVisibleWidget(UIWidget *visible);

    /** Shortcut: Hide all widgets, clear the UICards area on the display. */
    void hide();

    /** Shortcut: Show first widget. */
    void showFirstWidget();

    /** Render the cards widget group onto the display. */
    UIArea* render(U8G2 *display,bool force);

    /** UICards will consider the render request from the child only if it is the currently visible child. */
    void childNeedsRendering(UIWidget *child);

  protected:

    /** Compute the preferred size as the maximum envelope of the preferred sizes of all subwidgets. */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** Pointer to the visible widget. */
    UIWidget *visible;

    /** Flag whether redraw has to take place. */
    bool forceInternally;

};

// end of file
