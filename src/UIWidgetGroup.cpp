// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIWidgetGroup.h"

/* Create a widget group with a linked list of sub-widgets and a potential successor. */
UIWidgetGroup::UIWidgetGroup(UIWidget* firstChild,UIWidget* next):
UIWidget(next), firstChild(firstChild), rendered(), renderChildren(false) {
  UIWidget* elem=firstChild;
  while (elem) {
    elem->setParent(this);
    elem=elem->next;
  }
}

/* Default implementation of rendering a widget group is to render all of its sub-widgets. */
UIArea* UIWidgetGroup::render(U8G2* display,bool force) {
  rendered.set(0,0,0,0);
  if (force || renderChildren) {
    UIWidget* elem=firstChild;
    while (elem) {
      rendered.uniteWith(elem->render(display,force));
      elem=elem->next;
    }
    renderChildren=false;
  }
  return &rendered;
}

/** Called by any child of this group to indicate that it wants to be rendered. */
void UIWidgetGroup::childNeedsRendering(UIWidget *child) {
  if (!renderChildren) {
    renderChildren=true;
    signalNeedsRendering();
  }
}

// end of file
