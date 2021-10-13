/*
	Neutrino-GUI  -   DBoxII-Project
	
	$Id: hintbox.cpp 2013/10/12 mohousch Exp $

	Copyright (C) 2001 Steffen Hehn 'McClean'
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
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <global.h>
#include <neutrino.h>

#include <system/debug.h>
#include <system/settings.h>

#include <gui/widget/hintbox.h>


CHintBox::CHintBox(const neutrino_locale_t Caption, const char * const Text, const int Width, const char * const Icon)
{
	char * begin;
	char * pos;
	int    nw;

	message = strdup(Text);

	cFrameBox.iWidth = Width;

	cFrameBoxTitle.iHeight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight();
	cFrameBoxItem.iHeight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight();
	cFrameBox.iHeight = cFrameBoxTitle.iHeight + cFrameBoxItem.iHeight;

	caption = g_Locale->getText(Caption);

	begin = message;

	while (true)
	{
		cFrameBox.iHeight += cFrameBoxItem.iHeight;
		if (cFrameBox.iHeight > HINTBOX_MAX_HEIGHT)
			cFrameBox.iHeight -= cFrameBoxItem.iHeight;

		line.push_back(begin);

		pos = strchr(begin, '\n');
		if (pos != NULL)
		{
			*pos = 0;
			begin = pos + 1;
		}
		else
			break;
	}

	entries_per_page = ((cFrameBox.iHeight - cFrameBoxTitle.iHeight) / cFrameBoxItem.iHeight) - 1;
	current_page = 0;

	unsigned int additional_width;

	if (entries_per_page < line.size())
		additional_width = BORDER_LEFT + BORDER_RIGHT + SCROLLBAR_WIDTH;
	else
		additional_width = BORDER_LEFT + BORDER_RIGHT;

	if (Icon != NULL)
	{
		iconfile = Icon;
		additional_width += BORDER_LEFT + BORDER_RIGHT + 2*ICON_OFFSET;
	}
	else
		iconfile = "";

	nw = additional_width + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getRenderWidth(caption); // UTF-8

	if (nw > cFrameBox.iWidth)
		cFrameBox.iWidth = nw;

	for (std::vector<char *>::const_iterator it = line.begin(); it != line.end(); it++)
	{
		nw = additional_width + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(*it, true); // UTF-8
		if (nw > cFrameBox.iWidth)
		{
			cFrameBox.iWidth = nw;

			if(cFrameBox.iWidth > HINTBOX_MAX_WIDTH)
				cFrameBox.iWidth = HINTBOX_MAX_WIDTH;
		}
	}

	//
	cFrameBox.iX = CFrameBuffer::getInstance()->getScreenX() + ((CFrameBuffer::getInstance()->getScreenWidth() - cFrameBox.iWidth ) >> 1);
	cFrameBox.iY = CFrameBuffer::getInstance()->getScreenY() + ((CFrameBuffer::getInstance()->getScreenHeight() - cFrameBox.iHeight) >> 2);
	
	m_cBoxWindow.setPosition(cFrameBox.iX, cFrameBox.iY, cFrameBox.iWidth, cFrameBox.iHeight);
	m_cBoxWindow.enableSaveScreen();
	m_cBoxWindow.setColor(COL_MENUCONTENT_PLUS_0);
	m_cBoxWindow.enableShadow();
}

CHintBox::CHintBox(const char * Caption, const char * const Text, const int Width, const char * const Icon)
{
	char * begin;
	char * pos;
	int    nw;

	message = strdup(Text);

	cFrameBox.iWidth   = Width;

	cFrameBoxTitle.iHeight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight();
	cFrameBoxItem.iHeight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight();
	cFrameBox.iHeight = cFrameBoxTitle.iHeight + cFrameBoxItem.iHeight;

	caption = Caption;

	begin = message;

	while (true)
	{
		cFrameBox.iHeight += cFrameBoxItem.iHeight;
		if (cFrameBox.iHeight > HINTBOX_MAX_HEIGHT)
			cFrameBox.iHeight -= cFrameBoxItem.iHeight;

		line.push_back(begin);
		pos = strchr(begin, '\n');
		if (pos != NULL)
		{
			*pos = 0;
			begin = pos + 1;
		}
		else
			break;
	}
	entries_per_page = ((cFrameBox.iHeight - cFrameBoxTitle.iHeight) / cFrameBoxItem.iHeight) - 1;
	current_page = 0;

	unsigned int additional_width;

	if (entries_per_page < line.size())
		additional_width = BORDER_LEFT + BORDER_RIGHT + SCROLLBAR_WIDTH;
	else
		additional_width = BORDER_LEFT + BORDER_RIGHT;

	if (Icon != NULL)
	{
		iconfile = Icon;
		additional_width += BORDER_LEFT + BORDER_RIGHT + 2*ICON_OFFSET;
	}
	else
		iconfile = "";

	nw = additional_width + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getRenderWidth(caption); // UTF-8

	if (nw > cFrameBox.iWidth)
		cFrameBox.iWidth = nw;

	for (std::vector<char *>::const_iterator it = line.begin(); it != line.end(); it++)
	{
		nw = additional_width + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(*it, true); // UTF-8
		if (nw > cFrameBox.iWidth)
		{
			cFrameBox.iWidth = nw;

			if(cFrameBox.iWidth > HINTBOX_MAX_WIDTH)
				cFrameBox.iWidth = HINTBOX_MAX_WIDTH;		
		}
	}

	// Box
	cFrameBox.iX = CFrameBuffer::getInstance()->getScreenX() + ((CFrameBuffer::getInstance()->getScreenWidth() - cFrameBox.iWidth ) >> 1);
	cFrameBox.iY = CFrameBuffer::getInstance()->getScreenY() + ((CFrameBuffer::getInstance()->getScreenHeight() - cFrameBox.iHeight) >> 2);
	
	// Box
	m_cBoxWindow.setPosition(cFrameBox.iX, cFrameBox.iY, cFrameBox.iWidth, cFrameBox.iHeight);
	m_cBoxWindow.enableSaveScreen();
	m_cBoxWindow.setColor(COL_MENUCONTENT_PLUS_0);
	m_cBoxWindow.enableShadow();
}

CHintBox::~CHintBox(void)
{
	dprintf(DEBUG_NORMAL, "CHintBox::del:\n");

	free(message);
	//hide();
}

void CHintBox::paint(void)
{
	dprintf(DEBUG_NORMAL, "CHintBox::paint\n");

	refresh();
	
	CFrameBuffer::getInstance()->blit();
}

void CHintBox::refresh(void)
{
	//body
	m_cBoxWindow.paint();
	
	// title
	cFrameBoxTitle.iX = cFrameBox.iX + 1;
	cFrameBoxTitle.iY = cFrameBox.iY + 1;
	cFrameBoxTitle.iWidth = cFrameBox.iWidth - 2;

	CHeaders headers(cFrameBoxTitle, caption.c_str(), iconfile.c_str());
	headers.setCorner();
	headers.paint();

	// body text
	int count = entries_per_page;
	int ypos  = cFrameBoxTitle.iY + cFrameBoxTitle.iHeight + (cFrameBoxItem.iHeight >> 1);

	for (std::vector<char *>::const_iterator it = line.begin() + (entries_per_page * current_page); ((it != line.end()) && (count > 0)); it++, count--)
	{
		g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(cFrameBox.iX + BORDER_LEFT, (ypos += cFrameBoxItem.iHeight), cFrameBox.iWidth, *it, COL_MENUCONTENT, 0, true); 
	}

	// scrollBar #TODO
	if (entries_per_page < line.size())
	{
		ypos = cFrameBox.iY + cFrameBoxTitle.iHeight;

		scrollBar.paint(cFrameBox.iX + cFrameBox.iWidth - SCROLLBAR_WIDTH, ypos, entries_per_page*cFrameBoxItem.iHeight, (line.size() + entries_per_page - 1) / entries_per_page, current_page);
	}	
}

bool CHintBox::has_scrollbar(void)
{
	return (entries_per_page < line.size());
}

void CHintBox::scroll_up(void)
{
	if (current_page > 0)
	{
		current_page--;
		refresh();
	}
}

void CHintBox::scroll_down(void)
{
	if ((entries_per_page * (current_page + 1)) <= line.size())
	{
		current_page++;
		refresh();
	}
}

void CHintBox::hide(void)
{
	dprintf(DEBUG_NORMAL, "CHintBox::hide:\n");

	// reinit
	//cFrameBox.iX = CFrameBuffer::getInstance()->getScreenX() + ((CFrameBuffer::getInstance()->getScreenWidth() - cFrameBox.iWidth ) >> 1);
	//cFrameBox.iY = CFrameBuffer::getInstance()->getScreenY() + ((CFrameBuffer::getInstance()->getScreenHeight() - cFrameBox.iHeight) >> 2);

	m_cBoxWindow.hide();

	CFrameBuffer::getInstance()->blit();	
}

int CHintBox::exec(int timeout)
{
	int res = messages_return::none;

	neutrino_msg_t msg;
	neutrino_msg_data_t data;

	paint();

	if ( timeout == -1 )
		timeout = g_settings.timing[SNeutrinoSettings::TIMING_INFOBAR];

	uint64_t timeoutEnd = CRCInput::calcTimeoutEnd( timeout );

	while ( ! ( res & ( messages_return::cancel_info | messages_return::cancel_all ) ) )
	{
		g_RCInput->getMsgAbsoluteTimeout( &msg, &data, &timeoutEnd );

		if ((msg == RC_timeout) || (msg == RC_home) || (msg == RC_ok))
		{
			res = messages_return::cancel_info;
		}
		else if ((has_scrollbar()) && ((msg == RC_up) || (msg == RC_down)))
		{
			if (msg == RC_up)
				scroll_up();
			else
				scroll_down();
		}
		else if((msg == RC_mode) || (msg == RC_next) || (msg == RC_prev)) 
		{
			res = messages_return::cancel_info;
			g_RCInput->postMsg(msg, data);
		}
		else
		{
			res = CNeutrinoApp::getInstance()->handleMsg(msg, data);
/*
			if (res & messages_return::unhandled)
			{
				dprintf(DEBUG_NORMAL, "CHintBox::exec: message unhandled\n");

				res = messages_return::cancel_info;
				g_RCInput->postMsg(msg, data);
			}
*/
		}

		CFrameBuffer::getInstance()->blit();	
	}

	hide();

	return res;
}

int HintBox(const neutrino_locale_t Caption, const char * const Text, const int Width, int timeout, const char * const Icon)
{
	int res = messages_return::none;

	neutrino_msg_t msg;
	neutrino_msg_data_t data;

 	CHintBox * hintBox = new CHintBox(Caption, Text, Width, Icon);

	res = hintBox->exec(timeout);
		
	delete hintBox;
	hintBox = NULL;

	return res;
}

int HintBox(const neutrino_locale_t Caption, const neutrino_locale_t Text, const int Width, int timeout, const char * const Icon)
{
	return HintBox(Caption, g_Locale->getText(Text), Width, timeout, Icon);
}

int HintBox(const char * Caption, const char * const Text, const int Width, int timeout, const char * const Icon)
{
	int res = messages_return::none;

	neutrino_msg_t msg;
	neutrino_msg_data_t data;

 	CHintBox * hintBox = new CHintBox(Caption, Text, Width, Icon);

	res = hintBox->exec(timeout);
		
	delete hintBox;
	hintBox = NULL;

	return res;
}

int HintBox(const char * Caption, const neutrino_locale_t Text, const int Width, int timeout, const char * const Icon)
{
	return HintBox(Caption, g_Locale->getText(Text), Width, timeout, Icon);
}


