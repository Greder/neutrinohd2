#include <systeminfo.h>


sfileline sinbuffer[3*MAXLINES];
sreadline sysbuffer[(2*MAXLINES)];

int slinecount,syscount;
bool refreshIt = true;

//Konstruktor
CBESysInfoWidget::CBESysInfoWidget(int m)
{
	frameBuffer = CFrameBuffer::getInstance();
	selected = 0;
	
	// windows size
	width  = w_max ( (frameBuffer->getScreenWidth() / 20 * 17), (frameBuffer->getScreenWidth() / 20 ));
	height = h_max ( (frameBuffer->getScreenHeight() / 20 * 16), (frameBuffer->getScreenHeight() / 20));
       
	ButtonHeight = 25;
	theight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight();
	fheight = g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST]->getHeight();
	listmaxshow = (height-theight-0)/fheight;
	height = theight+listmaxshow*fheight; // recalc height
	x =(((g_settings.screen_EndX- g_settings.screen_StartX)-width) / 2) + g_settings.screen_StartX;
	y =(((g_settings.screen_EndY- g_settings.screen_StartY)-height) / 2) + g_settings.screen_StartY;
	liststart = 0;
	state = beDefault;
	mode = m;
}

//zeichnet einen Listeneintrag
void CBESysInfoWidget::paintItem(int pos)
{
	int ypos = y + theight + 0 + pos*fheight;
	uint8_t    color;
	fb_pixel_t bgcolor;
	
	if (liststart + pos == selected)
	{ 
		color   = COL_MENUCONTENTSELECTED;
		bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
	}
	else
	{
		color   = COL_MENUCONTENT;
		bgcolor = COL_MENUCONTENT_PLUS_0;
	}
				
	if ((liststart + pos == selected)&&(mode != SYSINFO))
	{
		color = COL_MENUCONTENTSELECTED;
	}

	frameBuffer->paintBoxRel(x, ypos, width - 15, fheight, bgcolor);

	if (liststart + pos < syscount)
	{
		char tmpline75[75];

		memcpy(tmpline75,  &sysbuffer[liststart+pos].line[0], 75);
		tmpline75[75] = '\0';

		g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST]->RenderString(x + 5, ypos + fheight, width - 30, tmpline75, color);
	}
}

//zeichent die Liste
void CBESysInfoWidget::paint()
{
	//printf("[neutrino] System-Info\n");

	liststart = (selected/listmaxshow)*listmaxshow;

	for(unsigned int count = 0; count < listmaxshow; count++)
	{
		paintItem(count);
	}

	int ypos = y + theight;
	int sb   = fheight* listmaxshow;
	
	frameBuffer->paintBoxRel(x + width - 15, ypos, 15, sb, COL_MENUCONTENT_PLUS_1);

	int sbc = (syscount/listmaxshow) + 1;
	sbc = (syscount/listmaxshow) + 1;
	float sbh= (sb - 4)/ sbc;
	int sbs  = (selected/listmaxshow);
	frameBuffer->paintBoxRel(x + width - 13, ypos + 2 + int(sbs* sbh), 11, int(sbh),  COL_MENUCONTENT_PLUS_3);

}

//zeichnet Kopfzeile
void CBESysInfoWidget::paintHead()
{
	char buf[100];

	frameBuffer->paintBoxRel(x, y, width, theight, COL_MENUHEAD_PLUS_0, RADIUS_MID, CORNER_TOP);
	
	if(mode == SYSINFO)
		sprintf((char *) buf, "%s", "System-Info:");
	
	if(mode == DMESGINFO)
		sprintf((char *) buf, "%s", "System-Messages:");
	
	if(mode == CPUINFO)
		sprintf((char *) buf, "%s", "CPU/File-Info:");
	
	if(mode == PSINFO)
		sprintf((char *) buf, "%s", "Prozess-Liste:");

	g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(x + 10, y + theight + 0, width, buf, COL_MENUHEAD);
}

