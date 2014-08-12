#include "Fonts.h"
#include "GameData.h"

#include <SFML/Graphics.hpp>

using namespace game;

sf::Font Fonts::font_COMMS = sf::Font();
//sf::Font Fonts::font_SFOld = sf::Font();
sf::Font Fonts::font_BOGSTD = sf::Font();

void Fonts::StaticInit()
{
	font_COMMS.LoadFromFile(GD_RES_FONT_COMMS);
	//font_SFOld.LoadFromFile("res/SFOldRepublic.ttf");
	font_BOGSTD.LoadFromFile(GD_RES_FONT_BOGSTAND);
}
