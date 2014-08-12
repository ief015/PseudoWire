#pragma once
#include <vector>
#include "Platform.h"

#include "XComponent.h"

#include "Point.h"
#include "State.h"
#include "Component.h"

using namespace sys;

XComponent::XComponent(State* parent_state)
: Component(parent_state)
, hoveringComponent_(NULL)
{
}

XComponent::~XComponent()
{
	for(u32 i = 0; i < children.size(); ++i)
	{
		if( children[i] )
			delete children[i];
	}
}

void XComponent::AddChild(Component* c)
{
	if(!c->Locked &&
		sys::Engine::Ref().GetMouseX() >= c->Position.X &&
		sys::Engine::Ref().GetMouseY() >= c->Position.Y &&
		sys::Engine::Ref().GetMouseX() < c->Position.X + c->Size.X &&
		sys::Engine::Ref().GetMouseY() < c->Position.Y + c->Size.Y )
	{
		if(hoveringComponent_)
		{
			hoveringComponent_->OnMouseLeave( hoveringComponent_->Position.X - sys::Engine::Ref().GetMouseX(), hoveringComponent_->Position.Y - sys::Engine::Ref().GetMouseY() );
		}
		hoveringComponent_ = c;
	}

	c->SetParent(this);
}

s32 XComponent::GetChildCount()
{
	return children.size();
}

Component* XComponent::GetChild(u32 idx)
{
	return children[idx];
}

void XComponent::RemoveChild(Component* c)
{
	for(u32 i = 0; i < children.size(); ++i)
	{
		if(children[i] == c)
		{
			if(children[i] == hoveringComponent_)
				hoveringComponent_ = NULL;

			//remove child from parent. Does not free memory
			children.erase(children.begin() + i);
			break;
		}
	}
}

void XComponent::RemoveChild(u32 idx, bool freeMem)
{
	if(children[idx] == hoveringComponent_)
		hoveringComponent_ = NULL;
	
	if(freeMem)
		delete children[idx];

	children.erase(children.begin() + idx);
}

void XComponent::Draw(const Point& screenPos)
{
	// draw ourself first
	XDraw(screenPos);
	
	// attempt to draw all children
	for(u32 i = 0; i < children.size(); ++i)
	{
		Component* c = children[i];
		
		// the component must be visible
		if(c->Visible)
		{
			if(GetParentState()->AllowExclusiveDrawing)
			{
				//who cares if the component is off the screen? draw anyway.
				Point scrpos = screenPos + c->Position;
				c->Draw(scrpos);
			}
			else
			{
				//check if the component is in the screen, draw if it is
				if( c->Position.X + c->Size.X >= 0 &&
					c->Position.Y + c->Size.Y >= 0 &&
					c->Position.X < sys::Engine::Ref().GetWidth() &&
					c->Position.Y < sys::Engine::Ref().GetHeight() )
				{
					Point scrpos = screenPos + c->Position;
					c->Draw(scrpos);
				}
			}
		}
	}
}

void XComponent::Tick(f32 dt)
{
	// tick ourself first
	XTick(dt);
	
	// tick our children
	for(u32 i = 0; i < children.size(); ++i)
		children[i]->Tick(dt);
}

void XComponent::OnKeyPress(s32 key, bool shift, bool ctrl, bool alt)
{
	XOnKeyPress(key, shift, ctrl, alt);
}

void XComponent::OnKeyRelease(s32 key, bool shift, bool ctrl, bool alt)
{
	XOnKeyRelease(key, shift, ctrl, alt);
}

void XComponent::OnKeyDown(s32 key, bool shift, bool ctrl, bool alt)
{
	XOnKeyPress(key, shift, ctrl, alt);
}

void XComponent::OnKeyUp(s32 key, bool shift, bool ctrl, bool alt)
{
	XOnKeyRelease(key, shift, ctrl, alt);
}

void XComponent::OnText(s32 key)
{
	XOnText(key);
}

