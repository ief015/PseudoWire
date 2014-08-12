#include "Platform.h"
#include "Engine.h"
#include "State.h"

using namespace sys;

Engine::Engine()
:
state_(NULL),
statequeued_(NULL),
mousex_(0),
mousey_(0),
mousexp_(0),
mouseyp_(0)
{
}

Engine::~Engine()
{
	if(state_ != NULL)
		delete state_;
}

void Engine::Begin(s32 width, s32 height)
{
	//engine is now ready
	running_ = true;

	width_ = width;
	height_ = height;
}

void Engine::Exit()
{
	running_ = false;
}

void Engine::SetState(State * state)
{
	if(state_) //queue if currently in a state
		statequeued_ = state;
	else
	{
		state_ = state;
		if(state_)
			state_->DoInitialized();
	}
}

void Engine::SetSize(s32 width, s32 height)
{
	width_ = width;
	height_ = height;
}

void Engine::Tick(f32 dt)
{
	if(state_ != NULL)
		state_->DoTick(dt);

	if(statequeued_ != NULL)
	{
		if(state_ != NULL)
		{
			state_->DoExit();
			delete state_;
			state_ = NULL;
		}
		state_ = statequeued_;
		statequeued_ = NULL;

		if(state_ != NULL)
			state_->DoInitialized();
	}
}

void Engine::Draw()
{
	if(state_)
		state_->DoDraw();
}

void Engine::onKeyPress(s32 key, bool shift, bool ctrl, bool alt)
{
	if(state_)
		state_->DoKeyPress(key, shift, ctrl, alt);
}

void Engine::onKeyRelease(s32 key, bool shift, bool ctrl, bool alt)
{
	if(state_)
		state_->DoKeyRelease(key, shift, ctrl, alt);
}

void Engine::onText(s32 key)
{
	if(state_)
		state_->DoText(key);
}

void Engine::onMouseClick(s32 x, s32 y, u32 button)
{
	if(state_)
		state_->DoMouseDown(x, y, button);
}

void Engine::onMouseUnclick(s32 x, s32 y, u32 button)
{
	if(state_)
		state_->DoMouseUp(x, y, button);
}

void Engine::onMouseMove(s32 x, s32 y)
{
	mousex_ = x;
	mousey_ = y;
	if(state_)
		state_->DoMouseMove(x, y, mousex_ - mousexp_, mousey_ - mouseyp_);
	mousexp_ = x;
	mouseyp_ = y;
}

void Engine::onMouseWheel(s32 x, s32 y, s32 delta)
{
	if(state_)
		state_->DoMouseWheel(x, y, delta);
}

void Engine::onResize(s32 newWidth, s32 newHeight)
{
	SetSize(newWidth, newHeight);

	if(state_)
		state_->DoResize(newWidth, newHeight);
}

void Engine::onClose()
{
	if(state_)
		state_->DoExit();
}
