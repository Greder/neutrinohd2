/*
	Neutrino-GUI  -   DBoxII-Project
	
	$Id: window.cpp 2016.12.12 11:43:30 mohousch Exp $

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

#include <gui/widget/window.h>
#include <system/debug.h>

#include <video_cs.h>


extern cVideo * videoDecoder;

CWindow::CWindow(const int x, const int y, const int dx, const int dy)
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	itemBox.iX = x;
	itemBox.iY = y;
	itemBox.iWidth = dx;
	itemBox.iHeight = dy;

	centerPos = false;

	init();
}

CWindow::CWindow(CBox* position)
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	itemBox = *position;

	centerPos = false;

	init();
}

void CWindow::init()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	frameBuffer = CFrameBuffer::getInstance();

	radius = NO_RADIUS;
	corner = CORNER_NONE;
	bgcolor = COL_MENUCONTENT_PLUS_0;
	gradient = nogradient;

	enableshadow = false;

	savescreen = false;
	background = NULL;

	// sanity check
	if(itemBox.iHeight > ((int)frameBuffer->getScreenHeight()))
		itemBox.iHeight = frameBuffer->getScreenHeight();

	// sanity check
	if(itemBox.iWidth > (int)frameBuffer->getScreenWidth())
		itemBox.iWidth = frameBuffer->getScreenWidth();

	if(centerPos)
	{
		itemBox.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - itemBox.iWidth) >> 1 );
		itemBox.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - itemBox.iHeight) >> 1 );
	}
}

void CWindow::saveScreen()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	if(!savescreen)
		return;

	if(background)
	{
		delete[] background;
		background = NULL;
	}

	background = new fb_pixel_t[itemBox.iWidth*itemBox.iHeight];
	
	if(background)
	{
		frameBuffer->saveScreen(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, background);
	}
}

void CWindow::restoreScreen()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	if(background) 
	{
		if(savescreen)
			frameBuffer->restoreScreen(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, background);
	}

	delete[] background;
	background = NULL;
}

void CWindow::enableSaveScreen()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	savescreen = true;
	
	if(!savescreen && background) 
	{
		delete[] background;
		background = NULL;
	}
}

void CWindow::setPosition(const int x, const int y, const int dx, const int dy)
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	itemBox.iX = x;
	itemBox.iY = y;
	itemBox.iWidth = dx;
	itemBox.iHeight = dy;
}

void CWindow::setPosition(CBox* position)
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	itemBox = *position;
}

void CWindow::paint()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	if(savescreen) 
		saveScreen();

	if(enableshadow)
	{
		// shadow Box
		frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, COL_MENUCONTENT_PLUS_6);

		// window Box
		frameBuffer->paintBoxRel(itemBox.iX + 1, itemBox.iY + 1, itemBox.iWidth - 2, itemBox.iHeight - 2, bgcolor, enableshadow? NO_RADIUS : radius, enableshadow? CORNER_NONE : corner, gradient);
	}
	else
		frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, bgcolor, enableshadow? NO_RADIUS : radius, enableshadow? CORNER_NONE : corner, gradient);
}

void CWindow::hide()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	if( savescreen && background)
		restoreScreen();
	else
		frameBuffer->paintBackgroundBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight);
}

// pig
CPig::CPig(const int x, const int y, const int dx, const int dy)
{
	itemBox.iX = x;
	itemBox.iY = y;
	itemBox.iWidth = dx;
	itemBox.iHeight = dy;

	init();
}

CPig::CPig(CBox* position)
{
	itemBox = *position;

	init();
}

void CPig::init()
{
	frameBuffer = CFrameBuffer::getInstance();
}

void CPig::paint()
{
	frameBuffer->paintBackgroundBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight);	
		

	if(videoDecoder)
		videoDecoder->Pig(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight);	
}

void CPig::hide()
{
	if(videoDecoder)  
		videoDecoder->Pig(-1, -1, -1, -1);

	frameBuffer->paintBackgroundBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight);
}

// grid
CGrid::CGrid(const int x, const int y, const int dx, const int dy)
{
	itemBox.iX = x;
	itemBox.iY = y;
	itemBox.iWidth = dx;
	itemBox.iHeight = dy;

	init();
}

CGrid::CGrid(CBox* position)
{
	itemBox = *position;

	init();
}

void CGrid::init()
{
	frameBuffer = CFrameBuffer::getInstance();

	rgb = 0x505050;
	inter_frame = 15;
}

void CGrid::paint()
{
	// hlines grid
	for(int count = 0; count < itemBox.iHeight; count += inter_frame)
		frameBuffer->paintHLine(itemBox.iX, itemBox.iX + itemBox.iWidth, itemBox.iY + count, make16color(rgb) );

	// vlines grid
	for(int count = 0; count < itemBox.iWidth; count += inter_frame)
		frameBuffer->paintVLine(itemBox.iX + count, itemBox.iY, itemBox.iY + itemBox.iHeight, make16color(rgb) );
}

void CGrid::hide()
{
	frameBuffer->paintBackgroundBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight);
}





