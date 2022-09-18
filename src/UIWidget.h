// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UISize.h"
#include "UIArea.h"
//#include "UIParent.h"

// Forward reference
class UIParent;

/** Basic widget class, ancestor of all UI widgets.
 *
 * A UIWidget has three basic operations which are called by the framework in that order:
 *
 * 1. preferredSize() returns the size the widget wants to have on the screen.
 *    Any or all lengths may be Size::MAX_LEN in which case it gets what is available.
 * 2. layout() gets the actual coordinates in which the widget may render itself.
 *    They can be larger or smaller than the preferred size. The widget has to cope with it.
 * 3. render() asks the widget to actually draw itself into the frame buffer.
 *    It must restrict itself to the area it is assigned to.
 *    It returns the area where it actually has changed something.
 *    If no changes are needed, it returns the empty area.
 *
 * A UIWidget may have a successor which is the next widget in the interface ON THE SAME LEVEL.
 * Specialized UIWidgets bring certain semantics into such a chain of widgets.
 *
 * The UIWidget class itself has only a tiny amount of business logic. The majority of it
 * is spread through a number of derived classes representing all kinds of user interface elements.
 */
class UIWidget {
  public:

    /** Return the preferred size of this widget, will be called before a call to layout().
     *
     * This method delegates the operation into computePreferredSize()
     * if the preferred size has not yet been computed.
     */
    UISize preferredSize(U8G2* display);

    /** Layout widget in the given rectanglar area, will be called before first call to render().
     *
     * Note that overloading implementation should invoke UIWidget::layout(),
     * otherwise certain internal mechanisms will not work.
     */
    virtual void layout(U8G2* display,UIArea* area);

    /** Render component in the space given in layout().
     *
     * If force is true, the component must render itself in its rectangle.
     * Otherwise, it may refrain from doing anything if its internal state says that nothing has changed.
     * Component returns the area which _actually_ must be written to screen.
     * This area may be smaller than the whole area of the component.
     * If the component has not changed anything,
     * it must return an empty area, e.g. UIArea::EMPTY().
     */
    virtual UIArea* render(U8G2* display,bool force)=0;

    /** Pointer to the next widget on the same level. */
    UIWidget *next;

    /** Set the parent widget group (if it exists).
     *
     * This method is called by the widget group this widget is assigned to during initialisation.
     * It should not be called by anyone else.
     */
    virtual void setParent(UIParent *parent);

  protected:

    /** Initialize this widget and potentially set its successor. */
    UIWidget(UIWidget *next=nullptr);

    /** Actual area of this widget, set in layout(). */
    UIArea dim;

    /** Reference to the group this widget is connected to. */
    UIParent *parent;

    /** Clear the given area. */
    void clearBox(U8G2* display,UIArea *dimm);

    /** Clear the full area of this widget. */
    void clearFull(U8G2 *display);

    /** Set U8g2's clip window to the area of this widget, should be called from render(). */
    void clip(U8G2 *display);

    /** Store preferred size of this widget into the preferredSize reference.
     *
     * Implementations can be assured to be called only once and the passed preferredSize will be (0,0).
     */
    virtual void computePreferredSize(U8G2 *display,UISize *preferredSize)=0;

    /** Called internally: Signals to parent that this widget needs to be rendered. */
    void signalNeedsRendering();

  private:

    /** Preferred size of this widget, stored as it might be needed several time, e.g. in layout(). */
    UISize thePreferredSize;

};

// end of file
