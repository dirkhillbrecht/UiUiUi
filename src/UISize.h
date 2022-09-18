// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"

/** Representation of a size, i.e. a width and a height. */
class UISize final {
  public:
    /** Less than half of max value so that addition of two does not overflow */
    static const uint16_t MAX_LEN;

    /** UISize representing maximal size in all directions */
    static const UISize MAX_SIZE;

    /** UISize representing an empty size (both directions 0) */
    static const UISize EMPTY;

    /** Initialize a size from a width and a height. */
    UISize(uint16_t width,uint16_t height);

    /** Initialize a size from another size */
    UISize(UISize *other);

    /** Initialize a size without width or height (both 0). */
    UISize();

    /** Width */
    uint16_t width;

    /** Height */
    uint16_t height;

    /** Set height from a numeric width and height. */
    void set(uint16_t width,uint16_t height);

    /** Set size from another referenced height */
    void set(UISize *other);

    /** Cumulate width and height of this and the referenced size into this size. */
    void cumulateBoth(UISize *other);

    /** Cumulate the heights of this and the other size and take the maximum of both widths,
     * write everything into this. */
    void maxWidthCumulateHeight(UISize *other);

    /** Cumulate the widths of this and the other size and take the maximum of both heights,
     * write everything into this. */
    void cumulateWidthMaxHeight(UISize *other);

    /** Take the maximum of widths and height of this and the other size and write it into this. */
    void maxBoth(UISize *other);

    /** Shrink this so that neither width nor height is larger than the one of other. */
    void shrinkTo(UISize *other);

    /** Return true if this size is empty, i.e. it has neither width nor height, i.e. both are 0. */
    bool isEmpty();

    /** Static helper method: maximum of two given uint16_t fields */
    static uint16_t my_max(uint16_t a,uint16_t b);

    /** Static helper method: maximum of two given uint16_t fields */
    static uint16_t my_min(uint16_t a,uint16_t b);

    /** Static helper method: restrict given value to MAX_LEN at most. */
    static uint16_t max_len_or(uint16_t a);

    /** Debug output of this size with some prepended label. */
    void debugPrint(const char* label);

};

// end of file
