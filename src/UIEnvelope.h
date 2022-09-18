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
#include "UIParent.h"


/** Envelope widget which contains exactly one other widget and changes its representation.
 *
 * The envelope can do the following things:
 *
 * * If the enclosed widget does not spawn the whole display area it is assigned,
 *   the envelope can place it with a certain alignment.
 * * The envelope can add a border to the inner widget. This border will be empty.
 * * If the inner widget prefers to be smaller than it could be, the envelope can report
 *   a larger area to the caller.
 */
class UIEnvelope : public UIWidget, public UIParent {
  public:

    /** Create an envelope widget.
     *
     * @param expansion Report an enlarged preferred size to the caller for the axes given by this parameter.
     * @param alignment Align the content widget this way if it is smaller than the area of the envelope.
     * @param borderSize Generate an empty border of this size around the content widget
     * @param content The content of the envelope. If empty, the envelope renders an empty area.
     * @param next Next widget on the same level as the envelope.
     */
    UIEnvelope(UIExpansion expansion,UIAlignment alignment,UISize borderSize=UISize(),UIWidget* content=nullptr,UIWidget* next=nullptr);

    /** Create an envelope widget without border.
     *
     * @param expansion Report an enlarged preferred size to the caller for the axes given by this parameter.
     * @param alignment Align the content widget this way if it is smaller than the area of the envelope.
     * @param content The content of the envelope. If empty, the envelope renders an empty area.
     * @param next Next widget on the same level as the envelope.
     */
    UIEnvelope(UIExpansion expansion,UIAlignment alignment,UIWidget* content,UIWidget* next=nullptr);

    /** Layout the envelope.
     *
     * This includes shaping the content according to all the constraints.
     */
    void layout(U8G2 *display,UIArea *dim);

    /** Render the envelope - which also renders the content if it exists. */
    UIArea* render(U8G2 *display,bool force);

    /** Called by the child of this envelope to indicate that it wants to be rendered. */
    void childNeedsRendering(UIWidget *child);

  protected:

    /** Compute the preferred size which is determined by the content by may be changed by the envelope. */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);

  private:

    /** Expansion strategy during computation of preferred size. */
    UIExpansion expansion;

    /** Alignment of inner content widget. */
    UIAlignment alignment;

    /** Border size of the envelope. */
    UISize borderSize;

    /** Reference to the content widget of the envelope. */
    UIWidget* content;

};

// end of file
