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
#include "UIParent.h"


/** Border widget which contains exactly one other widget, changes its representation, and wraps it in a visible frame.
 *
 * The border can do the following things:
 *
 * * If the enclosed widget does not spawn the whole display area it is assigned,
 *   the border can place it with a certain alignment.
 * * The border will add a visible frame to the inner widget with a supplied thickness.
 * * If the inner widget prefers to be smaller than the space it is allocated, 
     the frame can be positioned tightly around the widget or loosely around the provided space.
 */
class UIBorder : public UIWidget, public UIParent {
  public:

    /** Create a border widget.
     *
     * @param thickness Generate a visible frame of the specified horizontal and vertical thickness around the content widget.
     * @param content The content of the border. If empty, the border renders an empty area.
     * @param next Next widget on the same level as the border.
     */
    UIBorder(UISize thickness ,UIWidget* content=nullptr,UIWidget* next=nullptr);

/** Create a border widget.
     *
     * @param thickness Generate a visible frame of the specified uniform thickness around the content widget.
     * @param content The content of the border. If empty, the border renders an empty area.
     * @param next Next widget on the same level as the border.
     */
    UIBorder(uint16_t thickness, UIWidget* content=nullptr,UIWidget* next=nullptr);

    
    /** Layout the border.
     *
     * This includes shaping the content according to all the constraints.
     */
    void layout(U8G2 *display,UIArea *dim);

    /** Render the frame - which also renders the content if it exists. */
    UIArea* render(U8G2 *display,bool force);

    /** Called by the child of this frame to indicate that it wants to be rendered. */
    void childNeedsRendering(UIWidget *child);

  protected:

    /** Compute the preferred size which is determined by the content and the frame thickness. */
    void computePreferredSize(U8G2 *display,UISize *preferredSize);
	
	/** Compute the area representing the outside of the border frame given the allocated area of this widget and its content. */
    void computeOuterFrame(UIArea* allocatedArea, UIArea* contentArea, UIArea* outerFrame);

  private:


    /** Border thickness, horizontal lines may be thicker than vertical lines. */
    UISize thickness;

    /** Reference to the content widget of the border. */
    UIWidget* content;


};

// end of file