//zeichnet Fusszeile
void CBESysInfoWidget::paintFoot()
{
	int ButtonWidth = (width - 28) / 4;
	frameBuffer->paintBoxRel(x, y + height, width, ButtonHeight, COL_MENUHEAD_PLUS_0, RADIUS_MID, CORNER_BOTTOM);
	frameBuffer->paintHLine(x, x + width, y, COL_INFOBAR_SHADOW_PLUS_0);

	//if(mode != SYSINFO)
	{
		frameBuffer->paintIcon(NEUTRINO_ICON_BUTTON_RED,    x+width- 4* ButtonWidth - 20, y+height+4);
		g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(x+width- 4* ButtonWidth, y+height+24 - 2, ButtonWidth- 26, "info", COL_INFOBAR);
	}
	
	//if(mode != DMESGINFO)
	{
		frameBuffer->paintIcon(NEUTRINO_ICON_BUTTON_GREEN,  x+width- 3* ButtonWidth - 30, y+height+4);
		g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(x+width- 3* ButtonWidth - 10, y+height+24 - 2, ButtonWidth- 26, "dmesg", COL_INFOBAR);
	}
	
	//if(mode != CPUINFO)
	{
		frameBuffer->paintIcon(NEUTRINO_ICON_BUTTON_YELLOW,   x+width- 2* ButtonWidth - 30, y+height+4);
		g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(x+width- 2* ButtonWidth - 10, y+height+24 - 2, ButtonWidth- 26, "cpu/file", COL_INFOBAR);
	}
	
	//if(mode != PSINFO)
	{
		frameBuffer->paintIcon(NEUTRINO_ICON_BUTTON_BLUE,   x+width- 1* ButtonWidth - 30, y+height+4);
		g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(x+width- 1* ButtonWidth - 10, y+height+24 - 2, ButtonWidth- 26, "ps", COL_INFOBAR);
	}
	
	/*
	//else
	{
		if(refreshIt == true)
		{
			frameBuffer->paintIcon(NEUTRINO_ICON_BUTTON_BLUE,   x+width- 1* ButtonWidth - 30, y+height+4);
			g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(x+width- 1* ButtonWidth - 10, y+height+24 - 2, ButtonWidth- 26, "stop refresh", COL_INFOBAR);
		}
		else
		{
			frameBuffer->paintIcon(NEUTRINO_ICON_BUTTON_BLUE,   x+width- 1* ButtonWidth - 30, y+height+4);
			g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(x+width- 1* ButtonWidth - 10, y+height+24 - 2, ButtonWidth- 26, "start refresh", COL_INFOBAR);
		}
	}
	*/
}

//killt sysinfo
void CBESysInfoWidget::hide()
{
	frameBuffer->paintBackgroundBoxRel(x, y, width, height + ButtonHeight);
}

//main
int CBESysInfoWidget::exec(CMenuTarget *parent, const std::string & actionKey)
{
	int res = menu_return::RETURN_REPAINT;

	if(mode == SYSINFO)
		sysinfo();
	else if(mode == DMESGINFO)
		dmesg();
	else if(mode == CPUINFO)
		cpuinfo();
	else if(mode == PSINFO)
		ps();
	else
	{
		//ShowHint("Alert", "Error", "info.raw", 430);
		hide();
		return(-1);
	}

	if (parent)
		parent->hide();

	paintHead();
	paint();
	paintFoot();

	neutrino_msg_t msg; 
	neutrino_msg_data_t data;
	int timercount = 0;
	unsigned long long timeoutEnd = g_RCInput->calcTimeoutEnd(5);

	while (msg != (neutrino_msg_t) g_settings.key_channelList_cancel)
	{
		g_RCInput->getMsgAbsoluteTimeout( &msg, &data, &timeoutEnd );

		if (msg <= CRCInput::RC_MaxRC  ) 
			timeoutEnd = g_RCInput->calcTimeoutEnd(5);
		
		if (msg == CRCInput::RC_timeout)
		{
			if (mode == SYSINFO)
			{
				timercount = 0;
				sysinfo();
				selected = 0;
				paintHead();
				paint();
				paintFoot();
			}
			
			if ((mode == DMESGINFO) && (++timercount>11))
			{
				timercount = 0;
				dmesg();
				paintHead();
				paint();
				paintFoot();
			}
			
			if ((mode == PSINFO)&&(refreshIt==true))
			{
				timercount = 0;
				ps();
				paintHead();
				paint();
				paintFoot();
			}

			timeoutEnd = g_RCInput->calcTimeoutEnd(5);
			g_RCInput->getMsgAbsoluteTimeout( &msg, &data, &timeoutEnd );
		}
		
		if ((msg == CRCInput::RC_up || msg == CRCInput::RC_left)&&(mode != SYSINFO))
		{
			int step = 0;
			int prev_selected = selected;

			step = (msg == CRCInput::RC_left) ? listmaxshow : 1;
			selected -= step;
			if((prev_selected-step) < 0) selected = syscount-1;
			if(state == beDefault)
			{
				paintItem(prev_selected - liststart);
				unsigned int oldliststart = liststart;
				liststart = (selected/listmaxshow)*listmaxshow;
				if(oldliststart!=liststart) 
					paint();
				else 
					paintItem(selected - liststart);
			}
		}
		else if ((msg == CRCInput::RC_down || msg == CRCInput::RC_right)&&(mode != SYSINFO))
		{
			int step = 0;
			int prev_selected = selected;

			step = (msg == CRCInput::RC_right) ? listmaxshow : 1;
			selected += step;
			if(selected>=syscount) selected = 0;
			if(state == beDefault)
			{
				paintItem(prev_selected - liststart);
				unsigned int oldliststart = liststart;
				liststart = (selected/listmaxshow)*listmaxshow;
				if(oldliststart!=liststart) 
					paint();
				else 
					paintItem(selected - liststart);
			}
		}
		else if ((msg == CRCInput::RC_red)&&(mode != SYSINFO))
		{
			mode = SYSINFO;
			sysinfo();
			selected = 0;
			paintHead();
			paint();
			paintFoot();

		}
		else if ((msg == CRCInput::RC_green)&&(mode != DMESGINFO))
		{
			mode = DMESGINFO;
			timercount = 0;
			dmesg();
			paintHead();
			paint();
			paintFoot();
		}
		else if ((msg == CRCInput::RC_yellow)&&(mode != CPUINFO))
		{
			mode = CPUINFO;
			cpuinfo();
			paintHead();
			paint();
			paintFoot();
		}
		else if (msg == CRCInput::RC_blue)
		{
			if (mode == PSINFO)
				refreshIt =! refreshIt;
			else
			    refreshIt = true;
			mode = PSINFO;

			if (refreshIt)
				ps();
			paintHead();
			paint();
			paintFoot();
		}
		else
		{
			CNeutrinoApp::getInstance()->handleMsg( msg, data );
			// kein canceling...
		}
	}
	
	hide();
	
	return res;
}

