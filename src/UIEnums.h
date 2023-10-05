// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

// enum definitions for the UiUiUi library

#pragma once

/** Alignment of a widget within another. */
enum UIAlignment { TopLeft,TopCenter,TopRight,CenterLeft,Center,CenterRight,BottomLeft,BottomCenter,BottomRight };

/** Expansion directions if there is more space on the display than preferred. */
enum UIExpansion { None,Horizontal,Vertical,Both };

/** Direction of a widget if it can be oriented in multiple ways. Compatible with U8G2.setFontDirection. */
enum UIDirection { LeftToRight, TopToDown, RightToLeft, DownToTop };

// end of file
