// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIDisplay.h"

/* Setup the UIDisplay onto the given U8g2 instance and with the given root widget. */
UIDisplay::UIDisplay(UIWidget* root):
root(root), maxFirstUpdateTiles(0xffff), maxFollowUpdateTiles(0xffff) {
  root->setParent(this);
}

/* Initialize the whole user interface, layout the widgets. */
void UIDisplay::init(U8G2* display,bool enable,bool render) {

  // trigger computation of the preferred sizes throughout the complete widget tree.
  // UIDisplay itself does not care about preferred sizes, it knows the size pf the physical
  // display and that's all that counts...
  root->preferredSize(display);

  // Create the dimension of the actual physical display
  UIArea fullscreen=UIArea(0,0,display->getDisplayWidth(),display->getDisplayHeight());

  // Layout all components, this is done throughout the whole widget tree.
  root->layout(display,&fullscreen);

  // Finally enable the display and perform initial rendering if requested
  active=true;
  if (enable) {
    this->enabled=false;
    this->enable(display,render,true);
  }
  else if (render)
    this->render(display,true);
}

void UIDisplay::setUpdateTiles(uint16_t maxFirstUpdateTiles,uint16_t maxFollowUpdateTiles) {
  this->maxFirstUpdateTiles=maxFirstUpdateTiles;
  this->maxFollowUpdateTiles=maxFollowUpdateTiles;
}

/** Enable the UI: It presents itself on screen, display is turned on. */
void UIDisplay::enable(U8G2* display,bool render,bool force) {
  if (!enabled) {
    display->setPowerSave(0);
    enabled=true;
    if (render)
      this->render(display,force);
  }
}

/** Disables the UI: No rendering and display is turned off. */
void UIDisplay::disable(U8G2* display) {
  if (enabled) {
    display->setPowerSave(1);
    enabled=false;
  }
}

/** Deactivates the UI: It will be shown but no rendering will be performed. */
void UIDisplay::deactivate() {
  active=false;
}

/** Activates the UI: Rendering will be performed if render() is called. */
void UIDisplay::activate() {
  active=true;
}

/** Called by the root widget to indicate that rendering must be performed. */
void UIDisplay::childNeedsRendering(UIWidget *child) {
  renderingNeeded=true;
}

void UIDisplay::doUpdateTiles(U8G2 *display,uint16_t maxUpdateTiles) {
  uint16_t updateWidth=updateTiles.right-updateTiles.left;
  uint16_t updateHeight=updateTiles.bottom-updateTiles.top;
  if (maxUpdateTiles>=updateWidth*updateHeight) {
    // If maximum number of update tiles allows to update the whole area, just do it.
    display->updateDisplayArea(updateTiles.left,updateTiles.top,updateWidth,updateHeight);
    if (furtherUpdateTiles.hasArea()) {
      updateTiles.set(&furtherUpdateTiles);
      furtherUpdateTiles.clear();
    }
    else
      updateTiles.clear();
  }
  else {
    if (updateWidth>maxUpdateTiles && !furtherUpdateTiles.hasArea()) {
      furtherUpdateTiles.set(&updateTiles);
      updateTiles.right=updateTiles.left+maxUpdateTiles;
      furtherUpdateTiles.left=updateTiles.right;
      updateWidth=maxUpdateTiles;
    }
    uint16_t updateRows=maxUpdateTiles/updateWidth;
    display->updateDisplayArea(updateTiles.left,updateTiles.top,updateWidth,updateRows);
    updateTiles.top+=updateRows;
    if (!updateTiles.hasArea() && furtherUpdateTiles.hasArea()) {
      updateTiles.set(&furtherUpdateTiles);
      furtherUpdateTiles.clear();
    }
  }
}

/* Render the interface, update everything that has changed since the last render() call. */
void UIDisplay::render(U8G2 *display,bool force) {
  // Do not do anything if display is not enabled
  if (!enabled)
    return;
  if (!force && updateTiles.hasArea())  // updating display tiles takes precedence over (non-forced) rendering
    doUpdateTiles(display,maxFollowUpdateTiles);
  else if (active && (force || renderingNeeded)) { // start rendering only if active and some need for rendering
    renderingNeeded=false;
    UIArea rendered=UIArea();
    rendered.uniteWith(root->render(display,force));
    if (rendered.hasArea()) {
	  computeTileAreaFromPixelArea(display,&rendered,&updateTiles);
      if (maxFirstUpdateTiles>0)
        doUpdateTiles(display,maxFirstUpdateTiles);
    }
  }
}

bool UIDisplay::isUpdatingDisplay() {
  return updateTiles.hasArea();
}

void UIDisplay::computeTileAreaFromPixelArea(U8G2 *display, UIArea * pixelArea, UIArea * tileArea)
{
  //resulting computation depends on the rotation of the display. 
  //Note that the U8G2 rotation #define is actually a callback function pointer, not just an enum, so be careful!
  if(display->getU8g2()->cb == U8G2_R0)//No rotation
  {
  	tileArea->set(((                             pixelArea->left)    >>3)&0xff,
			      ((                             pixelArea->top)     >>3)&0xff,
			      ((                             pixelArea->right+7) >>3)&0xff,
			      ((                             pixelArea->bottom+7)>>3)&0xff);
  }
  else if(display->getU8g2()->cb == U8G2_R1)//top right of pixel area is top left of screen
  {
  	tileArea->set(((display->getDisplayHeight()- pixelArea->bottom)  >>3)&0xff,
			      ((                             pixelArea->left)    >>3)&0xff,
			      ((display->getDisplayHeight()- pixelArea->top+7)   >>3)&0xff,
			      ((                             pixelArea->right+7) >>3)&0xff);
  }
  else if(display->getU8g2()->cb == U8G2_R2)//top right of pixel area is bottom left of screen
  {
  	tileArea->set(((display->getDisplayWidth() - pixelArea->right)   >>3)&0xff,
			      ((display->getDisplayHeight()- pixelArea->bottom)  >>3)&0xff,
			      ((display->getDisplayWidth() - pixelArea->left+7)  >>3)&0xff,
			      ((display->getDisplayHeight()- pixelArea->top+7)   >>3)&0xff);
  }
  else if(display->getU8g2()->cb == U8G2_R3)//top right of pixel area is bottom right of screen
  {
  	tileArea->set(((                             pixelArea->top)     >>3)&0xff,
			      ((display->getDisplayWidth() - pixelArea->left)    >>3)&0xff,
			      ((                             pixelArea->bottom+7)>>3)&0xff,
			      ((display->getDisplayWidth() - pixelArea->right+7) >>3)&0xff);
  }
  else//Should never happen. just update the whole screen.
  {
  	tileArea->set((0)&0xff,
			      (0)&0xff,
			      ((display->getDisplayWidth())                      >>3)&0xff,
			      ((display->getDisplayHeight())                     >>3)&0xff);
  }
}

// end of file
