// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"
#include "UIWidget.h"


/** A widget containing a line of text which is set dynamically during runtime using setText().
 *
 * Height is derived from the assigned font. Width is "as wide as possible".
 * The text line keeps track of its former rendered content and will clear only
 * the portion of its area which has to be cleaned.
 *
 * A text line can align itself according to the Alignment possibilities.
 * You do not need to enclose it in a UIEnvelope.
 */
class UITextLine : public UIWidget {

  public:

    /** Create a text line with the given font and alignment and a potential successor. */
    UITextLine(const uint8_t* font,UIAlignment alignment,bool useUTF8=false,UIWidget* next=nullptr);

    /** Create a text line with the given font and alignment, not using UTF8 and a potential successor. */
    UITextLine(const uint8_t* font,UIAlignment alignment,UIWidget* next);

    /** Create a text line with the given font, centered alignment, the given UTF8 flag and a potential successor. */
    UITextLine(const uint8_t* font,bool useUTF8,UIWidget* next=nullptr);

    /** Create a text line with the given font, centered alignment and a potential successor. */
    UITextLine(const uint8_t* font,UIWidget* next);

    /** Create a text line with the given font and no successor. */
    UITextLine(const uint8_t* font);

    /** Set the text of the text line.
     *
     * This method can be called dynamically at any time.
     * It's the heart of the text line (and of the dynamic behaviour of UiUiUi generally...)
     */
    void setText(const char* text);

    /** Clear the text in this text line. */
    void clearText();

    /** Insist that on the next render() call, the text line will update its content.
     *
     * Use this if you use a static text field which you update from outside
     * so that the setText() change detection does not work.
     *
     * You should use setText() only once and clearText() never in this setup.
     */
    void updateText();

    /** Render the text line.
     *
     * This method is carefully designed so that it only draws and changes those parts of
     * the display which actually have to be changed. */
    UIArea* render(U8G2* display,bool force);

  protected:

    /** The font of this text line */
    const uint8_t* font;

    /** Generate the preferred size from the font size. */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** The text to render */
    const char* text;

    /** Alignment of the text in the widget's area */
    UIAlignment alignment;

    /** Flag whether the text line uses UTF8 encoding for rendering */
    bool useUTF8;

    /** Size of the currently rendered text. */
    UISize textSize;

    /** Top left point of where the text to render. */
    UIPoint topLeft;

    /** Flag whether the text has changed compared to the last rendered text. */
    bool changed;

    /** Area of the last text which has been rendered by this text line. */
    UIArea lastArea;

    /** Area of the text which is currently rendered by this text line. */
    UIArea thisArea;

    /** Compute the text height and return it.
     *
     * NOTE: This method works with an internal side effect into the textSize field. */
    uint16_t getTextHeight(U8G2* display);

};

// end of file