void XComponent::OnMouseClick(s32 localx, s32 localy, u32 button)
{
	bool childclicked = false;
	
	//check if clicked a child
	for(s32 i = static_cast<s32>(children.size()-1); i >= 0 ; --i)
	{
		Component* c = children[i];
		//child must be unlocked
		if(!c->Locked)
		{
			//is mouse inside?
			if( localx >= c->Position.X &&
				localy >= c->Position.Y &&
				localx < c->Position.X + c->Size.X &&
				localy < c->Position.Y + c->Size.Y )
			{
				childclicked = true;
				GetParentState()->FocusComponent(c);
				c->OnMouseClick(localx - c->Position.X, localy - c->Position.Y, button);
				break;
			}
		}
	}
	
	//if a child wasn't clicked, send click to ourself
	if(!childclicked)
	{
		XOnMouseClick(localx, localy, button);
		GetParentState()->FocusComponent(this);
	}
}

void XComponent::OnMouseDown(s32 x, s32 y, u32 button)
{
	XOnMouseDown(x, y, button);
	for(u32 i = 0; i < children.size(); ++i)
	{
		Component* c = children[i];
		if(!c->Locked)
			c->OnMouseDown(x, y, button);
	}
}

void XComponent::OnMouseHover(s32 localx, s32 localy)
{
	// check if hovering on children
	for(s32 i = static_cast<s32>(children.size()-1); i >= 0; --i)
	{
		Component* c = children[i];
		if(!c->Locked)
		{
			if( localx >= c->Position.X &&
				localy >= c->Position.Y &&
				localx < c->Position.X + c->Size.X &&
				localy < c->Position.Y + c->Size.Y )
			{
				c->OnMouseHover(localx - c->Position.X, localy - c->Position.Y);
				hoveringComponent_ = c;
				break;
			}
		}
	}
	
	// always allow hover on parent (?)
	XOnMouseHover(localx, localy);
}

void XComponent::OnMouseMoved(s32 localx, s32 localy, s32 dx, s32 dy)
{
	XOnMouseMoved(localx, localy, dx, dy);
	for(u32 i = 0; i < children.size(); ++i)
	{
		Component* c = children[i];
		if(!c->Locked)
			c->OnMouseMoved(localx - c->Position.X, localy - c->Position.Y, dx, dy);
	}
}

void XComponent::OnMouseMovedInside(s32 localx, s32 localy, s32 dx, s32 dy)
{
	for(u32 i = 0; i < children.size(); ++i)
	{
		Component* c = children[i];
		if(!c->Locked)
		{
			Point local	(localx - c->Position.X, localy - c->Position.Y)
			, prevlocal (local.X - dx, local.Y - dy);
			
			// mouse currently inside?
			if( local.X >= 0 &&
				local.Y >= 0 &&
				local.X < c->Size.X &&
				local.Y < c->Size.Y )
			{
				c->OnMouseMovedInside(localx - c->Position.X, localy - c->Position.Y, dx, dy);
				
				// was the mouse outside?
				if(!(prevlocal.X >= 0 &&
					 prevlocal.Y >= 0 &&
					 prevlocal.X < c->Size.X &&
					 prevlocal.Y < c->Size.Y ) )
				{
					if(hoveringComponent_)
					{
						hoveringComponent_->OnMouseLeave( hoveringComponent_->Position.X - localx, hoveringComponent_->Position.Y - localy );
					}
					c->OnMouseEnter(local.X, local.Y);
					hoveringComponent_ = c;
				}
			}
			// if not currently inside
			else
			{
				// was the mouse inside?
				if(	prevlocal.X >= 0 &&
					prevlocal.Y >= 0 &&
					prevlocal.X < c->Size.X &&
					prevlocal.Y < c->Size.Y )
				{
					c->OnMouseLeave(local.X, local.Y);
				}
				
			}
		}
	}
	
	// always allow hover on parent (?)
	XOnMouseMovedInside(localx, localy, dx, dy);
}

void XComponent::OnMouseEnter(s32 localx, s32 localy)
{
	XOnMouseEnter(localx, localy);
}

