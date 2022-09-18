// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#include "Arduino.h"
#include "UISize.h"

/** Representation of a size, i.e. a width and a height. */

const uint16_t UISize::MAX_LEN=32767;  // Half of max value so that addition of two does not overflow

const UISize MAX_SIZE=UISize(UISize::MAX_LEN,UISize::MAX_LEN);

const UISize EMPTY=UISize();

UISize::UISize(uint16_t width,uint16_t height): width(width), height(height) {}

UISize::UISize(UISize *other): UISize(other->width,other->height) {}

UISize::UISize(): UISize(0,0) {}

void UISize::set(uint16_t width,uint16_t height) { this->width=width; this->height=height; }

void UISize::set(UISize *other) { set(other->width,other->height); }

void UISize::cumulateBoth(UISize *other) {
  width=max_len_or(width+other->width);
  height=max_len_or(height+other->height);
}

void UISize::maxWidthCumulateHeight(UISize *other) {
  width=my_max(width,other->width);
  height=max_len_or(height+other->height);
}

void UISize::cumulateWidthMaxHeight(UISize *other) {
  width=max_len_or(width+other->width);
  height=my_max(height,other->height);
}

void UISize::maxBoth(UISize *other) {
  width=my_max(width,other->width);
  height=my_max(height,other->height);
}

void UISize::shrinkTo(UISize *other) {
  if (other->width<width)
    width=other->width;
  if (other->height<height)
    height=other->height;
}

bool UISize::isEmpty() {
  return width==0 && height==0;
}

/* Static helper method: maximum of two given uint16_t fields */
uint16_t UISize::my_max(uint16_t a,uint16_t b) {
  return (a>b?a:b);
}

/* Static helper method: maximum of two given uint16_t fields */
uint16_t UISize::my_min(uint16_t a,uint16_t b) {
  return (a<b?a:b);
}

/* Static helper method: restrict given value to MAX_LEN at most. */
uint16_t UISize::max_len_or(uint16_t a) {
  return my_min(MAX_LEN,a);
}

void UISize::debugPrint(const char* label) {
  Serial.print(label);Serial.print("(");
  Serial.print(width);Serial.print(",");
  Serial.print(height);Serial.println(")");
}

// end of file
