// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"
#include "UIWidget.h"
#include "UITextLine.h"

/* Create a text line with the given font, alignment, UTF8 flag and a potential successor. */
UITextLine::UITextLine(const uint8_t* font,UIAlignment alignment,bool useUTF8,UIWidget* next):
  UIWidget(next),font(font),text(""),alignment(alignment),useUTF8(useUTF8),changed(false) {}

/* Create a text line with the given font and alignment and a potential successor. */
UITextLine::UITextLine(const uint8_t* font,UIAlignment alignment,UIWidget* next):
  UITextLine(font,alignment,false,next) {}

/* Create a text line with the given font and alignment and a potential successor. */
UITextLine::UITextLine(const uint8_t* font,bool useUTF8,UIWidget* next):
  UITextLine(font,UIAlignment::Center,useUTF8,next) {}

/* Create a text line with the given font, centered alignment and a potential successor. */
UITextLine::UITextLine(const uint8_t* font,UIWidget* next):
  UITextLine(font,UIAlignment::Center,next) {}

/* Create a text line with the given font and no successor. */
UITextLine::UITextLine(const uint8_t* font):
  UITextLine(font,UIAlignment::Center,nullptr) {}

void UITextLine::setText(const char* text) {
  // Only do something if the new text differs from the current one.
  if (this->text!=text) {
    if (strcmp(this->text,text) != 0)
      this->updateText();
    this->text=text;
  }
}

void UITextLine::clearText() {
  setText("");
}

void UITextLine::updateText() {
  lastArea.set(&thisArea);  // Remember the last (rendered) content area
  textSize.width=0;
  changed=true;
  signalNeedsRendering();
}

UIArea* UITextLine::render(U8G2* display,bool force) {
  // Only do something if a change has occured or rendering is forced
  //   AND there is actually an area to render to.
  if ((changed || force) && dim.hasArea()) {
    changed=false;  // As we render now, there is no change to what is rendered
    clip(display);  // Clip every drawing to the frame buffer to this component's area.
    if (lastArea.hasArea())
      clearBox(display,&lastArea); // Clear the area of the last rendered content of this text line
    if (sizeof(text)>0) { // If there is a text to be drawn
      display->setFontPosTop(); // Setup everything for text painting
      display->setFont(font);
      display->setFontDirection(0);
      if (textSize.width==0) {  // Text width has not yet been computed
        textSize.width=(useUTF8?display->getUTF8Width(text):display->getStrWidth(text));
        topLeft.setFrom(dim.alignedTopLeft(alignment,&textSize));
      }
      display->setDrawColor(1); // Setup painting
      display->setBitmapMode(1);
      if (useUTF8)
        display->drawUTF8(topLeft.x,topLeft.y,text);  // Draw the text
      else
        display->drawStr(topLeft.x,topLeft.y,text);  // Draw the text
      // Store the current text's area
      thisArea.set(topLeft.x,topLeft.y,topLeft.x+textSize.width,topLeft.y+textSize.height);
      thisArea.intersectWith(&dim);
      lastArea.uniteWith(&thisArea); // Old and current text area together are where this component has rendered things.
    }
    return &lastArea;
  }
  else // No need to render: Return empty area
    return &UIArea::EMPTY;
} // render()

void UITextLine::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  display->setFont(font);
  display->setFontRefHeightExtendedText();
  preferredSize->set(UISize::MAX_LEN,getTextHeight(display));
}

uint16_t UITextLine::getTextHeight(U8G2* display) {
  if (textSize.height==0) {
    display->setFont(font);
    display->setFontRefHeightExtendedText();
    textSize.height=(display->getAscent()-display->getDescent())+1;
  }
  return textSize.height;
}

// end of file
