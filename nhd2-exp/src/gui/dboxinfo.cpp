/*
	Neutrino-GUI  -   DBoxII-Project
	
	$Id: dboxinfo.cpp 2013/10/12 mohousch Exp $

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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <sys/vfs.h>

#include <global.h>

#include <driver/fontrenderer.h>
#include <driver/screen_max.h>
#include <driver/rcinput.h>

#include <gui/dboxinfo.h>
#include <gui/imageinfo.h>
#include <gui/streaminfo2.h>

// zapit includes
#include <frontend_c.h>

#include <system/debug.h>


static const int FSHIFT = 16;              	// nr of bits of precision
#define FIXED_1         (1<<FSHIFT)     	// 1.0 as fixed-point
#define LOAD_INT(x) ((x) >> FSHIFT)
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)

extern int FrontendCount;
CFrontend * getFE(int index);

// hdd
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/mount.h>


static int my_filter(const struct dirent * dent)
{
	if(dent->d_name[0] == 's' && dent->d_name[1] == 'd')
		return 1;
	
	return 0;
}

CDBoxInfoWidget::CDBoxInfoWidget()
{
	dprintf(DEBUG_DEBUG, "CDBoxInfoWidget::CDBoxInfoWidget:\n");
	
	Box.iX = g_settings.screen_StartX + 20;
	Box.iY = g_settings.screen_StartY + 20;
	Box.iWidth = g_settings.screen_EndX - g_settings.screen_StartX - 40;
	Box.iHeight = (g_settings.screen_EndY - g_settings.screen_StartY - 40);
}

int CDBoxInfoWidget::exec(CMenuTarget * parent, const std::string &)
{
	dprintf(DEBUG_NORMAL, "CDBoxInfoWidget::exec:\n");

	if (parent)
		parent->hide();
	
	showInfo();

	hide();	

	return RETURN_REPAINT;
}

void CDBoxInfoWidget::hide()
{
	dprintf(DEBUG_NORMAL, "CDBoxInfoWidget::hide:\n");
}

void CDBoxInfoWidget::showInfo()
{
	dprintf(DEBUG_NORMAL, "CDBoxInfoWidget::showInfo:\n");

	dboxInfo = new CWidget(&Box);
	
	m_window = new CWindow(&Box);
	m_window->setCorner(RADIUS_MID, CORNER_ALL);
	
	int yPos = Box.iY;
	
	// head
	CHeaders* head = new CHeaders(Box.iX, yPos, Box.iWidth, 40, _("Box Info"), NEUTRINO_ICON_INFO);
	head->enablePaintDate();
	head->setFormat("%d.%m.%Y %H:%M:%S");

	int i = 0;

	//cpu
	yPos += 40 + 10;
	
	std::string CPU = g_settings.hints_dir;
	CPU += "/";
	CPU += NEUTRINO_ICON_MENUITEM_IMAGEINFO;
	CPU += ".png";
	
	CCIcon* cpuIcon = new CCIcon(Box.iX + 10, yPos, cpuIcon->iWidth, cpuIcon->iHeight);
	cpuIcon->setIcon(CPU.c_str());
	m_window->addCCItem(cpuIcon);
	
	CCLabel* cpuLabel = new CCLabel();
	cpuLabel->setText(_("CPU:"));
	cpuLabel->setFont(g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1]);
	cpuLabel->setColor(COL_MENUHEAD);
	cpuLabel->setPosition(Box.iX + 10 + cpuIcon->iWidth + 10, yPos, cpuLabel->getWindowsPos().iWidth, cpuIcon->iHeight);
	m_window->addCCItem(cpuLabel);
	
	yPos += cpuIcon->iHeight + 10;
	
	FILE* fd = fopen("/proc/cpuinfo", "rt");

	if (fd == NULL) 
	{
		printf("error while opening proc-cpuinfo\n" );
	} 
	else 
	{
		char *buffer = NULL;
		size_t len = 0;
		ssize_t read;

		while ((read = getline(&buffer, &len, fd)) != -1) 
		{
			if (!(strncmp(const_cast<char *>("Hardware"), buffer, 8))) 
			{
				char *t = rindex(buffer, '\n');
				if (t)
					*t = '\0';

				std::string hw;
				char *p = rindex(buffer, ':');
				if (p)
					hw = ++p;
				hw += " Info";
				
				CCLabel* cpuLabel1 = new CCLabel();
				cpuLabel1->setText(hw.c_str());
				cpuLabel1->setFont(g_Font[SNeutrinoSettings::FONT_TYPE_MENU]);
				cpuLabel1->setColor(COL_MENUCONTENT);
				cpuLabel1->setPosition(Box.iX + 10 + cpuIcon->iWidth, yPos, Box.iWidth, cpuLabel1->getWindowsPos().iHeight);
				m_window->addCCItem(cpuLabel1);
				
				yPos += cpuLabel1->getWindowsPos().iHeight;
				
				break;
			}
			i++;

			if (i > 4)
				continue;
				
			CCLabel* cpuLabel2 = new CCLabel();
			cpuLabel2->setText(buffer);
			cpuLabel2->setFont(g_Font[SNeutrinoSettings::FONT_TYPE_MENU]);
			cpuLabel2->setColor(COL_MENUCONTENT);
			cpuLabel2->setPosition(Box.iX + 10 + cpuIcon->iWidth, yPos, Box.iWidth, cpuLabel2->getWindowsPos().iHeight);
			m_window->addCCItem(cpuLabel2);
			
			yPos += cpuLabel2->getWindowsPos().iHeight;
		}
		fclose(fd);

		if (buffer)
			free(buffer);
	}
	
	// separator
	CCHline* hLine = new CCHline();
	hLine->setPosition(Box.iX + 10,  yPos, Box.iWidth - 20, 10);
	m_window->addCCItem(hLine);

	// up time
	int updays, uphours, upminutes;
	struct sysinfo info;
	struct tm *current_time;
	time_t current_secs;
	char ubuf[256], sbuf[256];

	memset(sbuf, 0, 256);
	time(&current_secs);
	current_time = localtime(&current_secs);

	sysinfo(&info);

	sprintf( ubuf, "%2d:%02d%s  up ", 
			current_time->tm_hour%12 ? current_time->tm_hour%12 : 12, 
			current_time->tm_min, current_time->tm_hour > 11 ? "pm" : "am");

	strcat(sbuf, ubuf);
	updays = (int) info.uptime / (60*60*24);
	if (updays) 
	{
		sprintf(ubuf, "%d day%s, ", updays, (updays != 1) ? "s" : "");
		strcat(sbuf, ubuf);
	}
	upminutes = (int) info.uptime / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	if(uphours)
		sprintf(ubuf,"%2d:%02d, ", uphours, upminutes);
	else
		sprintf(ubuf,"%d min, ", upminutes);
	strcat(sbuf, ubuf);

	sprintf(ubuf, "load: %ld.%02ld, %ld.%02ld, %ld.%02ld\n", 
			LOAD_INT(info.loads[0]), LOAD_FRAC(info.loads[0]), 
			LOAD_INT(info.loads[1]), LOAD_FRAC(info.loads[1]), 
			LOAD_INT(info.loads[2]), LOAD_FRAC(info.loads[2]));

	strcat(sbuf, ubuf);
	
	CCLabel* upLabel = new CCLabel();
	upLabel->setText(sbuf);
	upLabel->setFont(g_Font[SNeutrinoSettings::FONT_TYPE_MENU]);
	upLabel->setColor(COL_MENUCONTENTINACTIVE);
	upLabel->setPosition(Box.iX + 10, yPos, Box.iWidth, 35);
	m_window->addCCItem(upLabel);
	
	// mem
	sprintf(ubuf, "memory total %dKb, free %dKb", (int) info.totalram/1024, (int) info.freeram/1024);

	yPos += upLabel->getWindowsPos().iHeight + 10;
	
	CCLabel* memLabel = new CCLabel();
	memLabel->setText(ubuf);
	memLabel->setFont(g_Font[SNeutrinoSettings::FONT_TYPE_MENU]);
	memLabel->setColor(COL_MENUCONTENTINACTIVE);
	memLabel->setPosition(Box.iX + 10, yPos, Box.iWidth, 35);
	m_window->addCCItem(memLabel);
	
	// separator
	yPos += memLabel->getWindowsPos().iHeight + 10;
	
	CCHline* hLine2 = new CCHline();
	hLine2->setPosition(Box.iX + 10,  yPos, Box.iWidth - 20, 10);
	m_window->addCCItem(hLine2);
    	
    	//hdd devices
    	std::string HDD = g_settings.hints_dir;
	HDD += "/";
	HDD += NEUTRINO_ICON_MENUITEM_HDDSETTINGS;
	HDD += ".png";
    	CCIcon* hddIcon = new CCIcon();
    	hddIcon->setIcon(HDD.c_str());
    	
	FILE * f;
	int fd_hdd;
	struct dirent **namelist;
	
	int n = scandir("/sys/block", &namelist, my_filter, alphasort);
	
	if (n)
	{
		yPos += 10;
		
		hddIcon->setPosition(Box.iX + 10, yPos, hddIcon->iWidth, hddIcon->iHeight);
		m_window->addCCItem(hddIcon);
		
		CCLabel* hddLabel = new CCLabel();
		hddLabel->setText(_("HDD Devices:"));
		hddLabel->setFont(g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1]);
		hddLabel->setColor(COL_MENUHEAD);
		hddLabel->setPosition(Box.iX + 10 + hddIcon->iWidth + 10, yPos, hddLabel->getWindowsPos().iWidth, hddIcon->iHeight);
		m_window->addCCItem(hddLabel);
	}
	
	yPos += hddIcon->iHeight + 10;
	for(int i1 = 0; i1 < n; i1++) 
	{
		char str[256];
		char vendor[128];
		char model[128];
		int64_t bytes;
		int64_t megabytes;
		int removable = 0;
		
		sprintf(str, "/dev/%s", namelist[i1]->d_name);
		fd_hdd = open(str, O_RDONLY);

		if(fd_hdd < 0) 
		{
			//printf("[neutrino] HDD: Cant open %s\n", str);
			continue;
		}
		
		if (ioctl(fd_hdd, BLKGETSIZE64, &bytes))
			perror("BLKGETSIZE64");
                
                close(fd_hdd);

		megabytes = bytes/1000000;

		// vendor
		sprintf(str, "/sys/block/%s/device/vendor", namelist[i1]->d_name);
		f = fopen(str, "r");

		if(!f) 
		{
			continue;
		}
		fscanf(f, "%s", vendor);
		fclose(f);

		// model
		sprintf(str, "/sys/block/%s/device/model", namelist[i1]->d_name);
		f = fopen(str, "r");
		
		if(!f) 
		{
			continue;
		}
		fscanf(f, "%s", model);
		fclose(f);

		// removable
		sprintf(str, "/sys/block/%s/removable", namelist[i1]->d_name);
		f = fopen(str, "r");
		
		if(!f) 
		{
			continue;
		}
		fscanf(f, "%d", &removable);
		fclose(f);
		
		// free space on hdd
		struct statfs s;
		
		if (::statfs(g_settings.network_nfs_recordingdir, &s) == 0) 
		{
			sprintf(str, "%s (%s-%s %lld %s), free %ldMB", namelist[i1]->d_name, vendor, model, megabytes < 10000 ? megabytes : megabytes/1000, megabytes < 10000 ? "MB" : "GB", (long)( ((s.f_bfree/1024)/1024))*s.f_bsize);
		}
		else
			sprintf(str, "%s (%s-%s %lld %s)", namelist[i1]->d_name, vendor, model, megabytes < 10000 ? megabytes : megabytes/1000, megabytes < 10000 ? "MB" : "GB");
		
		free(namelist[i1]);
		
		CCLabel* hddLabel1 = new CCLabel();
		hddLabel1->setText(str);
		hddLabel1->setFont(g_Font[SNeutrinoSettings::FONT_TYPE_MENU]);
		hddLabel1->setColor(COL_MENUCONTENT);
		hddLabel1->setPosition(Box.iX + 10 + hddIcon->iWidth, yPos, Box.iWidth, hddLabel1->getWindowsPos().iHeight);
		m_window->addCCItem(hddLabel1);
			
		yPos += hddLabel1->getWindowsPos().iHeight;
	}
	
	//frontend
	yPos += hddIcon->iHeight + 10;
	
	std::string TUNER = g_settings.hints_dir;
	TUNER += "/";
	TUNER += NEUTRINO_ICON_MENUITEM_SCANSETTINGS;
	TUNER += ".png";
	
	CCIcon* tunerIcon = new CCIcon();
	tunerIcon->setIcon(TUNER.c_str());
	
	if (FrontendCount)
	{
		tunerIcon->setPosition(Box.iX + 10, yPos, tunerIcon->iWidth, tunerIcon->iHeight);
		m_window->addCCItem(tunerIcon);
		
		CCLabel* tunerLabel = new CCLabel();
		tunerLabel->setText(_("Frontend:"));
		tunerLabel->setFont(g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1]);
		tunerLabel->setColor(COL_MENUHEAD);
		tunerLabel->setPosition(Box.iX + 10 + tunerIcon->iWidth + 10, yPos, tunerLabel->getWindowsPos().iWidth, tunerIcon->iHeight);
		m_window->addCCItem(tunerLabel);
	}
	
	yPos += tunerIcon->iHeight + 10;
	for(int i2 = 0; i2 < FrontendCount; i2++)
	{
		CFrontend * fe = getFE(i2);
		char tbuf[255];
		
		sprintf(tbuf, "Tuner-%d: %s", i2 + 1, fe->getInfo()->name);
		
		CCLabel* tunerLabel1 = new CCLabel();
		tunerLabel1->setText(tbuf);
		tunerLabel1->setFont(g_Font[SNeutrinoSettings::FONT_TYPE_MENU]);
		tunerLabel1->setColor(COL_MENUCONTENT);
		tunerLabel1->setPosition(Box.iX + 10 + tunerIcon->iWidth, yPos, tunerLabel1->getWindowsPos().iWidth, tunerLabel1->getWindowsPos().iHeight);
		m_window->addCCItem(tunerLabel1);
		
		yPos += tunerLabel1->getWindowsPos().iHeight;
		
	}

	dboxInfo->addItem(m_window);
	dboxInfo->addItem(head);
	
	dboxInfo->exec(NULL, "");

	delete dboxInfo;
	dboxInfo = NULL;
	
	delete m_window;
	m_window = NULL;	
}

//
int CInfoMenu::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CInfoMenu::exec: %s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if (parent)
		parent->hide();
		
	showMenu();
	
	return ret;
}

void CInfoMenu::showMenu()
{
	infoMenu = new CMenuWidget("Information", NEUTRINO_ICON_INFO);
	
	infoMenu->setWidgetMode(MODE_MENU);
	infoMenu->setWidgetType(WIDGET_TYPE_CLASSIC);
	infoMenu->setMenuPosition(MENU_POSITION_CENTER);
	infoMenu->enablePaintDate();
	infoMenu->enableShrinkMenu();
	
	infoMenu->addItem( new CMenuForwarder(_("Information"), true, NULL, new CDBoxInfoWidget(), NULL, RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_BOXINFO, _("Here you can get HW infos about your STB.\n")));
	
	infoMenu->addItem(new CMenuForwarder(_("Image info"),  true, NULL, new CImageInfo(), NULL, RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_IMAGEINFO, _("Here you can get infos about the software.\n")), false);
	
	infoMenu->addItem(new CMenuForwarder(_("Stream information"), true, NULL, new CStreamInfo2Handler(), "", RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_BOXINFO, _("Here you can get infos about the Channel.\n")));
	
	infoMenu->integratePlugins(CPlugins::I_TYPE_MAIN);
	
	infoMenu->exec(NULL, "");
}


