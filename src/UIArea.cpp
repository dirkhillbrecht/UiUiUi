// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#include "Arduino.h"
#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"

/** Dimension consisting of left, top, right, and bottom value. */
UIArea::UIArea(uint16_t left,uint16_t top,uint16_t right,uint16_t bottom):
  left(left),top(top),right(right),bottom(bottom) {}

UIArea::UIArea(): UIArea(0,0,0,0) {}

UIArea::UIArea(UIArea* dim):
  left(dim->left),top(dim->top),right(dim->right),bottom(dim->bottom) {}

UIArea::UIArea(UIPoint *topLeft,UISize *size):
  left(topLeft->x),top(topLeft->y),right(topLeft->x+size->width),bottom(topLeft->y+size->height) {}

UIArea::UIArea(UIPoint *topLeft,uint16_t width,uint16_t height):
  left(topLeft->x),top(topLeft->y),right(topLeft->x+width),bottom(topLeft->y+height) {}

void UIArea::set(uint16_t left,uint16_t top,uint16_t right,uint16_t bottom) {
  this->left=left;this->top=top;this->right=right;this->bottom=bottom;
}

void UIArea::set(const UIArea *source) {
  this->left=source->left;this->top=source->top;
  this->right=source->right;this->bottom=source->bottom;
}

void UIArea::set(UIPoint *topLeft,UISize *size) {
  left=topLeft->x;top=topLeft->y;
  right=left+size->width; bottom=top+size->height;
}

void UIArea::set(UIPoint *topLeft,uint16_t width,uint16_t height) {
  left=topLeft->x;top=topLeft->y;
  right=left+width; bottom=top+height;
}

void UIArea::clear() {
  left=0;right=0;top=0;bottom=0;
}

bool UIArea::hasArea() {
  return (right>left && bottom>top);
}

void UIArea::uniteWith(UIArea* area) {
  if (area->hasArea()) {
    if (this->hasArea()) {
      set(min(this->left,area->left),
          min(this->top,area->top),
          max(this->right,area->right),
          max(this->bottom,area->bottom));
    } else {
      this->set(area);
    }
  }
}

void UIArea::intersectWith(UIArea* area) {
  if (area->hasArea()) {
    this->left=(this->left>this->right ? area->left : max(this->left,area->left));
    this->right=min(this->right,area->right);
    this->top=(this->top>this->bottom ? area->top : max(this->top,area->top));
    this->bottom=max(this->bottom,area->bottom);
  }
  else
    this->clear();
}

void UIArea::shrink(UISize *diff) {
  left+=diff->width;
  right=max(left,right>diff->width?(uint16_t)(right-diff->width):(uint16_t)0);
  top+=diff->height;
  bottom=max(top,bottom>diff->height?(uint16_t)(bottom-diff->height):(uint16_t)0);
}

UIPoint UIArea::alignedTopLeft(UIAlignment alignment,UISize *element) {
  uint16_t x=(alignment==UIAlignment::TopLeft || alignment==UIAlignment::CenterLeft || alignment==UIAlignment::BottomLeft)?left:
  (alignment==UIAlignment::TopCenter || alignment==UIAlignment::Center || alignment==UIAlignment::BottomCenter)?((right+left)-element->width)/2:
  right-element->width;
  uint16_t y=(alignment==UIAlignment::TopLeft || alignment==UIAlignment::TopCenter || alignment==UIAlignment::TopRight)?top:
  (alignment==UIAlignment::CenterLeft || alignment==UIAlignment::Center || alignment==UIAlignment::CenterRight)?((bottom+top)-element->height)/2:
  bottom-element->height;
  return UIPoint(x,y);
}

UISize UIArea::getSize() {
  return UISize(right-left,bottom-top);
}

void UIArea::debugPrint(const char* label) {
  Serial.print(label);Serial.print("(");
  Serial.print(left);Serial.print(",");
  Serial.print(top);Serial.print(",");
  Serial.print(right);Serial.print(",");
  Serial.print(bottom);Serial.println(")");
}

/* Empty area: All coordinates are 0. */
UIArea UIArea::EMPTY=UIArea();

// end of file
