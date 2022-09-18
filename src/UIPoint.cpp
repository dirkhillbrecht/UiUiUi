// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#include "Arduino.h"
#include "UIPoint.h"

/* Initialize the point from two values. */
UIPoint::UIPoint(uint16_t x,uint16_t y): x(x),y(y) {}

/* Initialize the point as "0,0" */
UIPoint::UIPoint(): UIPoint(0,0) {}

/* Set the point from another point. */
void UIPoint::setFrom(UIPoint p) { x=p.x; y=p.y; }

/* Debug output of this point with some prepended label. */
void UIPoint::debugPrint(const char* label) {
  Serial.print(label);Serial.print("(");
  Serial.print(x);Serial.print(",");
  Serial.print(y);Serial.println(")");
}

// end of file
