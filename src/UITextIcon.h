// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UISize.h"
#include "UIWidget.h"
#include "UITextLine.h"

/** Widget which shows an icon by some normal text operation, usually with a font with icons.
 *
 * This is a specialized text line widget.
 * Its main difference is that its preferred size is not "as wide as possible", but
 * explicitly given in width and height.
 *
 * The actual icon to show is given via the inherited UITextLine::setText method.
 * Of course, clearText() works also.
 */
class UITextIcon : public UITextLine {

  public:

    /** Create a text icon with a specific font, a preferred size and a potential successor. */
    UITextIcon(const uint8_t* font,UISize preferredSize,UIWidget* next=nullptr);

  protected:

    /** The preferred size is completely determined by the size given at construction time.
     *
     * The text line's preferred size computation is called nevertheless,
     * so that the internal initialisation takes place.
     */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** The preferred size as given by the constructor. */
    UISize myPreferredSize;

};

// end of file
