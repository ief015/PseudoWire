#pragma once
#include <sys/sys.h>

#include <SFML/Graphics.hpp>

namespace game
{
class AddonPackage;
class AddonCategory;
class AddonItem;

class AddonItem
{
public:
	AddonItem()
		: p_category(NULL)
		, name("")
		, path("")
		, description("")
	{}
	~AddonItem()
	{}

	AddonCategory* p_category;

	std::string name;
	std::string path;
	std::string description;
};

class AddonCategory
{
public:
	AddonCategory()
		: p_package(NULL)
		, name("")
		, path("")
	{}

	~AddonCategory()
	{
		for(unsigned i = 0; i < items.size(); ++i)
			delete items[i];
	}

	AddonPackage* p_package;
	std::vector<AddonItem*> items;

	std::string name;
	std::string path;
};

class AddonPackage
{
public:
	AddonPackage()
		: name("")
		, author("")
		, version("1.0.0")
		, path("")
		, id(-1)
	{}

	~AddonPackage()
	{
		for(unsigned i = 0; i < categories.size(); ++i)
			delete categories[i];
	}

	std::vector<AddonCategory*> categories;

	std::string name;
	std::string author;
	std::string version;
	std::string path;
	int id;
};

class ImageItem
{
public:
	ImageItem()
	{
		img = NULL;
	}

	~ImageItem()
	{
		if(img) delete img;
	}

	std::string filepath;
	int addon_idx;
	std::string ref;
	sf::Image* img;
};

class FontItem
{
public:
	FontItem()
	{
		fnt = NULL;
	}
	~FontItem()
	{
		if(fnt) delete fnt;
	}

	std::string filepath;
	int addon_idx;
	std::string ref;
	sf::Font* fnt;
};

class AddonReader : public sys::Singleton<AddonReader>
{
public:

	//Refresh packages
	void RefreshAddons();

	sf::Image* const GetImage(const char* file, int addonid = -1);
	sf::Font* const GetFont(const char* file, int addonid = -1);

	std::vector<AddonPackage*> Packages;
	std::vector<ImageItem*> Images;
	std::vector<FontItem*> Fonts;

	~AddonReader()
	{
		for(size_t i = 0; i < Packages.size(); ++i)
			delete Packages[i];
		for(size_t i = 0; i < Images.size(); ++i)
			delete Images[i];
		for(size_t i = 0; i < Fonts.size(); ++i)
			delete Fonts[i];
	}
};

}
