// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"

/** Representation of a point on the display. */
class UIPoint final {
  public:

    /** Initialize the point from two values. */
    UIPoint(uint16_t x,uint16_t y);

    /** Initialize the point as "0,0" */
    UIPoint();

    /** Set the point from another point. */
    void setFrom(UIPoint p);

    /** Debug output of this point with some prepended label. */
    void debugPrint(const char* label);

    /** X Coordinate of the point, normally 0 is top */
    uint16_t x;

    /** Y Coordinate of the point, normally 0 is left */
    uint16_t y;

};

// end of file
