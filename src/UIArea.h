// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"

/** Area consisting of left, top, right, and bottom value.
 *
 * Areas describe a rectangular - well - area.
 * Left and top coordinate are usually assumed to be inclusive,
 * right and bottom coordinate are assumed exclusive to the described area.
 *
 * An area is empty if right is not larger than left OR bottom is not larger than top.
 * The actual coordinate values do not matter.
 */
class UIArea final {
  public:

    /** Static incarnation of an empty area.
     *
     * This area instance has all four values set to 0.
     */
    static UIArea EMPTY;

    /** Initialize area from four explicit values. */
    UIArea(uint16_t left,uint16_t top,uint16_t right,uint16_t bottom);

    /** Initialize empty area: All values 0. */
    UIArea();

    /** Initialize area from another referenced area. */
    UIArea(UIArea* area);

    /** Initialize area with top left point and a size */
    UIArea(UIPoint *topLeft,UISize *size);

    /** Initialize area with top left point and a size */
    UIArea(UIPoint *topLeft,uint16_t width,uint16_t height);

    /** Set values of this area from four distinct values. */
    void set(uint16_t left,uint16_t top,uint16_t right,uint16_t bottom);

    /** Set values of this area from given other referenced area. */
    void set(const UIArea *source);

    /** Set this area from the top-left point and a size containing width and height */
    void set(UIPoint *topLeft,UISize *size);

    /** Set this area from the top-left point and a width and a height given directly. */
    void set(UIPoint *topLeft,uint16_t width,uint16_t height);

    /** sets this area to (0,0,0,0) and make it "clear" this way. */
    void clear();

    /** Return whether the area has any area: right is greater then left AND bottom is greater than top. */
    bool hasArea();

    /** Unite this area with the referenced one and write the result into this area.
     *
     * The united area is the smallest possible enclosure of both original areas.
     * It can be relatively large, however, if the two originals are not connected.
     * The unification of (1,15,4,17) (size 6) and (19,2,22,3) (size 3) is (1,2,22,17) (size 315).
     *
     * @param area The area to unite with.
     */
    void uniteWith(UIArea* area);

    /** Intersect this with the other area and store the result here.
     *
     * This method performs also an overflow check:
     * If left is larger than right, it is set to dim->left,
     * if top is larger than bottom it is set to dim->top.
     * This can happen if an underflow occurs
     * and the value which would be smaller than 0 flips to some 65500 values.
     *
     * @param area Other area with which this area is intersected.
     */
    void intersectWith(UIArea* area);

    /** Shrink this area by the given width and height on each side (so diff is applied twice in each direction). */
    void shrink(UISize *diff);

    /** Return the top-left point of an area of the given element if that element is aligned as stated in the alignment. */
    UIPoint alignedTopLeft(UIAlignment alignment,UISize *element);

    /** Return the size of this area. */
    UISize getSize();

    /** Debug output of this area with some prepended label. */
    void debugPrint(const char* label);

    /** Left coordinate of the area, 0 is leftmost, inclusive. */
    uint16_t left;

    /** Top coordinate of the area, 0 is topmost, inclusive. */
    uint16_t top;

    /** Right coordinate of the area, 0 is leftmost, exclusive. */
    uint16_t right;

    /** Bottom coordinate of the area, 0 is topmost, exclusive. */
    uint16_t bottom;

};

// end of file
