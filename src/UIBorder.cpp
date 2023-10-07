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


UIBorder::UIBorder(UISize thickness,UIWidget* content,UIWidget* next):
  UIWidget(next), thickness(thickness), content(content) {
    if (content)
      content->setParent(this);
  }

UIBorder::UIBorder(uint16_t thickness, UIWidget* content,UIWidget* next):
  UIBorder(UISize(thickness,thickness),content,next) {}
  

void UIBorder::layout(U8G2 *display,UIArea *dim) {
  UIWidget::layout(display,dim);
  if (content) {
	dim->debugPrint("border layout");
    UIArea contentDim=UIArea(dim);
	contentDim.shrink(&thickness);
	contentDim.debugPrint("content layout");
    content->layout(display,&contentDim);
  }
}

void UIBorder::childNeedsRendering(UIWidget *child) {
  signalNeedsRendering(); // just pass signal to parent
}

UIArea* UIBorder::render(U8G2 *display,bool force) {
  if (force) {
    clearFull(display);
	dim.debugPrint("border render");
    UIArea contentDim=UIArea(dim);
    contentDim.shrink(&thickness);
	contentDim.debugPrint("content render");
	
	if(content && contentDim.hasArea()){//only draw if there is some area for the content to draw
	  clip(display);
	  display->setDrawColor(1);
	  display->drawBox(dim.left,dim.top,thickness.width,dim.bottom-dim.top);//draw left border
	  display->drawBox(dim.left,dim.top,dim.right-dim.left,thickness.height);//draw top border
	  display->drawBox(dim.right-thickness.width,dim.top,thickness.width,dim.bottom-dim.top);//draw right border
	  display->drawBox(dim.left,dim.bottom-thickness.height,dim.right-dim.left,thickness.height);//draw bottom border
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
    preferredSize->set(&contentSize);
  } 
   preferredSize->cumulateBoth(&thickness);
   preferredSize->cumulateBoth(&thickness);//double because thickness dimensions only represent half of the total thickness each
   preferredSize->debugPrint("preferred size");
}


// end of file
