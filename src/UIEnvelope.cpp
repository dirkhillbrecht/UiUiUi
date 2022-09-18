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
#include "UIEnvelope.h"


UIEnvelope::UIEnvelope(UIExpansion expansion,UIAlignment alignment,UISize borderSize,UIWidget* content,UIWidget* next):
  UIWidget(next), expansion(expansion), alignment(alignment), borderSize(borderSize), content(content) {
    if (content)
      content->setParent(this);
  }

UIEnvelope::UIEnvelope(UIExpansion expansion,UIAlignment alignment,UIWidget* content,UIWidget* next):
  UIEnvelope(expansion,alignment,UISize(),content,next) {}

void UIEnvelope::layout(U8G2 *display,UIArea *dim) {
  UIWidget::layout(display,dim);
  if (content) {
    UIArea contentDim=UIArea(dim);
    contentDim.shrink(&borderSize);
    UISize contentSize=contentDim.getSize();
    UISize preferredContentSize=content->preferredSize(display);
    if (preferredContentSize.width<contentSize.width || preferredContentSize.height<contentSize.height) {
      preferredContentSize.shrinkTo(&contentSize);
      UIPoint topLeft=contentDim.alignedTopLeft(alignment,&preferredContentSize);
      contentDim.set(&topLeft,&preferredContentSize);
    }
    content->layout(display,&contentDim);
  }
}

void UIEnvelope::childNeedsRendering(UIWidget *child) {
  signalNeedsRendering(); // just pass signal to parent
}

UIArea* UIEnvelope::render(U8G2 *display,bool force) {
  if (force) {
    clearFull(display);
    if (content)
      content->render(display,true);
    return &dim;
  }
  else
    return (content?content->render(display,false):&UIArea::EMPTY);
}

void UIEnvelope::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  if (content) {
    UISize contentSize=content->preferredSize(display);
    preferredSize->set(&contentSize);
  }
  if (expansion==UIExpansion::Horizontal || expansion==UIExpansion::Both)
    preferredSize->width=UISize::MAX_LEN;
  if (expansion==UIExpansion::Vertical || expansion==UIExpansion::Both)
    preferredSize->height=UISize::MAX_LEN;
  preferredSize->cumulateBoth(&borderSize);
}

// end of file
