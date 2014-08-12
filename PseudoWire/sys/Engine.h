#pragma once

#include "Platform.h"
#include "Singleton.h"
#include "State.h"

namespace sys
{
	class State;

	/* class Engine
	 * 
	 * Controls the User Interface.
	 * Send user inputs to the Engine and the appropriate controls and components will interact.
	 */
	class Engine : public Singleton<Engine>
	{
	public:
		Engine();
		~Engine();

		void onMouseMove(s32 x, s32 y);
		void onMouseClick(s32 x, s32 y, u32 button);
		void onMouseUnclick(s32 x, s32 y, u32 button);
		void onMouseWheel(s32 x, s32 y, s32 delta);
		void onKeyPress(s32 key, bool shift, bool ctrl, bool alt);
		void onKeyRelease(s32 key, bool shift, bool ctrl, bool alt);
		void onText(s32 key);
		void onResize(s32 newWidth, s32 newHeight);
		void onClose();

		void Begin(s32 width, s32 height);
		inline bool Running() const { return running_; }
		void Exit();

		void Tick(f32 dt);
		void Draw();

		inline s32 GetMouseX() const { return mousex_; }
		inline s32 GetMouseY() const { return mousey_; }
		inline s32 GetWidth() const { return width_; }
		inline s32 GetHeight() const { return height_; }

		inline void SetSize(s32 width, s32 height);
		
		void SetState(State* state);
		inline State* const GetState() const { return state_; }
	private:
		State* statequeued_;
		State* state_;

		bool running_;
		
		s32 mousex_;
		s32 mousey_;
		s32 mousexp_;
		s32 mouseyp_;
		s32 width_;
		s32 height_;
	};

}
