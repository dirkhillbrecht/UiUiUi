// SPDX-License-Identifier: BSD-2-Clause
// (C) 2023 Andrew Burks

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"
#include "UIWidget.h"


/** A widget containing a progress bar which is set dynamically during runtime using setText().
 *
 * Height is derived from the assigned font. Width is "as wide as possible".
 * The widget keeps track of its former rendered content and will clear only
 * the portion of its area which has to be cleaned.
 *
 * A text line can align itself according to the Alignment possibilities.
 * You do not need to enclose it in a UIEnvelope.
 */
class UIProgressBar : public UIWidget {

  public:

    /** Create a progress bar oriented in the given direction, and a potential successor. */
    UIProgressBar(uint16_t thickness, uint16_t length, UIDirection direction,UIWidget* next=nullptr);

    /** Set the value of the progress bar.
     *
	 * A value of zero completely removes the bar, and a value of max_uint16 completely fills the bar.
     * This method can be called dynamically at any time.
     * It's the heart of the progress bar (and of the dynamic behaviour of UiUiUi generally...)
     */
    void setValue(uint16_t value);

    /** Clear the bar value back to zero. Acts as setValue(0). */
    void clearValue();

    /** Render the progress bar.
     *
     * This method is carefully designed so that it only draws and changes those parts of
     * the display which actually have to be changed. */
    UIArea* render(U8G2* display,bool force);

  protected:

    /** Generate the preferred size from the input parameters. */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** The value represented by the progress bar */
    uint16_t value;
	
	/** The desired size of the bar in the invariant direction */
    uint16_t thickness;
	
	/** The desired size of the bar in the variant direction when the value is at the maximum */
    uint16_t length;

    /** Direction that the bar grows in */
    UIDirection direction;

    /** Top left point of where the progress bar to render. */
    UIPoint topLeft;

    /** Flag whether the value has changed compared to the last rendered value. */
    bool changed;

    /** Filled area of progress bar during previous render. */
    UIArea lastFilledArea;
	
	/** Unfilled area of progress bar during previous render. */
    UIArea lastUnfilledArea;

    /** Filled area of progress bar during current render. */
    UIArea thisFilledArea;
	
	/** Unfilled area of progress bar during current render. */
    UIArea thisUnfilledArea;

	/** Determines the area of the filled rectangle representing the progress. */
	void computeFilledAreas(UIArea * filledArea, UIArea * unfilledArea);

};

// end of file
