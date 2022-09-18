// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UISize.h"
#include "UIWidget.h"


/** A widget drawing a checkerboard into its space. */
class UICheckerboard : public UIWidget {

  public:

    /** Create a checkerboard widget with the given preferred size and a potential successor. */
    UICheckerboard(UISize preferredSize=UISize(),UIWidget* next=nullptr);

    /** Create a checkerboard widget with the given successor. */
    UICheckerboard(UIWidget* next);

    /** Render the checkerboard pattern. */
    UIArea* render(U8G2* display,bool force);

  protected:

    /** Generate the preferred size from the font size. */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** The bit pattern to use for creating the checkerboard pattern. */
    static unsigned char checkerboard_bits[];

    /** Size of the currently rendered text. */
    UISize preferredSize;

};

// end of file