void XComponent::OnMouseLeave(s32 localx, s32 localy)
{
	XOnMouseLeave(localx, localy);
}

void XComponent::OnMouseUnclick(s32 localx, s32 localy, u32 button)
{
	bool childunclicked = false;
	
	//check if clicked a child
	for(s32 i = static_cast<s32>(children.size()-1); i >= 0 ; --i)
	{
		Component* c = children[i];
		
		//child must be unlocked
		if(!c->Locked)
		{
			//is mouse inside?
			if( localx >= c->Position.X &&
				localy >= c->Position.Y &&
				localx < c->Position.X + c->Size.X &&
				localy < c->Position.Y + c->Size.Y )
			{
				childunclicked = true;
				c->OnMouseUnclick(localx - c->Position.X, localy - c->Position.Y, button);
				break;
			}
		}
	}
	
	//if a child wasn't clicked, send click to ourself
	if(!childunclicked)
	{
		XOnMouseUnclick(localx, localy, button);
	}
}

void XComponent::OnMouseUp(s32 x, s32 y, u32 button)
{
	XOnMouseUp(x, y, button);
	for(u32 i = 0; i < children.size(); ++i)
	{
		Component* c = children[i];
		if(!c->Locked)
			c->OnMouseUp(x, y, button);
	}
}

void XComponent::OnMouseWheel(s32 localx, s32 localy, s32 d)
{
	XOnMouseWheel(localx, localy, d);
	for(u32 i = 0; i < children.size(); ++i)
	{
		Component* c = children[i];
		if(!c->Locked)
			c->OnMouseWheel(localx - c->Position.X, localy - c->Position.Y, d);
	}
}

void XComponent::OnMouseWheelInside(s32 localx, s32 localy, s32 d)
{
	XOnMouseWheelInside(localx, localy, d);
	//check if clicked a child
	for(s32 i = static_cast<s32>(children.size()-1); i >= 0 ; --i)
	{
		Component* c = children[i];
		
		//child must be unlocked
		if(!c->Locked)
		{
			//is mouse inside?
			if( localx >= c->Position.X &&
				localy >= c->Position.Y &&
				localx < c->Position.X + c->Size.X &&
				localy < c->Position.Y + c->Size.Y )
			{
				c->OnMouseWheelInside(localx - c->Position.X, localy - c->Position.Y, d);
				break;
			}
		}
	}
}

// ***** OVERRIDEABLES *****
// Kept empty.

void XComponent::XDraw(const Point& screenPos)
{
}

void XComponent::XTick(f32 dt)
{
}

void XComponent::XOnKeyDown(s32 key, bool shift, bool ctrl, bool alt)
{
}

void XComponent::XOnKeyPress(s32 key, bool shift, bool ctrl, bool alt)
{
}

void XComponent::XOnKeyRelease(s32 key, bool shift, bool ctrl, bool alt)
{
}

void XComponent::XOnKeyUp(s32 key, bool shift, bool ctrl, bool alt)
{
}

void XComponent::XOnText(s32 key)
{
}

void XComponent::XOnMouseClick(s32 localx, s32 localy, u32 button)
{
}

void XComponent::XOnMouseDown(s32 x, s32 y, u32 button)
{
}

void XComponent::XOnMouseHover(s32 localx, s32 localy)
{
}

void XComponent::XOnMouseMoved(s32 localx, s32 localy, s32 dx, s32 dy)
{
}

void XComponent::XOnMouseMovedInside(s32 localx, s32 localy, s32 dx, s32 dy)
{
}

void XComponent::XOnMouseEnter(s32 localx, s32 localy)
{
}

void XComponent::XOnMouseLeave(s32 localx, s32 localy)
{
}

void XComponent::XOnMouseUnclick(s32 localx, s32 localy, u32 button)
{
}

void XComponent::XOnMouseUp(s32 x, s32 y, u32 button)
{
}

void XComponent::XOnMouseWheel(s32 localx, s32 localy, s32 d)
{
}

void XComponent::XOnMouseWheelInside(s32 localx, s32 localy, s32 d)
{
}