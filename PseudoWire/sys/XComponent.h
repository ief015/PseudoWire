#pragma once
#include <vector>
#include "Platform.h"

#include "Point.h"
#include "State.h"
#include "Component.h"

namespace sys
{
	/* class XComponent
	 * 
	 * An eXtension of the Component class.
	 * Adds the ability to have child components.
	 * 
	 * See sys::Component
	 */
	class XComponent : public Component
	{
	public:
		friend Component;

		XComponent(State* parent_state);
		virtual ~XComponent();
		
		/* Add a child component.
		 * Similar to XComponent::SetParent
		 * 
		 * If the component is already parented, then this will become the new parent.
		 */
		void AddChild(Component* c);
		
		// Remove child from component. This DOES NOT free the component from memory. (See other overload)
		void RemoveChild(Component* c);
		
		// Remove child from component. This WILL free the component from memory unless told otherwise. (See other overload)
		void RemoveChild(u32 idx, bool freeMem = true);
		
		//Grab the number of children this component owns.
		s32 GetChildCount();
		
		//Get child of this component by index.
		Component* GetChild(u32 idx);
		
		void Tick(float dt);
		void Draw(const Point& screenPos);
		
		void OnMouseHover(s32 localx, s32 localy);
		void OnMouseMoved(s32 localx, s32 localy, s32 dx, s32 dy);
		void OnMouseMovedInside(s32 localx, s32 localy, s32 dx, s32 dy);
		void OnMouseEnter(s32 localx, s32 localy);
		void OnMouseLeave(s32 localx, s32 localy);
		void OnMouseDown(s32 x, s32 y, u32 button);
		void OnMouseUp(s32 x, s32 y, u32 button);
		void OnMouseClick(s32 localx, s32 localy, u32 button);
		void OnMouseUnclick(s32 localx, s32 localy, u32 button);
		void OnMouseWheel(s32 localx, s32 localy, s32 d);
		void OnMouseWheelInside(s32 localx, s32 localy, s32 d);
		void OnKeyPress(s32 key, bool shift, bool ctrl, bool alt);
		void OnKeyRelease(s32 key, bool shift, bool ctrl, bool alt);
		void OnKeyDown(s32 key, bool shift, bool ctrl, bool alt);
		void OnKeyUp(s32 key, bool shift, bool ctrl, bool alt);
		void OnText(s32 key);

	private:
		Component* hoveringComponent_;
		
	protected:
		// child components
		std::vector<Component*> children;
		
		//UI functions:
		/*
			void XTick(float dt);
			void XDraw(const sys::Point& screenPos);
			
			void XOnMouseHover(int localx, int localy);
			void XOnMouseMoved(int localx, int localy, int dx, int dy);
			void XOnMouseMovedInside(int localx, int localy, int dx, int dy);
			void XOnMouseEnter(int localx, int localy);
			void XOnMouseLeave(int localx, int localy);
			void XOnMouseDown(int x, int y, unsigned int button);
			void XOnMouseUp(int x, int y, unsigned int button);
			void XOnMouseClick(int localx, int localy, unsigned int button);
			void XOnMouseUnclick(int localx, int localy, unsigned int button);
			void XOnMouseWheel(int localx, int localy, int d);
			void XOnMouseWheelInside(int localx, int localy, int d);
			void XOnKeyPress(int key, bool shift, bool ctrl, bool alt);
			void XOnKeyRelease(int key, bool shift, bool ctrl, bool alt);
			void XOnKeyDown(int key, bool shift, bool ctrl, bool alt);
			void XOnKeyUp(int key, bool shift, bool ctrl, bool alt);
			void XOnText(int key);
		*/
		
		// Overridable. Called by XComponent::Tick()
		virtual void XTick(float dt);
		
		// Overridable. Called by XComponent::Draw()
		virtual void XDraw(const Point& screenPos);
		
		
		// Overridable. Called by XComponent::XOnMouseHover()
		virtual void XOnMouseHover(s32 localx, s32 localy);
		
		// Overridable. Called by XComponent::OnMouseMoved()
		virtual void XOnMouseMoved(s32 localx, s32 localy, s32 dx, s32 dy);
		
		// Overridable. Called by XComponent::OnMouseMovedInside()
		virtual void XOnMouseMovedInside(s32 localx, s32 localy, s32 dx, s32 dy);
		
		// Overridable. Called by XComponent::OnMouseEnter()
		virtual void XOnMouseEnter(s32 localx, s32 localy);
		
		// Overridable. Called by XComponent::OnMouseLeave()
		virtual void XOnMouseLeave(s32 localx, s32 localy);
		
		// Overridable. Called by XComponent::OnMouseDown()
		virtual void XOnMouseDown(s32 x, s32 y, u32 button);
		
		// Overridable. Called by XComponent::OnMouseUp()
		virtual void XOnMouseUp(s32 x, s32 y, u32 button);
		
		// Overridable. Called by XComponent::OnMouseClick()
		virtual void XOnMouseClick(s32 localx, s32 localy, u32 button);
		
		// Overridable. Called by XComponent::OnMouseUnclick()
		virtual void XOnMouseUnclick(s32 localx, s32 localy, u32 button);
		
		// Overridable. Called by XComponent::OnMouseWheel()
		virtual void XOnMouseWheel(s32 localx, s32 localy, s32 d);
		
		// Overridable. Called by XComponent::OnMouseWheelInside()
		virtual void XOnMouseWheelInside(s32 localx, s32 localy, s32 d);
		
		// Overridable. Called by XComponent::OnKeyPress()
		virtual void XOnKeyPress(s32 key, bool shift, bool ctrl, bool alt);
		
		// Overridable. Called by XComponent::OnKeyRelease()
		virtual void XOnKeyRelease(s32 key, bool shift, bool ctrl, bool alt);

		// Overridable. Called by XComponent::OnKeyDown()
		virtual void XOnKeyDown(s32 key, bool shift, bool ctrl, bool alt);

		// Overridable. Called by XComponent::OnKeyUp()
		virtual void XOnKeyUp(s32 key, bool shift, bool ctrl, bool alt);

		// Overridable. Called by XComponent::OnText()
		virtual void XOnText(s32 key);
	};

}
