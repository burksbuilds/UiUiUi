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
     * @param left Generate an visible frame of this thickness on the left side of the widget.
     * @param top Generate an visible frame of this thickness on the top side of the widget.
     * @param right Generate an visible frame of this thickness on the right side of the widget.
     * @param bottom Generate an visible frame of this thickness on the bottom side of the widget.
     * @param position move the border outward for the axes given by this parameter if more space is provided than requested.
	 * @param expansion Report an enlarged preferred size to the caller for the axes given by this parameter.
     * @param alignment Align the content widget this way if more space is provided than requested.
     * @param content The content of the border. If empty, the frame renders around its allocated space.
     * @param next Next widget on the same level as the border.
     */
    UIBorder(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom,UIExpansion position=UIExpansion::Both,UIExpansion expansion=UIExpansion::Both, UIAlignment alignment=UIAlignment::Center,UIWidget* content=nullptr,UIWidget* next=nullptr);

    /** Create a border widget.
     *
     * @param thickness Generate an visible frame of the specified uniform thickness around the content widget.
     * @param position move the border outward for the axes given by this parameter if more space is provided than requested.
	 * @param expansion Report an enlarged preferred size to the caller for the axes given by this parameter.
     * @param alignment Align the content widget this way if more space is provided than requested.
     * @param content The content of the border. If empty, the border renders an empty area.
     * @param next Next widget on the same level as the border.
     */
    UIBorder(uint16_t thickness,UIExpansion position,UIExpansion expansion, UIAlignment alignment,UIWidget* content=nullptr,UIWidget* next=nullptr);

/** Create a border widget.
     *
     * @param thickness Generate an visible frame of the specified uniform thickness around the content widget.
     * @param position move the border outward for the axes given by this parameter if more space is provided than requested.
	 * @param expansion Report an enlarged preferred size to the caller for the axes given by this parameter.
     * @param content The content of the border. If empty, the border renders an empty area.
     * @param next Next widget on the same level as the border.
     */
    UIBorder(uint16_t thickness,UIExpansion position,UIExpansion expansion, UIWidget* content=nullptr,UIWidget* next=nullptr);

    
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

    /** Frame position strategy when more space is allocated than requested. */
    UIExpansion position;
	
	/** Expansion strategy during computation of preferred size. */
    UIExpansion expansion;

    /** Alignment of inner content widget. */
    UIAlignment alignment;

    /** Border thickness, distinct for all 4 sides (kind of a hack to store them in this one parameter). */
    UIArea thickness;

    /** Reference to the content widget of the border. */
    UIWidget* content;
	
	/** Area occupied by the content. */
	UIArea contentDim;

};

// end of file
