#include <iostream>

#include <sys/sys.h>
#include <game/Renderer.h>
#include <game/GameState.h>

#include <SFML/Graphics.hpp>

#include "game/GameData.h"
#include "userdefs.h"

#include <windows.h>

extern bool BatterySaver = false;

int main()
{
	//Initialize subsystems
	sys::Engine::CreateSingleton();
	game::Renderer::CreateSingleton();
	game::Renderer::Ref().Create(SCR_WIDTH, SCR_HEIGHT, game::GD_WINDOWTITLE, sf::Style::Default, 4);
	sys::Engine::Ref().Begin(SCR_WIDTH, SCR_HEIGHT);

	//Create game instance and send to engine
	sys::Engine::Ref().SetState(new game::GameState());
	
	//Game loop
	sys::HRTimer timer; timer.Start();
	sys::HRTimer dttimer; dttimer.Start();
	double dt = 0.0;
	bool draw = false;
	double batteryTimeElapsed = 0.0;
	while(sys::Engine::Ref().Running())
	{
		draw = (timer.ElapsedMS() >= DRAW_TIME);
		if(draw)
		{
			timer.RemoveMilliSeconds(DRAW_TIME);

			sf::Event e;
			while(game::Renderer::Ref()->GetEvent(e)) switch(e.Type)
			{
				case sf::Event::Closed: sys::Engine::Ref().onClose(); sys::Engine::Ref().Exit(); break;
				case sf::Event::KeyPressed: sys::Engine::Ref().onKeyPress(e.Key.Code, e.Key.Shift, e.Key.Control, e.Key.Alt); break;
				case sf::Event::KeyReleased: sys::Engine::Ref().onKeyRelease(e.Key.Code, e.Key.Shift, e.Key.Control, e.Key.Alt); break;
				case sf::Event::MouseButtonPressed: sys::Engine::Ref().onMouseClick(e.MouseButton.X, e.MouseButton.Y, e.MouseButton.Button); break;
				case sf::Event::MouseButtonReleased: sys::Engine::Ref().onMouseUnclick(e.MouseButton.X, e.MouseButton.Y, e.MouseButton.Button); break;
				case sf::Event::MouseMoved: sys::Engine::Ref().onMouseMove(e.MouseMove.X, e.MouseMove.Y); break;
				case sf::Event::MouseWheelMoved: sys::Engine::Ref().onMouseWheel(e.MouseWheel.X, e.MouseWheel.Y, e.MouseWheel.Delta); break;
				case sf::Event::Resized: sys::Engine::Ref().onResize(e.Size.Width, e.Size.Height); break;
				case sf::Event::TextEntered: sys::Engine::Ref().onText(e.Text.Unicode); break;
			}
		}

		if(sys::Engine::Ref().Running())
		{
			dt = dttimer.ElapsedMS();
			dttimer.Reset();

			sys::Engine::Ref().Tick(static_cast<float>(dt));

			if (BatterySaver)
			{
				batteryTimeElapsed += SLEEP_TIME;
				Sleep(SLEEP_TIME);
			}
			else
			{
				if (batteryTimeElapsed > 0.0)
				{
					//timer.RemoveMilliSeconds(batteryTimeElapsed);
					batteryTimeElapsed = 0.0;
				}
			}
		}

		if(draw && sys::Engine::Ref().Running())
		{
			//game::Renderer::Ref()->Clear();
			sys::Engine::Ref().Draw();
			game::Renderer::Ref()->Display();
		}
	}
	
	
	//Kill resources
	sys::Engine::KillSingleton();
	//game::Renderer::KillSingleton(); // Segfault? Seems to be a problem with SFML windows when destructing.
	return 0;
}
