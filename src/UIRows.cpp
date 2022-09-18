// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIWidgetGroup.h"
#include "UIRows.h"


/* Create a group of row-wise arranged sub-widgets */
UIRows::UIRows(UIWidget* firstChild,UIWidget* next):
  UIWidgetGroup(firstChild,next) {}

/* Layout all the sub-widgets one below the other. */
void UIRows::layout(U8G2* display,UIArea* dim) {
  UIWidgetGroup::layout(display,dim);  // Call inherited method.
  UIArea stamp=UIArea(dim);  // Use this stamp for the areas of the sub-widgets
  stamp.bottom=stamp.top;
  // extendingHeight is the height which the widgets get who want to be "as high as possible"
  int extendingHeight=(extendingChildrenCount==0 || fixedHeight>=(dim->bottom-dim->top)?0:((dim->bottom-dim->top)-fixedHeight)/extendingChildrenCount);
  UIWidget* widget=firstChild;
  while (widget && stamp.bottom<dim->bottom) { // Walk through the widgets
    int requestedHeight=widget->preferredSize(display).height;
    // Bottom is top plus height of the current widget
    stamp.bottom=min(dim->bottom,(uint16_t)(stamp.bottom+(requestedHeight==UISize::MAX_LEN?extendingHeight:requestedHeight)));
    widget->layout(display,&stamp);
    widget=widget->next;
    // For next widget, top will be current bottom
    stamp.top=stamp.bottom;
  }
}

/* Preferred size of a rows group is computed by stacking the sub-widgets appripriately. */
void UIRows::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  UIWidget* elem=firstChild;
  while (elem) {
    UISize otherDim=elem->preferredSize(display);
    if (otherDim.height==UISize::MAX_LEN)
      extendingChildrenCount+=1;
    else
      fixedHeight+=otherDim.height;
    preferredSize->maxWidthCumulateHeight(&otherDim);
    elem=elem->next;
  }
}

// end of file
