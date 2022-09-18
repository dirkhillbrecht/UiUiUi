// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIWidgetGroup.h"
#include "UIColumns.h"


/* Create a group of column-wise arranged sub-widgets */
UIColumns::UIColumns(UIWidget* firstChild,UIWidget* next):
  UIWidgetGroup(firstChild,next) {}

/* Layout all the sub-widgets one next to the other. */
void UIColumns::layout(U8G2* display,UIArea* dim) {
  UIWidgetGroup::layout(display,dim);
  UIArea stamp=UIArea(dim);
  stamp.right=stamp.left;
  int extendingWidth=(extendingChildrenCount==0 || fixedWidth>=(dim->right-dim->left)?0:((dim->right-dim->left)-fixedWidth)/extendingChildrenCount);
  UIWidget* widget=firstChild;
  while (widget && stamp.right<dim->right) {
    int requestedWidth=widget->preferredSize(display).width;
    stamp.right=min(dim->right,(uint16_t)(stamp.right+(requestedWidth==UISize::MAX_LEN?extendingWidth:requestedWidth)));
    widget->layout(display,&stamp);
    widget=widget->next;
    stamp.left=stamp.right;
  }
}

/* Preferred size of a columns group is computed by attaching the sub-widgets appropriately. */
void UIColumns::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  UIWidget* elem=firstChild;
  while (elem) {
    UISize otherDim=elem->preferredSize(display);
    if (otherDim.width==UISize::MAX_LEN)
      extendingChildrenCount+=1;
    else
      fixedWidth+=otherDim.width;
    preferredSize->cumulateWidthMaxHeight(&otherDim);
    elem=elem->next;
  }
}

// end of file
