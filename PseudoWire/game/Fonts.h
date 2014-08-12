#pragma once
#include <SFML/Graphics.hpp>

namespace game
{
class Fonts
{
public:
	static void StaticInit();

	static sf::Font font_COMMS;
	//static sf::Font font_SFOld;
	static sf::Font font_BOGSTD;

};
}
