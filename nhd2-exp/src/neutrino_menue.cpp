/*
	Neutrino-GUI  -   DBoxII-Project
	
	$Id: neutrino_menu.cpp 2013/10/12 11:23:30 mohousch Exp $

	Copyright (C) 2001 Steffen Hehn 'McClean' and some other guys
	Homepage: http://dbox.cyberphoria.org/

	Kommentar:

	Diese GUI wurde von Grund auf neu programmiert und sollte nun vom
	Aufbau und auch den Ausbaumoeglichkeiten gut aussehen. Neutrino basiert
	auf der Client-Server Idee, diese GUI ist also von der direkten DBox-
	Steuerung getrennt. Diese wird dann von Daemons uebernommen.


	License: GPL

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>

#include <string>

#include <global.h>
#include <neutrino.h>

#include <daemonc/remotecontrol.h>

#include <gui/widget/menue.h>
#include <gui/widget/textbox.h>
#include <gui/widget/icons.h>
#include <gui/mediaplayer.h>
#include <gui/service_menu.h>
#include <gui/main_setup.h>
#include <gui/timerlist.h>
#include <gui/sleeptimer.h>
#include <gui/power_menu.h>
#include <gui/dboxinfo.h>

#include <gui/audio_select.h>
#include <gui/epgplus.h>
#include <gui/streaminfo2.h>
#include <gui/movieplayer.h>
#include <gui/pluginlist.h>

#include <system/debug.h>

#include <driver/encoding.h>


extern CPlugins * g_PluginList;    /* neutrino.cpp */

// mainmenu
void CNeutrinoApp::mainMenu(void)
{
	int shortcut = 1;

	dprintf(DEBUG_NORMAL, "CNeutrinoApp::mainMenu:\n");
	
	if ( g_settings.use_skin && (CNeutrinoApp::getInstance()->skin_exists("mainmenu")))
		CNeutrinoApp::getInstance()->startSkin("mainmenu");
	else	
	{
		CMenuWidget * nMenu = new CMenuWidget(LOCALE_MAINMENU_HEAD, NEUTRINO_ICON_BUTTON_SETUP);
		
		nMenu->setWidgetMode(MODE_MENU);
		nMenu->setWidgetType(WIDGET_TYPE_CLASSIC);
		nMenu->enableShrinkMenu();
		nMenu->setMenuPosition(MENU_POSITION_LEFT);
		nMenu->enablePaintDate();
		nMenu->enablePaintFootInfo();
		  
		// tv modus
		nMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_TVMODE, true, NULL, this, "tv", RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_TV, LOCALE_HELPTEXT_TVMODE), true);

		// radio modus
		nMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_RADIOMODE, true, NULL, this, "radio", RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_RADIO, LOCALE_HELPTEXT_RADIOMODE));	
		
		// webtv
		nMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_WEBTVMODE, true, NULL, this, "webtv", RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_WEBTV, LOCALE_HELPTEXT_WEBTVMODE));
		
	#if defined (ENABLE_SCART)
		// scart
		nMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_SCARTMODE, true, NULL, this, "scart", RC_blue, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MENUITEM_SCART, LOCALE_HELPTEXT_SCART));
	#endif

		// mediaplayer
		nMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_MEDIAPLAYER, true, NULL, new CMediaPlayerMenu(), NULL, CRCInput::convertDigitToKey(shortcut++), NULL, NEUTRINO_ICON_MENUITEM_MEDIAPLAYER, LOCALE_HELPTEXT_MEDIAPLAYER));
		
		// main setting menu
		nMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_SETTINGS, true, NULL, new CMainSetup(), NULL, CRCInput::convertDigitToKey(shortcut++), NULL, NEUTRINO_ICON_MENUITEM_SETTINGS, LOCALE_HELPTEXT_MAINSETTINGS));

		// service
		nMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_SERVICE, true, NULL, new CServiceSetup(), NULL, CRCInput::convertDigitToKey(shortcut++), NULL, NEUTRINO_ICON_MENUITEM_SERVICE, LOCALE_HELPTEXT_SERVICE));
		
		// features
		nMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_FEATURES, true, NULL, this, "features", CRCInput::convertDigitToKey(shortcut++), NULL, NEUTRINO_ICON_MENUITEM_FEATURES, LOCALE_HELPTEXT_FEATURES ));

		// power menu
		nMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_POWERMENU, true, NULL, new CPowerMenu(), NULL, RC_standby, NEUTRINO_ICON_BUTTON_POWER, NEUTRINO_ICON_MENUITEM_POWERMENU, LOCALE_HELPTEXT_POWERMENU));

		//box info
		nMenu->addItem( new CMenuForwarder(LOCALE_MESSAGEBOX_INFO, true, NULL, new CInfo(), NULL, RC_info, NEUTRINO_ICON_BUTTON_HELP, NEUTRINO_ICON_MENUITEM_BOXINFO, LOCALE_HELPTEXT_BOXINFO));

		nMenu->exec(NULL, "");
		delete nMenu;
		nMenu = NULL;
	}	
}

