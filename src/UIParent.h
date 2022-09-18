// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "UIWidget.h"

/** Simple abstraction of elements which have at least one child.
 *
 * This pure virtual class is mainly implemented by UIWidgetGroup,
 * as that is the class which contains other widgets as children.
 * But as UIDisplay also has a child, it is implemented there, too.
 */
class UIParent {

  public:

    /** Called by any child of this group to indicate that it wants to be rendered. */
    virtual void childNeedsRendering(UIWidget *child)=0;

};

// end of file
