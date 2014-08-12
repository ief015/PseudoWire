#pragma once

#include <sys/sys.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace game
{

class Renderer : public sys::Singleton<Renderer>
{
private:
	sf::RenderWindow rw;

public:
	Renderer()
	{
	}

	~Renderer()
	{
	}
	
	void Create(unsigned int width, unsigned int height, const char* title, int style = sf::Style::Default, unsigned int AA = 1)
	{
		rw.Create(sf::VideoMode(width, height, 32), title, style, sf::ContextSettings(24, 8, AA));
	}
	
	void Close()
	{
		rw.Close();
	}

	inline sf::RenderWindow* const GetRW()
	{
		return &rw;
	}
	
	inline sf::RenderWindow* const operator -> ()
	{
		return &rw;
	}
};

}