// User menu
// This is just a quick helper for the usermenu only. I already made it a class for future use.
#define BUTTONMAX 4

const neutrino_msg_t key_helper_msg_def[BUTTONMAX] = {
	RC_red,
	RC_green,
	RC_yellow,
	RC_blue
};

const char * key_helper_icon_def[BUTTONMAX]={
	NEUTRINO_ICON_BUTTON_RED, 
	NEUTRINO_ICON_BUTTON_GREEN, 
	NEUTRINO_ICON_BUTTON_YELLOW, 
	NEUTRINO_ICON_BUTTON_BLUE
};

class CKeyHelper
{
        private:
                int number_key;
                bool color_key_used[BUTTONMAX];
        public:
                CKeyHelper(){reset();};
                void reset(void)
                {
                        number_key = 1;
                        for(int i= 0; i < BUTTONMAX; i++ )
                                color_key_used[i] = false;
                };

                /* Returns the next available button, to be used in menu as 'direct' keys. Appropriate
                 * definitions are returnd in msp and icon
                 * A color button could be requested as prefered button (other buttons are not supported yet).
                 * If the appropriate button is already in used, the next number_key button is returned instead
                 * (first 1-9 and than 0). */
                bool get(neutrino_msg_t* msg, const char** icon, neutrino_msg_t prefered_key = RC_nokey)
                {
                        bool result = false;
                        int button = -1;
                        if(prefered_key == RC_red)
                                button = 0;
                        if(prefered_key == RC_green)
                                button = 1;
                        if(prefered_key == RC_yellow)
                                button = 2;
                        if(prefered_key == RC_blue)
                                button = 3;

                        *msg = RC_nokey;
                        *icon = "";
                        if(button >= 0 && button < BUTTONMAX)
                        {
				// try to get color button
                                if( color_key_used[button] == false)
                                {
                                        color_key_used[button] = true;
                                        *msg = key_helper_msg_def[button];
                                        *icon = key_helper_icon_def[button];
                                        result = true;
                                }
                        }

                        if( result == false && number_key < 10) // no key defined yet, at least try to get a numbered key
                        {
                                // there is still a available number_key
                                *msg = CRCInput::convertDigitToKey(number_key);
                                *icon = "";
                                if(number_key == 9)
                                        number_key = 0;
                                else if(number_key == 0)
                                        number_key = 10;
                                else
                                        number_key++;

                                result = true;
                        }

                        return (result);
                };
};

