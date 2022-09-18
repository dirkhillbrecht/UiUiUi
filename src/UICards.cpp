// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

// Basic helper classes and definitions
#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"

// Simple widgets
#include "UIWidget.h"
#include "UIWidgetGroup.h"
#include "UICards.h"


UICards::UICards(UIWidget* firstChild,UIWidget* next):
  UIWidgetGroup(firstChild,next),visible(nullptr) {}

void UICards::layout(U8G2* display,UIArea* dim) {
  UIWidgetGroup::layout(display,dim);
  UIWidget* widget=firstChild;
  while (widget) {
    widget->layout(display,dim);
    widget=widget->next;
  }
}

void UICards::setVisibleWidget(UIWidget *visible) {
  if (this->visible!=visible) {
    this->visible=visible;
    this->forceInternally=true;
    signalNeedsRendering();
  }
}

void UICards::hide() {
  setVisibleWidget(nullptr);
}

void UICards::showFirstWidget() {
  setVisibleWidget(firstChild);
}

UIArea* UICards::render(U8G2 *display,bool force) {
  if (this->forceInternally) {
    clip(display);
    clearFull(display);
    if (visible)
      visible->render(display,true);
    this->forceInternally=false;
    this->renderChildren=false;
    return &dim;
  }
  else if (force || this->renderChildren) {
    UIArea *retval=(visible?visible->render(display,force):&UIArea::EMPTY);
    this->renderChildren=false;
    return retval;
  }
  else
    return &UIArea::EMPTY;
}

void UICards::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  UIWidget* elem=firstChild;
  while (elem) {
    UISize otherSize=elem->preferredSize(display);
    preferredSize->maxBoth(&otherSize);
    elem=elem->next;
  }
}

void UICards::childNeedsRendering(UIWidget *child) {
  if (child==visible)
    UIWidgetGroup::childNeedsRendering(child);
}

// end of file