int CBESysInfoWidget::sysinfo()
{
	static long curCPU[5] = {0, 0, 0, 0, 0};
	static long prevCPU[5] = {0, 0, 0, 0, 0};
	double value[5]={0, 0, 0, 0, 0};
	float faktor;
	int i, j = 0;
	char strhelp[6];
	FILE *f;
	char line[MAXLINES];
	char *fmt = " %a %d %b %Y %H:%M";
	long t;

	/* Get and Format the SystemTime */
	t = time(NULL);
	struct tm *tp;
	tp = localtime(&t);
	strftime(line, sizeof(line), fmt, tp);
	/* Get and Format the SystemTime end */

	/* Create tmpfile with date /tmp/sysinfo */
	system("echo 'DATUM:' > /tmp/sysinfo");
	f=fopen("/tmp/sysinfo","a");
	if(f)
		fprintf(f,"%s\n", line);
	fclose(f);
	/* Create tmpfile with date /tmp/sysinfo end */

	/* Get the statistics from /proc/stat */
	if(prevCPU[0] == 0)
	{
		f=fopen("/proc/stat","r");
		if(f)
		{
			fgets(line, 256, f); /* cpu */
			sscanf(line,"cpu %lu %lu %lu %lu", &prevCPU[1], &prevCPU[2], &prevCPU[3], &prevCPU[4]);
			for(i = 1; i < 5; i++)
				prevCPU[0] += prevCPU[i];
		}
		fclose(f);
		sleep(1);
	}
	else
	{
		for(i=0;i<5;i++)
				prevCPU[i]=curCPU[i];
	}

	while(((curCPU[0]-prevCPU[0]) < 100) || (curCPU[0]==0))
	{
		f=fopen("/proc/stat","r");
		if(f)
		{
			curCPU[0]=0;
			fgets(line,256,f); /* cpu */
			sscanf(line,"cpu %lu %lu %lu %lu",&curCPU[1],&curCPU[2],&curCPU[3],&curCPU[4]);
			for(i=1;i<5;i++)
				curCPU[0]+=curCPU[i];
		}
		fclose(f);
		if((curCPU[0]-prevCPU[0])<100)
			sleep(1);
	}
	
	// some calculations
	if(!(curCPU[0] - prevCPU[0])==0)
	{
		faktor = 100.0/(curCPU[0] - prevCPU[0]);
		for(i=0;i<4;i++)
			value[i]=(curCPU[i]-prevCPU[i])*faktor;

		value[4]=100.0-value[1]-value[2]-value[3];

		f=fopen("/tmp/sysinfo","a");
		if(f)
		{
			memset(line,0x20,sizeof(line));
			for(i=1, j=0;i<5;i++)
			{
				memset(strhelp,0,sizeof(strhelp));
				sprintf(strhelp,"%.1f", value[i]);
				memcpy(&line[(++j*7)-2-strlen(strhelp)], &strhelp[0], strlen(strhelp));
				memcpy(&line[(j*7)-2], "%", 1);
			}
			line[(j*7)-1]='\0';
			fprintf(f,"\nPERFORMANCE:\n USER:  NICE:   SYS:  IDLE:\n%s\n", line);
		}
		fclose(f);
	}
	/* Get the statistics from /proc/stat end*/

	/* Get kernel-info from /proc/version*/
	f=fopen("/proc/version","r");
	if(f)
	{
		char* token;
		fgets(line,256,f); // version
		token = strstr(line,") (");
		if(token != NULL)
			*++token = 0x0;
		fclose(f);
		f=fopen("/tmp/sysinfo","a");
		fprintf(f, "\nKERNEL:\n %s\n %s\n", line, ++token);
	}
	fclose(f);
	/* Get kernel-info from /proc/version end*/

	/* Get uptime-info from /proc/uptime*/
	f=fopen("/proc/uptime","r");
	if(f)
	{
		fgets(line,256,f);
		float ret[4];
		const char* strTage[2] = {"Tage", "Tag"};
		const char* strStunden[2] = {"Stunden", "Stunde"};
		const char* strMinuten[2] = {"Minuten", "Minute"};
		sscanf(line,"%f",&ret[0]);
		ret[0]/=60;
		ret[1]=long(ret[0])/60/24; // Tage
		ret[2]=long(ret[0])/60-long(ret[1])*24; // Stunden
		ret[3]=long(ret[0])-long(ret[2])*60-long(ret[1])*60*24; // Minuten
		fclose(f);

		f=fopen("/tmp/sysinfo","a");
		if(f)
			fprintf(f, "UPTIME:\n System l�uft seit: %.0f %s %.0f %s %.0f %s\n", ret[1], strTage[int(ret[1])==1], ret[2], strStunden[int(ret[2])==1], ret[3], strMinuten[int(ret[3])==1]);
	}
	fclose(f);
	/* Get uptime-info from /proc/uptime end*/

	return(readList(sinbuffer));
}

