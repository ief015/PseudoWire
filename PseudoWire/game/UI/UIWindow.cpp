#include "UIWindow.h"

#include <sys/sys.h>
#include <SFML/Graphics.hpp>

#include "../GameData.h"

using namespace game;
using namespace game::ui;

const int UIWindow_Static::TITLEBAR_HEIGHT = 18;

sf::Image UIWindow_Static::img_chkbox_off;
sf::Image UIWindow_Static::img_chkbox_on;

sf::Image UIWindow_Static::img_radio_off;
sf::Image UIWindow_Static::img_radio_on;

sf::Image UIWindow_Static::img_accept;
sf::Image UIWindow_Static::img_decline;

void UIWindow_Static::StaticInit()
{
	img_chkbox_off.LoadFromFile(GD_RES_CHECKBOX);
	img_chkbox_on.LoadFromFile(GD_RES_CHECKBOX_CHECKED);
	img_radio_off.LoadFromFile(GD_RES_RADIO);
	img_radio_on.LoadFromFile(GD_RES_RADIO_CHECKED);
	img_accept.LoadFromFile(GD_RES_ACCEPT);
	img_decline.LoadFromFile(GD_RES_CANCEL);
}
