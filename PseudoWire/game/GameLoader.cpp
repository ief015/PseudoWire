#include "GameLoader.h"
#include <sys/sys.h>
#include "GameData.h"

#include <sys/io/io.h>
#include <SFML/Graphics.hpp>

#include "tinyxml/tinyxml.h"

using namespace game;

void AddonReader::RefreshAddons()
{
	//get all addons in addons folder
	sys::io::Path path_addons(GD_PATH_ADDONS);
	std::vector<sys::io::Path> addon_dirlist;
	path_addons.GetAllDirs(addon_dirlist);

	//temp holder
	const char* tmp = NULL;

	Packages.clear();
	for(int i = 0, idskips = 0; i < addon_dirlist.size(); ++i)
	{
		TiXmlDocument doc;
		doc.LoadFile((std::string(addon_dirlist[i].GetFullPath()) + GD_PATHSEP + GD_ADDONINFO_FILENAME).c_str());

		if(!doc.Error())
		{
			const TiXmlElement* n_package = doc.FirstChildElement(GD_XML_ADDONINFO_ROOT);
			if(n_package) do
			{
				//we got a package
				AddonPackage* package = new AddonPackage();
				
				if((tmp = n_package->Attribute( GD_XML_ADDONINFO_ROOT_ATT_NAME ))==NULL)
				{ delete package; continue; }
				package->name = tmp;

				if((tmp = n_package->Attribute( GD_XML_ADDONINFO_ROOT_ATT_AUTHOR ))==NULL)
				{ delete package; continue; }
				package->author = tmp;

				if((tmp = n_package->Attribute( GD_XML_ADDONINFO_ROOT_ATT_VERSION ))==NULL)
				{ delete package; continue; }
				package->version = tmp;

				package->id = i - idskips;

				const TiXmlElement* n_category = n_package->FirstChildElement(GD_XML_ADDONINFO_CATEGORY);

				if(n_category) do
				{
					//we got a category
					AddonCategory* category = new AddonCategory();

					if((tmp = n_category->Attribute(GD_XML_ADDONINFO_CATEGORY_ATT_NAME))==NULL)
					{ delete category; continue; }
					category->name = tmp;

					if((tmp = n_category->Attribute(GD_XML_ADDONINFO_CATEGORY_ATT_PATH))==NULL)
					{ delete category; continue; }
					category->path = tmp;

					const TiXmlElement* n_item = n_category->FirstChildElement(GD_XML_ADDONINFO_ITEM);
				
					if(n_item) do
					{
						//we got an item
						AddonItem* item = new AddonItem();

						if((tmp = n_item->Attribute(GD_XML_ADDONINFO_ITEM_ATT_NAME))==NULL)
						{ delete item; continue; }
						item->name = tmp;

						if((tmp = n_item->Attribute(GD_XML_ADDONINFO_ITEM_ATT_PATH))==NULL)
						{ delete item; continue; }
						item->path = tmp;

						if((tmp = n_item->GetText())!=NULL)
							item->description = tmp;

						item->p_category = category;
						category->items.push_back( item );
					} while((n_item = n_item->NextSiblingElement(GD_XML_ADDONINFO_ITEM)) != NULL);
				
					category->p_package = package;
					package->categories.push_back( category );

				} while((n_category = n_category->NextSiblingElement(GD_XML_ADDONINFO_CATEGORY)) != NULL);

				package->path = addon_dirlist[i].GetName();
				Packages.push_back( package );

				//apply links
				/*for(int c = 0; c < Packages[Packages.size()-1].categories.size(); ++c)
				{
					AddonPackage& p = Packages[Packages.size()-1];
					AddonCategory& p_c = p.categories[c];
					
					for(int item = 0; item < p_c.items.size(); ++item)
					{
						p_c.items[item].p_category = &p_c;
					}

					p_c.p_package = &p;
				}*/

			} while((n_package = n_package->NextSiblingElement(GD_XML_ADDONINFO_ROOT)) != NULL);
		}
		else ++idskips;
	}
	
	// Create a list of referencable images
	for(int i = 0; i < Images.size(); ++i)
		delete Images[i];
	Images.clear();

	for(int addon = 0; addon < Packages.size(); ++addon)
	{
		std::vector<sys::io::File> imgs;

		sys::String str = GD_NULLSTR;
		str << GD_PATH_ADDONS << GD_PATHSEP << Packages[addon]->path.c_str() << GD_PATHSEP << GD_PATH_ADDONS_IMG;

		if(sys::io::Exists(str.C_Str()))
		{
			sys::io::Path(str.C_Str()).GetAllFiles(imgs);

			for(int i = 0; i < imgs.size(); ++i)
			{
				ImageItem* img = new ImageItem();
				img->img = new sf::Image();
				img->filepath = imgs[i].GetFullPath();
				img->img->LoadFromFile(img->filepath);
				img->ref = imgs[i].GetFullName();
				img->addon_idx = addon;
				Images.push_back(img);
			}
		}
	}

	// Create a list of referencable fonts
	for(int i = 0; i < Fonts.size(); ++i)
		delete Fonts[i];
	Fonts.clear();

	for(int addon = 0; addon < Packages.size(); ++addon)
	{
		std::vector<sys::io::File> fonts;

		sys::String str = GD_NULLSTR;
		str << GD_PATH_ADDONS << GD_PATHSEP << Packages[addon]->path.c_str() << GD_PATHSEP << GD_PATH_ADDONS_FNT;

		if(sys::io::Exists(str.C_Str()))
		{
			sys::io::Path(str.C_Str()).GetAllFiles(fonts);

			for(int i = 0; i < fonts.size(); ++i)
			{
				FontItem* font = new FontItem();
				font->fnt = new sf::Font();
				font->filepath = fonts[i].GetFullPath();
				font->fnt->LoadFromFile(font->filepath);
				font->ref = fonts[i].GetFullName();
				font->addon_idx = addon;
				Fonts.push_back(font);
			}
		}
	}
}

sf::Image* const AddonReader::GetImage(const char* file, int addonid)
{
	if(addonid == -1)
	{
		//check all
		for(int i = 0; i < Images.size(); ++i)
		{
			if(Images[i]->ref == file)
			{
				return Images[i]->img;
			}
		}
		return NULL;
	}
	else
	{
		//check for images in addon first
		for(int i = 0; i < Images.size(); ++i)
		{
			if(Images[i]->addon_idx == addonid) //addon only
			if(Images[i]->ref == file)
			{
				return Images[i]->img;
			}
		}

		//check other addons
		for(int i = 0; i < Images.size(); ++i)
		{
			if(Images[i]->addon_idx != addonid) //already checked
			if(Images[i]->ref == file)
			{
				return Images[i]->img;
			}
		}
	}
	return NULL;
}

sf::Font* const AddonReader::GetFont(const char* file, int addonid)
{
	if(addonid == -1)
	{
		//check all
		for(int i = 0; i < Fonts.size(); ++i)
		{
			if(Fonts[i]->ref == file)
			{
				return Fonts[i]->fnt;
			}
		}
	}
	else
	{
		//check for fonts in addon first
		for(int i = 0; i < Fonts.size(); ++i)
		{
			if(Fonts[i]->addon_idx == addonid) //addon only
			if(Fonts[i]->ref == file)
			{
				return Fonts[i]->fnt;
			}
		}

		//check other addons
		for(int i = 0; i < Fonts.size(); ++i)
		{
			if(Fonts[i]->addon_idx != addonid) //already checked
			if(Fonts[i]->ref == file)
			{
				return Fonts[i]->fnt;
			}
		}
	}

	return NULL;
}
