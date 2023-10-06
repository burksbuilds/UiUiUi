// SPDX-License-Identifier: BSD-2-Clause
// (C) 2023 Andrew Burks

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIEnums.h"
#include "UIPoint.h"
#include "UISize.h"
#include "UIArea.h"
#include "UIWidget.h"
#include "UIBorder.h"


UIBorder::UIBorder(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom,UIExpansion position,UIExpansion expansion,UIAlignment alignment,UIWidget* content,UIWidget* next):
  UIWidget(next), position(position), expansion(expansion), alignment(alignment), thickness(left,top,right,bottom), content(content) {
    if (content)
      content->setParent(this);
  }

UIBorder::UIBorder(uint16_t thickness, UIExpansion position,UIExpansion expansion,UIAlignment alignment,UIWidget* content,UIWidget* next):
  UIBorder(thickness,thickness,thickness,thickness,position,expansion,alignment,content,next) {}
  
UIBorder::UIBorder(uint16_t thickness, UIExpansion position,UIExpansion expansion,UIWidget* content,UIWidget* next):
  UIBorder(thickness,thickness,thickness,thickness,position,expansion,UIAlignment::Center,content,next) {}

void UIBorder::layout(U8G2 *display,UIArea *dim) {
  UIWidget::layout(display,dim);
  if (content) {
    contentDim.set(dim);
	contentDim.shrink(thickness.left, thickness.top, thickness.right, thickness.bottom);
    UISize contentSize=contentDim.getSize();
    UISize preferredContentSize=content->preferredSize(display);
    if (preferredContentSize.width<contentSize.width || preferredContentSize.height<contentSize.height) {
      preferredContentSize.shrinkTo(&contentSize);
      UIPoint topLeft=contentDim.alignedTopLeft(alignment,&preferredContentSize);
      contentDim.set(&topLeft,&preferredContentSize);
    }
    content->layout(display,&contentDim);
  }
}

void UIBorder::childNeedsRendering(UIWidget *child) {
  signalNeedsRendering(); // just pass signal to parent
}

UIArea* UIBorder::render(U8G2 *display,bool force) {
  if (force) {
    clearFull(display);
	//draw a filled in rectangle sized by the outside of the frame, then a non-filled rectange sized by the inside of the frame
	UIArea frame;
	computeOuterFrame(&dim, &contentDim,&frame);//push border towards content or allocated space based on config 
	if(content && contentDim.hasArea() && frame.hasArea()){//only draw if there is some area for the content to draw
	  clip(display);
	  display->setDrawColor(1);
	  display->drawBox(frame.left,frame.top,frame.right-frame.left,frame.bottom-frame.top);
	  frame.shrink(thickness.left, thickness.top, thickness.right, thickness.bottom);
	  if(frame.hasArea()){
		display->setDrawColor(0);
	    display->drawBox(frame.left,frame.top,frame.right-frame.left,frame.bottom-frame.top);
	  }
	}
    if (content)
      content->render(display,true);
    return &dim;
  }
  else
    return (content?content->render(display,false):&UIArea::EMPTY);
}

void UIBorder::computePreferredSize(U8G2 *display,UISize *preferredSize) {
  if (content) {
    UISize contentSize=content->preferredSize(display);
    preferredSize->set(contentSize.width+thickness.left+thickness.right,contentSize.height+thickness.top+thickness.bottom);
  } 
  if (expansion==UIExpansion::Horizontal || expansion==UIExpansion::Both)
    preferredSize->width=UISize::MAX_LEN;
  if (expansion==UIExpansion::Vertical || expansion==UIExpansion::Both)
    preferredSize->height=UISize::MAX_LEN;
}

void UIBorder::computeOuterFrame(UIArea* allocatedArea, UIArea* contentArea, UIArea* outerFrame) {
	//compute left/right position
	if(content && (position == UIExpansion::None || position == UIExpansion::Vertical)){//case where border is tight around content
		outerFrame->left = contentArea->left-thickness.left;
		outerFrame->right = contentArea->right+thickness.right;
	} else {//case where border is moved to outside of alloted area
		outerFrame->left = allocatedArea->left;
		outerFrame->right = allocatedArea->right;
	}
	//compute top/bottom position
	if(content && (position == UIExpansion::None || position == UIExpansion::Horizontal)){//case where border is tight around content
		outerFrame->top = contentArea->top-thickness.top;
		outerFrame->bottom = contentArea->bottom+thickness.bottom;
	} else {//case where border is moved to outside of alloted area
		outerFrame->top = allocatedArea->top;
		outerFrame->bottom = allocatedArea->bottom;
	}
}

// end of file
