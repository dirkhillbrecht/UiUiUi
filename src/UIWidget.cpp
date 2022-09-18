// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UISize.h"
#include "UIArea.h"
#include "UIWidget.h"
#include "UIParent.h"


UIWidget::UIWidget(UIWidget *next): dim(),thePreferredSize(),next(next),parent(nullptr) {}

UISize UIWidget::preferredSize(U8G2* display) {
  if (thePreferredSize.isEmpty())
    computePreferredSize(display,&thePreferredSize);
  return thePreferredSize;
}

void UIWidget::layout(U8G2* display,UIArea* dim) {
  this->dim.set(dim);
}

void UIWidget::clearBox(U8G2* display,UIArea *dimm) {
  display->setDrawColor(0);
  display->drawBox(dimm->left,dimm->top,(dimm->right-dimm->left),(dimm->bottom-dimm->top));
}

void UIWidget::clearFull(U8G2 *display) {
  clearBox(display,&dim);
}

void UIWidget::clip(U8G2 *display) {
  display->setClipWindow(dim.left,dim.top,dim.right,dim.bottom);
}

void UIWidget::setParent(UIParent *parent) {
  this->parent=parent;
}

void UIWidget::signalNeedsRendering() {
  if (parent)
    parent->childNeedsRendering(this);
}

// end of file
