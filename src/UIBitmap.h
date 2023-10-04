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


/** A widget containing a bitmap (XBMP) which is set statically during compile time.
 *
 * Height and width must be supplied to match the XBMP data.
 * Alignment can be specified for cases where the bitmap is rendered in a larger area.
 */
class UIBitmap : public UIWidget {

  public:
	
	/** Create a bitmap with supplied alignment and potential successor.
	 * 
	 * The supplied width and height must match the bitmap data.
	 * The bitmap data must be XBMP formated, and compiled into PROGMEM.  */
    UIBitmap(uint16_t width, uint16_t height, const uint8_t *bitmap, UIAlignment alignment, UIWidget* next = nullptr);

    /** Create a bitmap with default centered alignment and potential successor.
	 * 
	 * The supplied width and height must match the bitmap data.
	 * The bitmap data must be XBMP formated, and compiled into PROGMEM.  */
    UIBitmap(uint16_t width, uint16_t height, const uint8_t *bitmap, UIWidget* next = nullptr);

    /** A bitmap is only rendered if the rendering is forced, otherwise it never changes. */
    UIArea* render(U8G2* display,bool force);

  protected:

    /** Generate the preferred size from the bitmap size. */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** The image to render, as a pointer to XBMP data in PROGMEM. */
    const uint8_t* bitmap;

    /** Size of the currently rendered text. */
    UISize imageSize;

    /** Alignment of the image in the widget's area */
    UIAlignment alignment;

    /** Top left point of where the text to render. */
    UIPoint topLeft;


};

// end of file
