#include "Engine.h"
#include "State.h"

#include <vector>
#include "Component.h"
#include "Platform.h"

using namespace sys;

State::State()
: UserData(NULL)
, focusedComponent_(NULL)
, hoveringComponent_(NULL)
{
}

State::~State()
{
	for(u32 i = 0, sz = Components.size(); i < sz; ++i)
		if( Components[i] )
			delete Components[i];
}

void State::AddComponent(Component* c)
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
	Components.push_back(c);
}

void State::InsertComponent(Component* c, int pos)
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
	Components.insert(Components.begin()+pos, c);
}

u32 State::GetComponentCount()
{
	return Components.size();
}

Component* State::GetComponent(u32 idx)
{
	return Components[idx];
}

void State::RemoveComponent(Component* c)
{
	// remove component WITHOUT freeing it.
	for(u32 i = 0; i < Components.size(); ++i)
	{
		// find the appropriate component index
		if(Components[i] == c)
		{
			if(Components[i] == hoveringComponent_)
				hoveringComponent_ = NULL;

			Components.erase(Components.begin() + i);
			
			// we're done
			return;
		}
	}
}

void State::RemoveComponent(u32 idx)
{
	if(Components[idx] == hoveringComponent_)
		hoveringComponent_ = NULL;

	// free component and remove it.
	delete Components[idx];
	Components.erase(Components.begin() + idx);
}

bool State::IsFocused(const Component* c) const
{
	return c == focusedComponent_;
}

void State::FocusComponent(Component* c)
{
	this->focusedComponent_ = c;
}

void State::DoExit()
{

	OnExit();
}

void State::DoInitialized()
{
	OnInitialized();
}

void State::DoDraw()
{
	OnDraw();

	//draw
	for(s32 i = 0, sz = Components.size(); i < sz; ++i)
	{
		Component* c = Components[i];
		if(c->Visible)
		{
			if(AllowExclusiveDrawing)
			{
				c->Draw(c->Position);
			}
			else
			{
				if( c->Position.X + c->Size.X >= 0 &&
					c->Position.Y + c->Size.Y >= 0 &&
					c->Position.X < sys::Engine::Ref().GetWidth() &&
					c->Position.Y < sys::Engine::Ref().GetHeight() )
				{
					c->Draw( c->Position );
				}
			}
		}
	}

	OnPostFXDraw();
}

void State::DoTick(f32 dt)
{
	//on mouse hover
	for(s32 i = Components.size() - 1; i >= 0; --i)
	{
		Component* c = Components[i];
		if(!c->Locked &&
			sys::Engine::Ref().GetMouseX() >= c->Position.X &&
			sys::Engine::Ref().GetMouseY() >= c->Position.Y &&
			sys::Engine::Ref().GetMouseX() < c->Position.X + c->Size.X &&
			sys::Engine::Ref().GetMouseY() < c->Position.Y + c->Size.Y )
		{
			c->OnMouseHover(sys::Engine::Ref().GetMouseX() - c->Position.X, sys::Engine::Ref().GetMouseY() - c->Position.Y);
			hoveringComponent_ = c;
			break;
		}
	}
			
	//tick
	for(s32 i = 0, sz = Components.size(); i < sz; ++i)
	{
		Components[i]->Tick(dt);
	}

	OnTick(dt);
}

void State::DoResize(s32 w, s32 h)
{
	OnResize(w, h);
}

void State::DoKeyPress(s32 key, bool shift, bool ctrl, bool alt)
{
	//on key press
	if(focusedComponent_ != NULL)
	{
		if(!focusedComponent_->Locked)
			focusedComponent_->OnKeyPress(key, shift, ctrl, alt);
	}

	for(s32 i = Components.size() - 1; i >= 0 ; --i)
	{
		if(!Components[i]->Locked)
		{
			Components[i]->OnKeyDown(key, shift, ctrl, alt);
		}
	}

	OnKeyPress(key, shift, ctrl, alt);
}

void State::DoKeyRelease(s32 key, bool shift, bool ctrl, bool alt)
{
	//on key unpress
	if(focusedComponent_ != NULL)
	{
		if(!focusedComponent_->Locked)
			focusedComponent_->OnKeyRelease(key, shift, ctrl, alt);
	}

	for(s32 i = Components.size() - 1; i >= 0 ; --i)
	{
		if(!Components[i]->Locked)
		{
			Components[i]->OnKeyUp(key, shift, ctrl, alt);
		}
	}

	OnKeyRelease(key, shift, ctrl, alt);
}

