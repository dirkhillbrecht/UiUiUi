// SPDX-License-Identifier: BSD-2-Clause
// (C) 2023 Andrew Burks

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"
#include "UIWidget.h"
#include "UIProgressBar.h"

/* Create a progress bar oriented in the given direction, and a potential successor. */
UIProgressBar::UIProgressBar(uint16_t thickness, uint16_t length, UIDirection direction,UIWidget* next):
  UIWidget(next),thickness(thickness),length(length),direction(direction),changed(false),value(0) {}

void UIProgressBar::setValue(uint16_t value) {
  // Only do something if the new value differs from the current one.
  if (this->value!=value) {
    this->value=value;
	changed=true;
	lastFilledArea.set(&thisFilledArea);  // Remember the last filled area
	lastUnfilledArea.set(&thisUnfilledArea);// Remember the last unfilled area
    signalNeedsRendering();
  }
}

void UIProgressBar::clearValue() {
  setValue(0);
}


UIArea* UIProgressBar::render(U8G2* display,bool force) {
  // Only do something if a change has occured or rendering is forced
  //   AND there is actually an area to render to.
  if ((changed || force) && dim.hasArea()) {
    changed=false;  // As we render now, there is no change to what is rendered
    clip(display);  // Clip every drawing to the frame buffer to this component's area.
    if (lastFilledArea.hasArea())
      clearBox(display,&lastFilledArea); // Clear the area of the last rendered content of this text line
    if (value>0) { // If there is a value to be drawn
      display->setDrawColor(1); // Setup painting
      display->setBitmapMode(1);
	  computeFilledAreas(&thisFilledArea,&thisUnfilledArea);
	  
	  // Draw the rectangle that represents filled area of progress bar
      display->drawBox(thisFilledArea.left,thisFilledArea.top,thisFilledArea.right-thisFilledArea.left,thisFilledArea.bottom-thisFilledArea.top);  
      
	  //calculate intersection of filled and unfilled areas from before/after render to find changed pixels
	  thisUnfilledArea.intersectWith(&dim);
	  lastUnfilledArea.intersectWith(&thisFilledArea);//pixels that have been added.
	  thisFilledArea.intersectWith(&dim);
	  lastFilledArea.intersectWith(&thisUnfilledArea);//pixels that have been removed.
	  
      lastFilledArea.uniteWith(&lastUnfilledArea); // One is probably empty, but this covers all of the bases
    }
    return &lastFilledArea;
  }
  else // No need to render: Return empty area
    return &UIArea::EMPTY;
} // render()

void UIProgressBar::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  if(direction == UIDirection::LeftToRight || direction == UIDirection::RightToLeft){
	  preferredSize->set(length,thickness);
  } else {
	  preferredSize->set(thickness,length);
  }
}

void UIProgressBar::computeFilledAreas(UIArea * filledArea, UIArea * unfilledArea)
{
    UISize widgetSize = dim.getSize();
	if(widgetSize.isEmpty()) // protect from divide by sero error below
	{
		filledArea->set(&UIArea::EMPTY);
		unfilledArea->set(&dim);
		return;
	}
	
	//compute number if bits per pixel of the bar
	uint16_t barResolution = 0xFFFF;
	if(direction == UIDirection::LeftToRight || direction == UIDirection::RightToLeft){
	  barResolution /= widgetSize.width;
    } else {
	  barResolution /= widgetSize.height;
	}
	//compute pixel length of bar. Adjust up by 1/2 pixel so that the function rounds instead of truncates
	uint16_t barLength = (value + barResolution/2)/barResolution;
	
	switch(direction)
	{
	case UIDirection::LeftToRight:
	  filledArea->set(dim.left,dim.top,dim.left+barLength,dim.bottom);
	  unfilledArea->set(dim.left+barLength,dim.top,dim.right,dim.bottom);
	  break;
    case UIDirection::TopToDown:
	  filledArea->set(dim.left,dim.top,dim.right,dim.top+barLength);
	  unfilledArea->set(dim.left,dim.top+barLength,dim.right,dim.bottom);
	  break;
    case UIDirection::RightToLeft:
	  filledArea->set(dim.right-barLength,dim.top,dim.right,dim.bottom);
	  unfilledArea->set(dim.left,dim.top,dim.right-barLength,dim.bottom);
	  break;
    case UIDirection::DownToTop:
	  filledArea->set(dim.left,dim.bottom-barLength,dim.left,dim.bottom);
	  unfilledArea->set(dim.left,dim.top,dim.right,dim.bottom-barLength);
	  break;
    default:
	  filledArea->set(&UIArea::EMPTY);
	  unfilledArea->set(&dim);
    }
}

// end of file
