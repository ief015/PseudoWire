#pragma once

#include <vector>

#include "Engine.h"
#include "Component.h"
#include "Platform.h"

namespace sys
{
	class Engine;
	class Component;
	
	/* class State
	 * 
	 * A UI state. Contains all components.
	 */
	class State
	{
	public:
		State();
		virtual ~State();

		bool AllowExclusiveDrawing; //false will not call draw on objects outside of bounds

		// Add Component to state
		void AddComponent(Component* c);

		// Add Component to state at position
		void InsertComponent(Component* c, s32 pos);
		
		// Get the number of components this state has.
		u32 GetComponentCount();
		
		// Get component by index. (See GetComponentCount())
		Component* GetComponent(u32 idx);
		
		// Remove a component from state. NOTE: This DOES NOT free component from memory.
		void RemoveComponent(Component* c);
		
		// Remove a component from state. NOTE: This WILL free component from memory.
		void RemoveComponent(u32 idx);
		
		void DoInitialized();
		void DoExit();
		void DoTick(f32 dt);
		void DoDraw();
		void DoResize(s32 w, s32 h);

		void DoMouseMove(s32 x, s32 y, s32 dx, s32 dy);
		void DoMouseDown(s32 x, s32 y, u32 button);
		void DoMouseUp(s32 x, s32 y, u32 button);
		void DoMouseWheel(s32 x, s32 y, s32 d);
		void DoKeyPress(s32 key, bool shift, bool ctrl, bool alt);
		void DoKeyRelease(s32 key, bool shift, bool ctrl, bool alt);
		void DoText(s32 key);

		bool IsFocused(const Component* c) const;
		void FocusComponent(Component* c);

		void* UserData;

	protected:
		virtual void OnInitialized() {}
		virtual void OnExit() {}
		virtual void OnTick(f32 dt) {}
		virtual void OnDraw() {}
		virtual void OnResize(s32 w, s32 h) {}
		virtual void OnPostFXDraw() {}

		virtual void OnMouseMove(s32 x, s32 y, s32 dx, s32 dy) {}
		virtual void OnMouseDown(s32 x, s32 y, u32 button) {}
		virtual void OnMouseUp(s32 x, s32 y, u32 button) {}
		virtual void OnMouseClicked(s32 x, s32 y, u32 button) {}
		virtual void OnMouseUnclicked(s32 x, s32 y, u32 button) {}
		virtual void OnMouseClickedComponent(s32 x, s32 y, u32 button, Component* comp) {}
		virtual void OnMouseUnclickedComponent(s32 x, s32 y, u32 button, Component* comp) {}
		virtual void OnMouseWheel(s32 x, s32 y, s32 d) {}
		virtual void OnKeyPress(s32 key, bool shift, bool ctrl, bool alt) {}
		virtual void OnKeyRelease(s32 key, bool shift, bool ctrl, bool alt) {}
		virtual void OnText(s32 key) {}

	private:
		std::vector<Component*> Components;
		Component* focusedComponent_;
		Component* hoveringComponent_;

	};

}