int CBESysInfoWidget::cpuinfo()
{
	char Wert1[30];
	char Wert2[10];
	char Wert3[10];
	char Wert4[10];
	char Wert5[6];
	char Wert6[30];

	FILE *f,*w;
	char line[256];
	int i = 0;
	
	/* Get file-info from /proc/cpuinfo*/
	system("df > /tmp/systmp");
	f=fopen("/tmp/systmp","r");
	if(f)
	{
		w=fopen("/tmp/sysinfo","w");
		if(w)
		{
			while((fgets(line,256, f)!=NULL))
			{
				sscanf(line,"%s %s %s %s %s %s ", &Wert1, &Wert2, &Wert3, &Wert4, &Wert5, &Wert6);
				if(i++)
					fprintf(w,"\nFilesystem: %s\n  1-KBlocks: %s\n  Used: %s\n  Free: %s\n  Use%%: %s\nMounted on: %s\n",Wert1,Wert2,Wert3,Wert4,Wert5,Wert6);
			}
			fprintf(w,"\nCPU:\n\n");
			fclose(w);
		}
	}
	fclose(f);
	/* Get file-info from /proc/cpuinfo end*/

	/* Get cpuinfo from /proc/cpuinfo*/
	system("cat /proc/cpuinfo >> /tmp/sysinfo");
	unlink("/tmp/systmp");
	/* Get cpuinfo from /proc/cpuinfo end*/
	
	return(readList(sinbuffer));

}

int CBESysInfoWidget::dmesg()
{
	/* Get System-Messages from dmesg*/
	system("dmesg > /tmp/sysinfo");
	/* Get System-Messages from dmesg end*/

	return(readList(sinbuffer));
}

int CBESysInfoWidget::ps()
{
	/* Get Processlist from ps*/
	system("ps -A > /tmp/sysinfo");
	/* Get Processlist from ps end*/

	return(readList(sinbuffer));
}

//Infos auslesen
int CBESysInfoWidget::readList(struct sfileline *sinbuffer)
{
	FILE *fp;
	char line[1024];

	memset(sinbuffer  ,0,(3*MAXLINES) * sizeof(struct sfileline));
	memset(sysbuffer ,0,(2*MAXLINES) * sizeof(struct sreadline));

	fp = fopen("/tmp/sysinfo","rb");

	if(fp==NULL)
		return(-1);

	slinecount=0;
	syscount=0;

	while(fgets(line, sizeof(line), fp) != NULL)
	{
		line[1024]='\0';
		memcpy(sysbuffer[syscount].line, line, sizeof(line));
		sinbuffer[slinecount].state = true;
		//printf("%s", sysbuffer[syscount].line);
		sinbuffer[slinecount++].addr=sysbuffer[syscount++].line;
	}
	fclose(fp);
	
	if (selected>=slinecount)
		selected=slinecount-1;
	
	return(0);
}

extern "C" int plugin_exec(void);

int plugin_exec(void)
{
	printf("Plugins: starting systeminfo\n");
	
	CBESysInfoWidget * SysInfoWidget = new CBESysInfoWidget();
	
	SysInfoWidget->exec(NULL, "");
	SysInfoWidget->hide();
	
	delete SysInfoWidget;
	
	return 0;
}