void State::DoText(s32 key)
{
	if(focusedComponent_ != NULL)
	{
		if(!focusedComponent_->Locked)
			focusedComponent_->OnText(key);
	}

	OnText(key);
}

void State::DoMouseDown(s32 x, s32 y, u32 button)
{
	//on mouse click
	bool clickState = false;
	for(s32 i = Components.size() - 1; i >= 0 ; --i)
	{
		Component* c = Components[i];
		if(!c->Locked)
		{
			if(x >= c->Position.X && y >= c->Position.Y && x < c->Position.X + c->Size.X && y < c->Position.Y + c->Size.Y)
			{
				FocusComponent(c);
				c->OnMouseClick(x - c->Position.X, y - c->Position.Y, button);
				clickState = true;
				OnMouseClickedComponent(x, y, button, c);
				break;
			}
		}
	}

	if(!clickState)
	{
		OnMouseClicked(x, y, button);
		FocusComponent(NULL);
	}

	//on mouse down
	for(s32 i = Components.size() - 1; i >= 0 ; --i)
	{
		Component* c = Components[i];
		if(!c->Locked)
			c->OnMouseDown(x, y, button);
	}

	OnMouseDown(x, y, button);
}

void State::DoMouseMove(s32 x, s32 y, s32 dx, s32 dy)
{
	//on mouse move (if true, and inside)

	bool domouseleaveenter = true;
	for(s32 i = Components.size() - 1; i >= 0 ; --i)
	{
		Component* c = Components[i];
		
		if(!c->Locked)
		{
			Point local	(x - c->Position.X, y - c->Position.Y)
			, a (local.X - dx, local.Y - dy);
			
			c->OnMouseMoved(local.X, local.Y, dx, dy);
			
			if(domouseleaveenter)
			{
				if(local.X >= 0 &&
				   local.Y >= 0 &&
				   local.X < c->Size.X &&
				   local.Y < c->Size.Y )
				{
					c->OnMouseMovedInside(local.X, local.Y, dx, dy);
				
					// entering?
					if(!(
						a.X >= 0 &&
						a.Y >= 0 &&
						a.X < c->Size.X &&
						a.Y < c->Size.Y ))
					{
						if(hoveringComponent_)
						{
							hoveringComponent_->OnMouseLeave( hoveringComponent_->Position.X - x, hoveringComponent_->Position.Y - y );
						}
						c->OnMouseEnter(local.X, local.Y);
						hoveringComponent_ = c;
					}

					domouseleaveenter = false;
					//break;
				}
				else
				{
					// leaving?
					if(	a.X >= 0 &&
						a.Y >= 0 &&
						a.X < c->Size.X &&
						a.Y < c->Size.Y )
					{
						c->OnMouseLeave(local.X, local.Y);
					}
				
				}
			}
		}
	}

	OnMouseMove(x, y, dx, dy);
}

void State::DoMouseUp(s32 x, s32 y, u32 button)
{
	//on mouse unclick
	bool unclicked = true;
	for(s32 i = Components.size() - 1; i >= 0 ; --i)
	{
		Component* c = Components[i];
		if(!c->Locked)
		{
			if(x >= c->Position.X && y >= c->Position.Y && x < c->Position.X + c->Size.X && y < c->Position.Y + c->Size.Y)
			{
				c->OnMouseUnclick(x - c->Position.X, y - c->Position.Y, button);
				unclicked = false;
				OnMouseUnclickedComponent(x, y, button, c);
				break;
			}
		}
	}

	if(unclicked)
	{
		OnMouseUnclicked(x, y, button);
	}

	//on mouse up
	for(s32 i = Components.size() - 1; i >= 0 ; --i)
	{
		Component* c = Components[i];
		if(!c->Locked)
			c->OnMouseUp(x, y, button);
	}

	OnMouseUp(x, y, button);
}

void State::DoMouseWheel(s32 x, s32 y, s32 d)
{
	//on mouse wheel focused
	for(s32 i = Components.size() - 1; i >= 0 ; --i)
	{
		Component* c = Components[i];
		if(x >= c->Position.X && y >= c->Position.Y && x < c->Position.X + c->Size.X && y < c->Position.Y + c->Size.Y)
		{
			if(!c->Locked)
				c->OnMouseWheelInside(x - c->Position.X, y - c->Position.Y, d);
			break;
		}
	}
	
	//on mouse wheel
	for(s32 i = Components.size() - 1; i >= 0 ; --i)
	{
		Component* c = Components[i];
		if(!c->Locked)
			c->OnMouseWheel(x - c->Position.X, y - c->Position.Y, d);
	}

	OnMouseWheel(x, y, d);
}