bool CNeutrinoApp::showUserMenu(int button)
{
	dprintf(DEBUG_NORMAL, "CNeutrinoApp::showUserMenu\n");
	
	if ( g_settings.use_skin && (CNeutrinoApp::getInstance()->skin_exists("mainmenu")))
		CNeutrinoApp::getInstance()->startSkin("usermenu");
	else	
	{
		if(button < 0 || button >= SNeutrinoSettings::BUTTON_MAX)
		        return false;

		CMenuItem * menu_item = NULL;
		CKeyHelper keyhelper;
		neutrino_msg_t key = RC_nokey;
		const char * icon = NULL;

		int menu_items = 0;
		int menu_prev = -1;
		static int selected[SNeutrinoSettings::BUTTON_MAX] = {
			-1,
	#if defined (ENABLE_FUNCTIONKEYS) //FIXME:???
			-1,
			-1,
			-1,
			-1,
	#endif		
		};

		std::string txt = g_settings.usermenu_text[button];

		if( button == SNeutrinoSettings::BUTTON_BLUE) 
		{
		        if( txt.empty() )
		                txt = g_Locale->getText(LOCALE_MAINMENU_FEATURES);
		}

		// other function keys

		CMenuWidget * menu = new CMenuWidget(txt.c_str(), NEUTRINO_ICON_FEATURES);
		if (menu == NULL)
		        return 0;

		//
		menu->setWidgetMode(MODE_MENU);
		menu->setWidgetType(WIDGET_TYPE_CLASSIC);
		menu->enableShrinkMenu();
		menu->setMenuPosition(MENU_POSITION_LEFT);
		menu->enablePaintFootInfo();

		menu->addKey(RC_blue, this, "plugins");

		// go through any postition number
		for(int pos = 0; pos < SNeutrinoSettings::ITEM_MAX ; pos++) 
		{
			// now compare pos with the position of any item. Add this item if position is the same
			switch(g_settings.usermenu[button][pos]) 
			{
				case SNeutrinoSettings::ITEM_NONE:
				{
					continue;
				}

				// timerlist
			       case SNeutrinoSettings::ITEM_TIMERLIST:
				        menu_items++;
				        menu_prev = SNeutrinoSettings::ITEM_TIMERLIST;
				        keyhelper.get(&key, &icon, RC_yellow);
					menu_item = new CMenuForwarder(LOCALE_TIMERLIST_NAME, true, NULL, new CTimerList, "-1", key, icon, NEUTRINO_ICON_MENUITEM_TIMERLIST, LOCALE_HELPTEXT_TIMERLIST);
				        menu->addItem(menu_item, false);
				        break;

				// rclock
			       case SNeutrinoSettings::ITEM_REMOTE:
				        menu_items++;
				        menu_prev = SNeutrinoSettings::ITEM_REMOTE;
				        keyhelper.get(&key, &icon);
					menu_item = new CMenuForwarder(LOCALE_RCLOCK_MENUEADD, true, NULL, this->rcLock, "-1", key, icon, NEUTRINO_ICON_MENUITEM_PARENTALLOCKSETTINGS);
				        menu->addItem(menu_item, false);
				        break;
				
				// vtxt	
				case SNeutrinoSettings::ITEM_VTXT:
					if (CNeutrinoApp::getInstance()->getMode() != NeutrinoMessages::mode_webtv)
					{
						menu_items++;
						menu_prev = SNeutrinoSettings::ITEM_VTXT;
						keyhelper.get(&key, &icon);
						menu_item = new CMenuForwarder(LOCALE_USERMENU_ITEM_VTXT, true, NULL, new CTuxtxtChangeExec, "-1", key, icon, NEUTRINO_ICON_MENUITEM_VTXT);
						menu->addItem(menu_item, false);
					}
					break;	
				
				// plugins
				case SNeutrinoSettings::ITEM_PLUGIN:
				{
					menu_item++;
					menu_prev = SNeutrinoSettings::ITEM_PLUGIN;
					keyhelper.get(&key, &icon, RC_blue);
					menu_item = new CMenuForwarder(LOCALE_USERMENU_ITEM_PLUGINS, true, NULL, new CPluginList(), "-1", key, icon, NEUTRINO_ICON_MENUITEM_PLUGIN, LOCALE_HELPTEXT_FEATURES);
					menu->addItem(menu_item, false);
				}
				break;

				default:
					dprintf(DEBUG_NORMAL, "[neutrino] WARNING! menu wrong item!!\n");
				        break;
			}
		}

		// integragte user plugins
		if( button == SNeutrinoSettings::BUTTON_BLUE) 
		{
			keyhelper.get(&key, &icon);
			menu->integratePlugins(CPlugins::I_TYPE_USER, key);
		}
		
		// start directly plugins if there is no items.
		if(menu && menu->getItemsCount() == 0)
		{
			CPluginList * pluginList = new CPluginList();
			pluginList->exec(NULL, "");
			delete pluginList;
			pluginList = NULL;
		}
		else
		{
			menu->setSelected(selected[button]);
		        menu->exec(NULL, "");
			selected[button] = menu->getSelected();
		}

		if(menu)
			delete menu;
	}

	return 0;
}

