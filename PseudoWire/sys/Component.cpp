#include "Platform.h"
#include "Component.h"

#include "Engine.h"
#include "Point.h"
#include "State.h"
#include "XComponent.h"

using namespace sys;

Component::Component(State* parent_state)
: parentstate_(parent_state)
, _parent(NULL)
, Position(Point(0,0))
, Size(Point(0,0))
, Locked(false)
, Visible(true)
, UserData(NULL)
{

}

Component::~Component()
{

}

bool Component::IsFocused() const
{
	return parentstate_->IsFocused(this);
}


void Component::SetParent(XComponent* new_parent)
{
	if(new_parent == NULL)
	{
		if(_parent != NULL)
		{
			// remove from current parent and send component to parent state
			for(s32 i = 0; i < _parent->GetChildCount(); ++i)
			{
				if(_parent->GetChild(i) == this)
				{
					// remove ourself from parent component
					_parent->RemoveChild(i, false);
					
					// add ourself to the parent state
					GetParentState()->AddComponent(this);
					
					//done in this loop.
					break;
				}
			}
		}
	}
	else
	{
		// remove from parent state (if in parent state) and place in new parent
		GetParentState()->RemoveComponent(this);
		new_parent->children.push_back(this);
	}
	this->_parent = new_parent;
}

// ***** OVERRIDEABLES *****
// Kept empty.

void Component::Draw(const Point& screenPos)
{
}

void Component::Tick(f32 dt)
{
}

void Component::OnKeyPress(s32 key, bool shift, bool ctrl, bool alt)
{
}

void Component::OnKeyDown(s32 key, bool shift, bool ctrl, bool alt)
{
}

void Component::OnKeyRelease(s32 key, bool shift, bool ctrl, bool alt)
{
}

void Component::OnKeyUp(s32 key, bool shift, bool ctrl, bool alt)
{
}

void Component::OnText(s32 key)
{
}

void Component::OnMouseClick(s32 localx, s32 localy, u32 button)
{
}

void Component::OnMouseDown(s32 x, s32 y, u32 button)
{
}

void Component::OnMouseHover(s32 localx, s32 localy)
{
}

void Component::OnMouseMoved(s32 localx, s32 localy, s32 dx, s32 dy)
{
}

void Component::OnMouseMovedInside(s32 localx, s32 localy, s32 dx, s32 dy)
{
}

void Component::OnMouseEnter(s32 localx, s32 localy)
{
}

void Component::OnMouseLeave(s32 localx, s32 localy)
{
}

void Component::OnMouseUnclick(s32 localx, s32 localy, u32 button)
{
}

void Component::OnMouseUp(s32 x, s32 y, u32 button)
{
}

void Component::OnMouseWheel(s32 localx, s32 localy, s32 d)
{
}

void Component::OnMouseWheelInside(s32 localx, s32 localy, s32 d)
{
}