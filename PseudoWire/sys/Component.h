#pragma once

#include "Point.h"
#include "State.h"
#include "Platform.h"

namespace sys
{
	class State;
	class XComponent;
	
	/* class Component
	 * 
	 * An interactive UI component that can be added to a state or an XComponent*.
	 * *See sys::XComponent
	 */
	class Component  
	{
	public:
		Component(State* parent_state);
		virtual ~Component();
		
		void* UserData;
		inline State* const GetParentState() const { return parentstate_; }
		bool IsFocused() const;

		Point Position;
		Point Size;
		bool Locked;
		bool Visible;

		/* See the parent of this component.
		 * If new_parent is NULL, this component will have no parent. (THIS DOES NOT delete THE COMPONENT. See XComponent::RemoveChild)
		 */
		void SetParent(XComponent* new_parent);

		//Get the parent component.
		inline XComponent* const GetParent() const { return _parent; }
		
		//UI functions:
		/*
			void Tick(float dt);
			void Draw(const sys::Point& screenPos);
			
			void OnMouseHover(int localx, int localy);
			void OnMouseMoved(int localx, int localy, int dx, int dy);
			void OnMouseMovedInside(int localx, int localy, int dx, int dy);
			void OnMouseEnter(int localx, int localy);
			void OnMouseLeave(int localx, int localy);
			void OnMouseDown(int x, int y, unsigned int button);
			void OnMouseUp(int x, int y, unsigned int button);
			void OnMouseClick(int localx, int localy, unsigned int button);
			void OnMouseUnclick(int localx, int localy, unsigned int button);
			void OnMouseWheel(int localx, int localy, int d);
			void OnMouseWheelInside(int localx, int localy, int d);
			void OnKeyPress(int key, bool shift, bool ctrl, bool alt);
			void OnKeyRelease(int key, bool shift, bool ctrl, bool alt);
			void OnKeyDown(int key, bool shift, bool ctrl, bool alt);
			void OnKeyUp(int key, bool shift, bool ctrl, bool alt);
			void OnText(int key);
		*/

		///
		// Called: Every tick.
		// Params:
		//	dt: The change in time.
		///
		virtual void Tick(f32 dt);
		
		///
		// Called: When ready to draw.
		// Params:
		// 	None
		///
		virtual void Draw(const Point& screenPos);
		
		
		
		
		///
		// Called: When the mouse is currently hovering over the item. (Called every tick)
		// Params:
		//	localx: Local mouse X position.
		//	localy: Local mouse Y position.
		///
		virtual void OnMouseHover(s32 localx, s32 localy);
		
		///
		// Called: When the mouse moves.
		// Params:
		//	localx: Local mouse X position.
		//	localy: Local mouse Y position.
		// 	dx: Mouse X delta.
		// 	dy: Mouse Y delta.
		///
		virtual void OnMouseMoved(s32 localx, s32 localy, s32 dx, s32 dy);
		
		///
		// Called: When the mouse moves.
		// Params:
		//	localx: Local mouse X position.
		//	localy: Local mouse Y position.
		// 	dx: Mouse X delta.
		// 	dy: Mouse Y delta.
		///
		virtual void OnMouseMovedInside(s32 localx, s32 localy, s32 dx, s32 dy);
		
		///
		// Called: When the mouse moves on top of the item.
		// Params:
		//	localx: Local mouse X position.
		//	localy: Local mouse Y position.
		// 	dx: Mouse X delta.
		// 	dy: Mouse Y delta.
		///
		virtual void OnMouseEnter(s32 localx, s32 localy);
		
		///
		// Called: When the mouse leaves the item.
		// Params:
		//	localx: Local mouse X position.
		//	localy: Local mouse Y position.
		///
		virtual void OnMouseLeave(s32 localx, s32 localy);
		
		///
		// Called: When a mouse button is pressed.
		// Params:
		// 	x: X position of the mouse.
		// 	y: Y position of the mouse.
		// 	button: The button that is being held down.
		///
		virtual void OnMouseDown(s32 x, s32 y, u32 button);
		
		///
		// Called: When a mouse button is released.
		// Params:
		// 	x: X position of the mouse.
		// 	y: Y position of the mouse.
		// 	button: The button that is being released.
		///
		virtual void OnMouseUp(s32 x, s32 y, u32 button);
		
		///
		// Called: When a mouse button is pressed on top of the item.
		// Params:
		// 	x: X position of the mouse.
		// 	y: Y position of the mouse.
		// 	button: The button that is being held down.
		///
		virtual void OnMouseClick(s32 localx, s32 localy, u32 button);
		
		///
		// Called: When a mouse button is released on top of the item.
		// Params:
		// 	x: X position of the mouse.
		// 	y: Y position of the mouse.
		// 	button: The button that is being released.
		///
		virtual void OnMouseUnclick(s32 localx, s32 localy, u32 button);
		
		///
		// Called: When the mouse wheel moves/changes.
		// Params:
		//	localx: Local mouse X position.
		//	localy: Local mouse Y position.
		// 	d: The mouse wheel movement value.
		///
		virtual void OnMouseWheel(s32 localx, s32 localy, s32 d);
		
		///
		// Called: When the mouse wheel moves/changes on top of the item.
		// Params:
		//	localx: Local mouse X position.
		//	localy: Local mouse Y position.
		// 	d: The mouse wheel movement value.
		///
		virtual void OnMouseWheelInside(s32 localx, s32 localy, s32 d);
		
		///
		// Called: When a key is pressed while focused.
		// Params:
		// 	key: The value of the key that is being pressed.
		// 	shift: Shift key is down.
		// 	ctrl: Control key is down.
		// 	alt: Alternate key is down.
		///
		virtual void OnKeyPress(s32 key, bool shift, bool ctrl, bool alt);
		
		///
		// Called: When a key is released while focused.
		// Params:
		// 	key: The value of the key that is being released.
		// 	shift: Shift key is released.
		// 	ctrl: Control key is released.
		// 	alt: Alternate key is released.
		///
		virtual void OnKeyRelease(s32 key, bool shift, bool ctrl, bool alt);

		///
		// Called: When a key is down.
		// Params:
		// 	key: The value of the key that is being released.
		// 	shift: Shift key is released.
		// 	ctrl: Control key is released.
		// 	alt: Alternate key is released.
		///
		virtual void OnKeyDown(s32 key, bool shift, bool ctrl, bool alt);

		///
		// Called: When a key is up.
		// Params:
		// 	key: The value of the key that is being released.
		// 	shift: Shift key is released.
		// 	ctrl: Control key is released.
		// 	alt: Alternate key is released.
		///
		virtual void OnKeyUp(s32 key, bool shift, bool ctrl, bool alt);

		///
		// Called: When text is entered. (Catches the keyboard buffer)
		// Params:
		//  key: The character value that is being entered.
		///
		virtual void OnText(s32 key);

	private:
		State* parentstate_;
		XComponent* _parent;
	};
}
