/*
	Neutrino-GUI  -   DBoxII-Project

	$id: service_setup.cpp 2015.12.22 17:19:30 mohousch $
	
	Copyright (C) 2001 Steffen Hehn 'McClean'
	and some other guys
	Homepage: http://dbox.cyberphoria.org/

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
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/ 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h> 

#include <global.h>
#include <neutrino.h>

#include <configfile.h>

#include <system/debug.h>
#include <system/settings.h>
#include <system/flashtool.h>

#include <gui/widget/hintbox.h>

#include <gui/service_menu.h>

#include <gui/scan_setup.h>
#include <gui/widget/icons.h>
#include <gui/update.h>
#include <gui/cam_menu.h>
#include <gui/imageinfo.h>

#include <gui/bedit/bouqueteditor_bouquets.h>


#if !defined (PLATFORM_COOLSTREAM)
#if defined (ENABLE_CI)
extern CCAMMenuHandler * g_CamHandler;		// defined neutrino.cpp
#endif
#endif

extern int FrontendCount;			// defined in zapit.cpp

//
CServiceSetup::CServiceSetup()
{
}

CServiceSetup::~CServiceSetup()
{
}

int CServiceSetup::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CServiceSetup::exec: actionKey:%s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "reloadchannels")
	{
		HintBox(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_SERVICEMENU_RELOAD_HINT));

		g_Zapit->reinitChannels();
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

// showmenu
void CServiceSetup::showMenu(void)
{
	dprintf(DEBUG_NORMAL, "CServiceSetup::showMenu\n");
	
	int shortcutService = 1;
	
	CMenuWidget * service = new CMenuWidget(LOCALE_SERVICEMENU_HEAD, NEUTRINO_ICON_UPDATE);

	service->setWidgetMode(MODE_MENU);
	service->setWidgetType(WIDGET_TYPE_CLASSIC);
	service->enableWidgetChange();
	service->enableShrinkMenu();
	service->setMenuPosition(MENU_POSITION_LEFT);
	service->enablePaintFootInfo();
	service->enablePaintDate();
	
	// tuner/scan setup
	service->addItem(new CMenuForwarder(LOCALE_SERVICEMENU_SCANTS, true, NULL, new CTunerSetup(), NULL, RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_SCANSETTINGS, LOCALE_HELPTEXT_SCANSETUP));

	// reload Channels
	service->addItem(new CMenuForwarder(LOCALE_SERVICEMENU_RELOAD, true, NULL, this, "reloadchannels", RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_RELOADCHANNELS, LOCALE_HELPTEXT_RELOADCHANNELS));

	// Bouquets Editor
	service->addItem(new CMenuForwarder(LOCALE_BOUQUETEDITOR_NAME, true, NULL, new CBEBouquetWidget(), NULL, RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_BOUQUETSEDITOR, LOCALE_HELPTEXT_BOUQUETSEDITOR));
	
	// CI Cam 	
#if defined (ENABLE_CI)
	service->addItem(new CMenuForwarder(LOCALE_CAM_SETTINGS, true, NULL, g_CamHandler, NULL, CRCInput::convertDigitToKey(shortcutService++), NULL, NEUTRINO_ICON_MENUITEM_CICAM, LOCALE_HELPTEXT_CAM));
#endif
	
	// image info
	service->addItem(new CMenuForwarder(LOCALE_SERVICEMENU_IMAGEINFO,  true, NULL, new CImageInfo(), NULL, RC_info, NEUTRINO_ICON_BUTTON_HELP, NEUTRINO_ICON_MENUITEM_IMAGEINFO, LOCALE_HELPTEXT_IMAGEINFO), false);
	
	// software update
	service->addItem(new CMenuForwarder(LOCALE_SERVICEMENU_UPDATE, true, NULL, new CUpdateSettings(), NULL, RC_blue, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MENUITEM_SOFTUPDATE, LOCALE_HELPTEXT_SOFTWAREUPDATE));

	service->integratePlugins(CPlugins::I_TYPE_SERVICE, shortcutService++);
	
	service->exec(NULL, "");
	service->hide();
	delete service;
	service = NULL;
}


