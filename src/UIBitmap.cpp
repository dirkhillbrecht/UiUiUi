// SPDX-License-Identifier: BSD-2-Clause
// (C) 2023 Andrew Burks

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"
#include "UIWidget.h"
#include "UIBitmap.h"
  
/* Create a bitmap with the given width, height, XBMP, alignment, and potential successor*/
UIBitmap::UIBitmap(uint16_t width, uint16_t height, const uint8_t *bitmap, UIAlignment alignment, UIWidget* next):
  UIWidget(next),imageSize(width,height),bitmap(bitmap),alignment(alignment) {}

/* Create a bitmap with the given width, height, XBMP, and potential successor*/
UIBitmap::UIBitmap(uint16_t width, uint16_t height, const uint8_t *bitmap, UIWidget* next):
  UIBitmap(width,height,bitmap,UIAlignment::Center,next) {}
  
UIArea* UIBitmap::render(U8G2* display,bool force) {
  // Only render image if forced
  if (force && dim.hasArea()) {
    clip(display);  // Clip every drawing to the frame buffer to this component's area.
    clearBox(display,&dim); // Clear the entire widget area, even unused buffer/border
	display->setDrawColor(1); // Setup painting
    display->setBitmapMode(1);
	UISize actualSize = dim.getSize();//get the available screen size and check if it is big enough for the bitmap
	if(actualSize.width >= imageSize.width && actualSize.height >= imageSize.height){
	  topLeft.setFrom(dim.alignedTopLeft(alignment,&imageSize));
      display->drawXBMP(topLeft.x, topLeft.y, imageSize.width, imageSize.height, bitmap);//draw the bitmap
	} else { //just draw a box with an X through it if the supplied area is not big enough
	  display->drawFrame(dim.left, dim.top, actualSize.width, actualSize.height);
	  display->drawLine(dim.left, dim.top, dim.right-1, dim.bottom-1);
	  display->drawLine(dim.left, dim.bottom-1, dim.right-1, dim.top);
	}
    return &dim;//return the entire widget area because the background was cleared
  }
  else // No need to render: Return empty area
    return &UIArea::EMPTY;
} // render()

void UIBitmap::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  preferredSize->set(&imageSize);
}

// end of file
