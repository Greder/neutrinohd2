/*
  $Id: test.cpp 26.09.2021 mohousch Exp $

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

#include <plugin.h>


extern "C" void plugin_exec(void);
extern "C" void plugin_init(void);
extern "C" void plugin_del(void);

class CTestMenu : public CMenuTarget
{
	private:
		// variables
		CFrameBuffer* frameBuffer;

		//
		CAudioPlayerGui tmpAudioPlayerGui;
		CMoviePlayerGui tmpMoviePlayerGui;
		CPictureViewerGui tmpPictureViewerGui;

		//
		int selected;

		//
		CFileFilter fileFilter;
		CFileList filelist;

		// movie
		CMovieInfo m_movieInfo;
		CMoviePlayList m_vMovieInfo;

		// audio
		CAudioPlayList AudioPlaylist;

		// pictures
		CPicture pic;
		CPicturePlayList PicPlaylist;

		//
		CTmdb* tmdb;
		std::string thumbnail_dir;
		CFileHelpers fileHelper;
		cYTFeedParser ytparser;
		std::string plist;
		int page;
		std::string TVShows;
		tmdb_video_list_t mvlist;
		std::string tmdbsearch;

		//
		CChannelList* webTVchannelList;
		CBouquetList* webTVBouquetList;

		// CButtons
		CButtons buttons;
		
		// CHeaders
		CBox headBox;
		CHeaders * headers;
		
		// CFooters
		CBox footBox;
		CFooters *footers;

		// CWidget
		CWidget *testWidget;

		// frameBox
		CBox topBox;
		CFrameBox *frameBoxWidget;
		int top_selected;

		// ClistBox
		CBox leftBox;
		ClistBox *leftWidget;
		int left_selected;

		// ClistBox
		CBox rightBox;
		ClistBox *rightWidget;
		int right_selected;

		//
		CListFrame *listFrame;
		CTextBox *textWidget;

		CWindow *windowWidget;
		CProgressWindow * progressWindow;
		
		// CMenuwidget
		CMenuWidget* listMenu;
		CMenuItem* item;

		// functions helpers
		void loadTMDBPlaylist(const char *txt = "movie", const char *list = "popular", const int seite = 1, bool search = false);
		void loadMoviePlaylist();
		void openMovieFileBrowser();
		void loadAudioPlaylist();
		void openAudioFileBrowser();
		void loadPicturePlaylist();
		void openPictureFileBrowser();	

		// testing widgets helpers (without CWidget)
		void test();

		// testing (with CWidget)
		void testCWidget();
		void testSingleWidget();
		void testFireTV();
		void testListFrameWidget();
		void testListBoxWidget();
		void testTextBoxWidget();
		//void testWindowWidget();
		//void testProgressWindowWidget();
		void testMultiWidget();

		// compenents helpers
		void testCBox();
		void testCIcon();
		void testCImage();
		void testCProgressBar();
		void testCButtons();

		// widget_helpers
		void testCHeaders();
		void testCWindow();
		void testCWindowShadow();
		void testCWindowCustomColor();
		void testCProgressWindow();
		void testCTextBox();
		void testCListFrame();
		void testCFrameBox();
		void testClistBox();
		void testClistBox2();
		void testClistBox3();
		void testClistBox4();
		void testClistBox5();
		void testClistBox6();
		
		// CMenuWidget
		void testClistBoxWidget();
		void testClistBoxWidget1();
		void testClistBoxWidget2();

		// widgets
		void testCStringInput();
		void testCStringInputSMS();
		void testCPINInput();
		void testCPLPINInput();
		void testCPINChangeWidget();
		void testCIPInput();
		void testCDateInput();
		void testCMACInput();
		void testCTimeInput();
		void testCIntInput();
		void testCInfoBox();
		void testCMessageBox();
		void testCMessageBoxInfoMsg();
		void testCMessageBoxErrorMsg();
		void testCHintBox();
		void testCHintBoxInfo();
		void testCHelpBox();

		// gui widgets
		void testVFDController();
		void testColorChooser();
		void testKeyChooser();
		void testMountChooser();

		//
		void testChannelSelectWidget();
		void testBEWidget();
		void testAVSelectWidget();
		void testAudioSelectWidget();
		void testDVBSubSelectWidget();
		void testAlphaSetupWidget();
		void testPSISetup();
		void testRCLock();
		void testSleepTimerWidget();
		void testMountGUI();
		void testUmountGUI();
		void testMountSmallMenu();
		void testPluginsList();

		//
		void testStartPlugin();

		//
		void testShowActuellEPG();

		//
		void testPlayMovieURL();
		void testPlayAudioURL();
		void testShowPictureURL();

		// players
		void testPlayMovieFolder();
		void testPlayAudioFolder();
		void testShowPictureFolder();

		//
		void testPlayMovieDir();
		void testPlayAudioDir();
		void testShowPictureDir();

		// channel/bouquet list
		void testChannellist();
		void testBouquetlist();

	public:
		CTestMenu();
		~CTestMenu();
		int exec(CMenuTarget* parent, const std::string& actionKey);

		// paint()
		void showMenu();

		// hide()
		void hide();
};

#define HEAD_BUTTONS_COUNT	3
const struct button_label HeadButtons[HEAD_BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_HELP, NONEXISTANT_LOCALE, NULL },
	{ NEUTRINO_ICON_BUTTON_SETUP, NONEXISTANT_LOCALE, NULL },
	{ NEUTRINO_ICON_BUTTON_MUTE_SMALL, NONEXISTANT_LOCALE, NULL}
};

#define FOOT_BUTTONS_COUNT	4
const struct button_label FootButtons[FOOT_BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_RED, NONEXISTANT_LOCALE, "next Page" },
	{ NEUTRINO_ICON_BUTTON_GREEN, NONEXISTANT_LOCALE, "prev Page" },
	{ NEUTRINO_ICON_BUTTON_YELLOW, NONEXISTANT_LOCALE, "Focus" },
	{ NEUTRINO_ICON_BUTTON_BLUE, NONEXISTANT_LOCALE, "Neue Filme" },
	
};

CTestMenu::CTestMenu()
{
	frameBuffer = CFrameBuffer::getInstance();

	selected = 0;

	//
	webTVchannelList = NULL;
	webTVBouquetList = NULL;

	//
	listMenu = NULL;
	item = NULL;

	//
	plist = "popular";
	page = 1;
	TVShows = "movies";

	//
	headers = NULL;
	footers = NULL;
	testWidget = NULL;
	frameBoxWidget = NULL;
	leftWidget = NULL;
	rightWidget = NULL;
	listFrame = NULL;
	textWidget = NULL;
	windowWidget = NULL;
	
	//
	progressWindow = NULL;
}

CTestMenu::~CTestMenu()
{
	filelist.clear();
	fileFilter.clear();
	m_vMovieInfo.clear();

	if(webTVchannelList)
	{
		delete webTVchannelList;
		webTVchannelList = NULL;
	}

	if(webTVBouquetList)
	{
		delete webTVBouquetList;
		webTVBouquetList = NULL;
	}
}

void CTestMenu::hide()
{
	frameBuffer->paintBackground();
	frameBuffer->blit();
}

//// helpers functions
void CTestMenu::loadAudioPlaylist()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:loadAudioPlaylist:\n");

	fileFilter.clear();
	filelist.clear();
	AudioPlaylist.clear();

	fileFilter.addFilter("cdr");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("m2a");
	fileFilter.addFilter("mpa");
	fileFilter.addFilter("mp2");
	fileFilter.addFilter("ogg");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("aac");
	fileFilter.addFilter("dts");
	fileFilter.addFilter("m4a");

	std::string Path = g_settings.network_nfs_audioplayerdir;

	//if(CFileHelpers::getInstance()->readDir(Path, &filelist, &fileFilter))
	CFileHelpers::getInstance()->addRecursiveDir(&filelist, Path, &fileFilter);

	if(filelist.size() > 0)
	{		
		CFileList::iterator files = filelist.begin();
		for(; files != filelist.end() ; files++)
		{
			if ( (files->getExtension() == CFile::EXTENSION_CDR)
					||  (files->getExtension() == CFile::EXTENSION_MP3)
					||  (files->getExtension() == CFile::EXTENSION_WAV)
					||  (files->getExtension() == CFile::EXTENSION_FLAC))
			{
				CAudiofile audiofile(files->Name, files->getExtension());

				// refill
				std::string title;
				std::string artist;
				std::string genre;
				std::string date;
				char duration[9] = "";

				CAudioPlayer::getInstance()->init();

				int ret = CAudioPlayer::getInstance()->readMetaData(&audiofile, true);

				if (!ret || (audiofile.MetaData.artist.empty() && audiofile.MetaData.title.empty() ))
				{
					//remove extension (.mp3)
					std::string tmp = files->getFileName().substr(files->getFileName().rfind('/') + 1);
					tmp = tmp.substr(0, tmp.length() - 4);	//remove extension (.mp3)

					std::string::size_type i = tmp.rfind(" - ");
		
					if(i != std::string::npos)
					{ 
						audiofile.MetaData.title = tmp.substr(0, i);
						audiofile.MetaData.artist = tmp.substr(i + 3);
					}
					else
					{
						i = tmp.rfind('-');
						if(i != std::string::npos)
						{
							audiofile.MetaData.title = tmp.substr(0, i);
							audiofile.MetaData.artist = tmp.substr(i + 1);
						}
						else
							audiofile.MetaData.title = tmp;
					}
				}
				
				AudioPlaylist.push_back(audiofile);
			}
		}
	}
}

void CTestMenu::openAudioFileBrowser()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:loadAudioFileBrowser:\n");
	
	CFileBrowser filebrowser((g_settings.filebrowser_denydirectoryleave) ? g_settings.network_nfs_audioplayerdir : "");

	fileFilter.clear();
	filelist.clear();
	AudioPlaylist.clear();

	fileFilter.addFilter("cdr");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("m2a");
	fileFilter.addFilter("mpa");
	fileFilter.addFilter("mp2");
	fileFilter.addFilter("ogg");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("aac");
	fileFilter.addFilter("dts");
	fileFilter.addFilter("m4a");

	filebrowser.Multi_Select = true;
	filebrowser.Dirs_Selectable = true;
	filebrowser.Filter = &fileFilter;

	std::string Path = g_settings.network_nfs_audioplayerdir;

	if (filebrowser.exec(Path.c_str()))
	{
		Path = filebrowser.getCurrentDir();
		CFileList::const_iterator files = filebrowser.getSelectedFiles().begin();
		for(; files != filebrowser.getSelectedFiles().end(); files++)
		{
			if ( (files->getExtension() == CFile::EXTENSION_CDR)
					||  (files->getExtension() == CFile::EXTENSION_MP3)
					||  (files->getExtension() == CFile::EXTENSION_WAV)
					||  (files->getExtension() == CFile::EXTENSION_FLAC))
			{
				CAudiofile audiofile(files->Name, files->getExtension());

				// skip duplicate
				for (unsigned long i = 0; i < AudioPlaylist.size(); i++)
				{
					if(AudioPlaylist[i].Filename == audiofile.Filename)
						AudioPlaylist.erase(AudioPlaylist.begin() + i); 
				}

				// refill
				std::string title;
				std::string artist;
				std::string genre;
				std::string date;
				char duration[9] = "";

				CAudioPlayer::getInstance()->init();

				int ret = CAudioPlayer::getInstance()->readMetaData(&audiofile, true);

				if (!ret || (audiofile.MetaData.artist.empty() && audiofile.MetaData.title.empty() ))
				{
					//remove extension (.mp3)
					std::string tmp = files->getFileName().substr(files->getFileName().rfind('/') + 1);
					tmp = tmp.substr(0, tmp.length() - 4);	//remove extension (.mp3)

					std::string::size_type i = tmp.rfind(" - ");
		
					if(i != std::string::npos)
					{ 
						audiofile.MetaData.title = tmp.substr(0, i);
						audiofile.MetaData.artist = tmp.substr(i + 3);
					}
					else
					{
						i = tmp.rfind('-');
						if(i != std::string::npos)
						{
							audiofile.MetaData.title = tmp.substr(0, i);
							audiofile.MetaData.artist = tmp.substr(i + 1);
						}
						else
							audiofile.MetaData.title = tmp;
					}
				}
		
				AudioPlaylist.push_back(audiofile);
			}
		}
	}
}

void CTestMenu::loadMoviePlaylist()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:loadMoviePlaylist:\n");
	
	fileFilter.clear();
	filelist.clear();
	m_vMovieInfo.clear();

	fileFilter.addFilter("ts");
	fileFilter.addFilter("mpg");
	fileFilter.addFilter("mpeg");
	fileFilter.addFilter("divx");
	fileFilter.addFilter("avi");
	fileFilter.addFilter("mkv");
	fileFilter.addFilter("asf");
	fileFilter.addFilter("aiff");
	fileFilter.addFilter("m2p");
	fileFilter.addFilter("mpv");
	fileFilter.addFilter("m2ts");
	fileFilter.addFilter("vob");
	fileFilter.addFilter("mp4");
	fileFilter.addFilter("mov");	
	fileFilter.addFilter("flv");	
	fileFilter.addFilter("dat");
	fileFilter.addFilter("trp");
	fileFilter.addFilter("vdr");
	fileFilter.addFilter("mts");
	fileFilter.addFilter("wmv");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("wma");
	fileFilter.addFilter("ogg");
	
	// recordingdir
	std::string Path = g_settings.network_nfs_recordingdir;
	m_vMovieInfo.clear();
	
	//
	if(CFileHelpers::getInstance()->readDir(Path, &filelist, &fileFilter))
	{
		// filter them
		MI_MOVIE_INFO movieInfo;
		m_movieInfo.clearMovieInfo(&movieInfo); // refresh structure

		CFileList::iterator files = filelist.begin();
		for(; files != filelist.end() ; files++)
		{
			//
			m_movieInfo.clearMovieInfo(&movieInfo); // refresh structure
					
			movieInfo.file.Name = files->Name;
					
			// load movie infos (from xml file)
			m_movieInfo.loadMovieInfo(&movieInfo);
					
			// 
			m_vMovieInfo.push_back(movieInfo);
		}
	}
}

void CTestMenu::openMovieFileBrowser()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:openMovieFileBrowser:\n");
	
	CFileBrowser filebrowser((g_settings.filebrowser_denydirectoryleave) ? g_settings.network_nfs_recordingdir : "");

	fileFilter.clear();
	filelist.clear();
	m_vMovieInfo.clear();

	fileFilter.addFilter("ts");
	fileFilter.addFilter("mpg");
	fileFilter.addFilter("mpeg");
	fileFilter.addFilter("divx");
	fileFilter.addFilter("avi");
	fileFilter.addFilter("mkv");
	fileFilter.addFilter("asf");
	fileFilter.addFilter("aiff");
	fileFilter.addFilter("m2p");
	fileFilter.addFilter("mpv");
	fileFilter.addFilter("m2ts");
	fileFilter.addFilter("vob");
	fileFilter.addFilter("mp4");
	fileFilter.addFilter("mov");	
	fileFilter.addFilter("flv");	
	fileFilter.addFilter("dat");
	fileFilter.addFilter("trp");
	fileFilter.addFilter("vdr");
	fileFilter.addFilter("mts");
	fileFilter.addFilter("wmv");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("wma");
	fileFilter.addFilter("ogg");

	filebrowser.Multi_Select = true;
	filebrowser.Dirs_Selectable = true;
	filebrowser.Filter = &fileFilter;

	std::string Path = g_settings.network_nfs_recordingdir;

	if (filebrowser.exec(Path.c_str()))
	{
		Path = filebrowser.getCurrentDir();

		MI_MOVIE_INFO movieInfo;
		m_movieInfo.clearMovieInfo(&movieInfo); // refresh structure

		CFileList::const_iterator files = filebrowser.getSelectedFiles().begin();
		for(; files != filebrowser.getSelectedFiles().end(); files++)
		{
			// filter them
			MI_MOVIE_INFO movieInfo;
			m_movieInfo.clearMovieInfo(&movieInfo); // refresh structure
					
			movieInfo.file.Name = files->Name;
					
			// load movie infos (from xml file)
			m_movieInfo.loadMovieInfo(&movieInfo);

			// skip duplicate
			for (unsigned long i = 0; i < m_vMovieInfo.size(); i++)
			{
				if(m_vMovieInfo[i].file.getFileName() == movieInfo.file.getFileName())
					m_vMovieInfo.erase(m_vMovieInfo.begin() + i); 
			}
					
			// 
			m_vMovieInfo.push_back(movieInfo);
		}
	}
}

void CTestMenu::loadPicturePlaylist()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:loadPicturePlaylist:\n");
	
	fileFilter.clear();
	filelist.clear();
	PicPlaylist.clear();

	fileFilter.addFilter("png");
	fileFilter.addFilter("bmp");
	fileFilter.addFilter("jpg");
	fileFilter.addFilter("jpeg");

	std::string Path = g_settings.network_nfs_picturedir;

	if(CFileHelpers::getInstance()->readDir(Path, &filelist, &fileFilter))
	{
		struct stat statbuf;
				
		CFileList::iterator files = filelist.begin();
		for(; files != filelist.end() ; files++)
		{
			if (files->getType() == CFile::FILE_PICTURE)
			{
				// fill 
				pic.Filename = files->Name;
				std::string tmp = files->Name.substr(files->Name.rfind('/') + 1);
				pic.Name = tmp.substr(0, tmp.rfind('.'));
				pic.Type = tmp.substr(tmp.rfind('.') + 1);
			
				if(stat(pic.Filename.c_str(), &statbuf) != 0)
					printf("stat error");
				pic.Date = statbuf.st_mtime;
				
				PicPlaylist.push_back(pic);
			}
		}
	}
}

void CTestMenu::openPictureFileBrowser()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:loadPictureBrowser:\n");
	
	CFileBrowser filebrowser((g_settings.filebrowser_denydirectoryleave) ? g_settings.network_nfs_picturedir : "");

	fileFilter.clear();
	filelist.clear();
	PicPlaylist.clear();

	fileFilter.addFilter("png");
	fileFilter.addFilter("bmp");
	fileFilter.addFilter("jpg");
	fileFilter.addFilter("jpeg");

	filebrowser.Multi_Select = true;
	filebrowser.Dirs_Selectable = true;
	filebrowser.Filter = &fileFilter;

	std::string Path = g_settings.network_nfs_picturedir;

	if (filebrowser.exec(Path.c_str()))
	{
		Path = filebrowser.getCurrentDir();
		CFileList::const_iterator files = filebrowser.getSelectedFiles().begin();
		for(; files != filebrowser.getSelectedFiles().end(); files++)
		{
			if(files->getType() == CFile::FILE_PICTURE)
			{
				CPicture pic;
				pic.Filename = files->Name;
				std::string tmp = files->Name.substr(files->Name.rfind('/') + 1);
				pic.Name = tmp.substr(0, tmp.rfind('.'));
				pic.Type = tmp.substr(tmp.rfind('.') + 1);
				struct stat statbuf;
				if(stat(pic.Filename.c_str(),&statbuf) != 0)
					printf("stat error");
				pic.Date = statbuf.st_mtime;

				// skip duplicate
				for (unsigned long i = 0; i < PicPlaylist.size(); i++)
				{
					if(PicPlaylist[i].Filename == pic.Filename)
						PicPlaylist.erase(PicPlaylist.begin() + i); 
				}
							
				PicPlaylist.push_back(pic);
			}
		}
	}
}

void CTestMenu::loadTMDBPlaylist(const char *txt, const char *list, const int seite, bool search)
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:loadTMDBPlaylist:\n");
	
	thumbnail_dir = "/tmp/nfilm";
	page = seite;
	plist = list;
	TVShows = txt;

	//
	tmdb = new CTmdb();
	mvlist.clear();

	fileHelper.removeDir(thumbnail_dir.c_str());
	fileHelper.createDir(thumbnail_dir.c_str(), 0755);

	if(search)
	{
		tmdb->clearMInfo();
		tmdb->searchMovieInfo(txt);

		mvlist = tmdb->getMInfos();
	}
	else
	{
		tmdb->clearMovieList();
		tmdb->getMovieTVList(TVShows, plist, page);

		mvlist = tmdb->getMovies();
	}

	if (mvlist.empty())
		return;

	m_vMovieInfo.clear();
	
	// fill our structure
	for(unsigned int i = 0; i < mvlist.size(); i++)
	{
		MI_MOVIE_INFO movieInfo;
		m_movieInfo.clearMovieInfo(&movieInfo);

		movieInfo.epgTitle = mvlist[i].title;
		
		movieInfo.epgInfo1 = mvlist[i].overview;
		movieInfo.ytdate = mvlist[i].release_date;
		movieInfo.vote_average = mvlist[i].vote_average;
		movieInfo.vote_count = mvlist[i].vote_count;
		movieInfo.original_title = mvlist[i].original_title;
		movieInfo.release_date = mvlist[i].release_date;
		movieInfo.media_type = mvlist[i].media_type;
		movieInfo.length = mvlist[i].runtime;
		movieInfo.runtimes = mvlist[i].runtimes;
		movieInfo.genres = mvlist[i].genres;
		movieInfo.cast = mvlist[i].cast;
		movieInfo.seasons = mvlist[i].seasons;
		movieInfo.episodes = mvlist[i].episodes;
			
		std::string tname = thumbnail_dir;
		tname += "/";
		tname += movieInfo.epgTitle;
		tname += ".jpg";

		tmdb->getSmallCover(mvlist[i].poster_path, tname);

		if(!tname.empty())
			movieInfo.tfile = tname;

		// video url (the first one)
		tmdb->clearVideoInfo();
		tmdb->getVideoInfo("movie", mvlist[i].id);

		std::vector<tmdbinfo>& videoInfo_list = tmdb->getVideoInfos();

		movieInfo.vid = videoInfo_list[0].vid;
		movieInfo.vkey = videoInfo_list[0].vkey;
		movieInfo.vname = videoInfo_list[0].vname;

		m_vMovieInfo.push_back(movieInfo);
	}
}

////
void CTestMenu::testCWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:testCWidget:\n");
	
	testWidget = new CWidget(frameBuffer->getScreenX(), frameBuffer->getScreenY(), frameBuffer->getScreenWidth(), frameBuffer->getScreenHeight());

	testWidget->enableSaveScreen();
	testWidget->setSelected(selected);
	testWidget->setBackgroundColor(COL_DARK_TURQUOISE);
	testWidget->enablePaintMainFrame();

	// head
	headBox.iWidth = frameBuffer->getScreenWidth();
	headBox.iHeight = 40;
	headBox.iX = frameBuffer->getScreenX();
	headBox.iY = frameBuffer->getScreenY();

	headers = new CHeaders(headBox.iX, headBox.iY, headBox.iWidth, headBox.iHeight, "CWidget(Multi Widget)", NEUTRINO_ICON_MP3);

	headers->setButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	headers->enablePaintDate();

	// foot
	footBox.iWidth = frameBuffer->getScreenWidth();
	footBox.iHeight = 40;
	footBox.iX = frameBuffer->getScreenX();
	footBox.iY = frameBuffer->getScreenY() + frameBuffer->getScreenHeight() - footBox.iHeight;

	footers = new CFooters(footBox.iX, footBox.iY, footBox.iWidth, footBox.iHeight);
	footers->setButtons(FootButtons, FOOT_BUTTONS_COUNT);

	footers->setCorner(RADIUS_MID, CORNER_BOTTOM);
	
	// topwidget (frameBox)
	topBox.iWidth = testWidget->getWindowsPos().iWidth;
	topBox.iHeight = 50;
	topBox.iX = testWidget->getWindowsPos().iX;
	topBox.iY = testWidget->getWindowsPos().iY + headBox.iHeight + INTER_FRAME_SPACE;

	top_selected = 0;

	frameBoxWidget = new CFrameBox(&topBox);
	//frameBoxWidget->setBackgroundColor(COL_DARK_TURQUOISE);
	frameBoxWidget->setOutFocus();

	CFrame * frame = NULL;

	frame = new CFrame();
	frame->setPosition(topBox.iX, topBox.iY, topBox.iWidth/3, topBox.iHeight);
	frame->setTitle("Filme");
	frame->setActionKey(this, "movie");
	frameBoxWidget->addFrame(frame);
	
	frame = new CFrame();
	frame->setPosition(topBox.iX + topBox.iWidth/3, topBox.iY, topBox.iWidth/3, topBox.iHeight);
	frame->setTitle("Serien");
	frame->setActionKey(this, "tv");
	frameBoxWidget->addFrame(frame);

	frame = new CFrame();
	frame->setPosition(topBox.iX + 2*topBox.iWidth/3, topBox.iY, topBox.iWidth/3, topBox.iHeight);
	frame->setTitle("Suche");
	frame->setOption(tmdbsearch.c_str());
	frame->setActionKey(this, "search");
	frameBoxWidget->addFrame(frame);

	frameBoxWidget->setSelected(top_selected); 

	// leftWidget (listBox)
	leftBox.iWidth = 200;
	leftBox.iHeight = testWidget->getWindowsPos().iHeight - headBox.iHeight - INTER_FRAME_SPACE - topBox.iHeight - 2*INTER_FRAME_SPACE - footBox.iHeight;
	leftBox.iX = testWidget->getWindowsPos().iX;
	leftBox.iY = testWidget->getWindowsPos().iY + headBox.iHeight + INTER_FRAME_SPACE + topBox.iHeight + INTER_FRAME_SPACE;

	left_selected = 0;

	leftWidget = new ClistBox(&leftBox);

	leftWidget->setSelected(left_selected);
	leftWidget->setOutFocus();

	ClistBoxItem *item1 = new ClistBoxItem("In den Kinos", true, NULL, this, "movie_in_cinema");
	ClistBoxItem *item2 = new ClistBoxItem("Am", true, NULL, this, "movie_popular");
	item2->setOption("populärsten");
	item2->set2lines();
	ClistBoxItem *item3 = new ClistBoxItem("Am besten", true, NULL, this, "movie_top_rated");
	item3->setOption("bewertet");
	item3->set2lines();
	ClistBoxItem *item4 = new ClistBoxItem("Neue Filme", true, NULL, this, "movie_new");
	CMenuSeparator *item5 = new CMenuSeparator();
	CMenuSeparator *item6 = new CMenuSeparator();
	CMenuSeparator *item7 = new CMenuSeparator();
	CMenuSeparator *item8 = new CMenuSeparator();
	ClistBoxItem *item9 = new ClistBoxItem("Beenden", true, NULL, this, "exit");

	leftWidget->addItem(item1);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item2);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item3);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item4);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item5);
	leftWidget->addItem(item6);
	leftWidget->addItem(item7);
	leftWidget->addItem(item8);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item9);
	leftWidget->addItem(new CMenuSeparator(LINE));

	// rightwidget (listBox)
	rightBox.iWidth = testWidget->getWindowsPos().iWidth - INTER_FRAME_SPACE - leftBox.iWidth;
	rightBox.iHeight = testWidget->getWindowsPos().iHeight - headBox.iHeight - INTER_FRAME_SPACE - topBox.iHeight - 2*INTER_FRAME_SPACE - footBox.iHeight;
	rightBox.iX = testWidget->getWindowsPos().iX + leftBox.iWidth + INTER_FRAME_SPACE;
	rightBox.iY = testWidget->getWindowsPos().iY + headBox.iHeight + INTER_FRAME_SPACE + topBox.iHeight + INTER_FRAME_SPACE;

	right_selected = 0;

	//
	rightWidget = new ClistBox(&rightBox);
	rightWidget->setWidgetType(WIDGET_TYPE_FRAME);
	rightWidget->setItemsPerPage(6,2);
	rightWidget->setSelected(right_selected);
	rightWidget->enablePaintFootInfo();

	// loadPlaylist
	CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	
	loadTMDBPlaylist();
	
	loadBox.hide();

	// load items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

		item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->set2lines();

		std::string tmp = m_vMovieInfo[i].epgTitle;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo1;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo2;

		item->setHint(tmp.c_str());

		rightWidget->addItem(item);
	}

	testWidget->addItem(headers);
	testWidget->addItem(frameBoxWidget);
	testWidget->addItem(leftWidget);
	testWidget->addItem(rightWidget);
	testWidget->addItem(footers);

	testWidget->addKey(RC_info, this, "winfo");
	testWidget->addKey(RC_red, this, "nextPage");
	testWidget->addKey(RC_green, this, "prevPage");

	testWidget->exec(NULL, "");

	delete testWidget;
	testWidget = NULL;

	delete frameBoxWidget;
	frameBoxWidget = NULL;

	delete leftWidget;
	leftWidget = NULL;

	delete rightWidget;
	rightWidget = NULL;

	delete headers;
	headers = NULL;

	delete footers;
	footers = NULL;
}

void CTestMenu::testSingleWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:testSingleWidget:\n");
	
	// mainBox
	CBox box;
	box.iX = CFrameBuffer::getInstance()->getScreenX() + 40;
	box.iY = CFrameBuffer::getInstance()->getScreenY() + 40;
	box.iWidth = CFrameBuffer::getInstance()->getScreenWidth() - 80;
	box.iHeight = CFrameBuffer::getInstance()->getScreenHeight() - 80;

	//testWidget = new CWidget(&box);

	//testWidget->setBackgroundColor(COL_DARK_TURQUOISE);
	//testWidget->enablePaintMainFrame();

	frameBoxWidget = new CFrameBox(&box);
	//testFrame->disablePaintFrame();

	CHintBox loadBox("singleWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	
	loadMoviePlaylist();
	
	loadBox.hide();

	// titleBox
	CBox titleBox;
	titleBox.iX = box.iX + 10;
	titleBox.iY = box.iY + 40 + 10;
	titleBox.iWidth = box.iWidth;
	titleBox.iHeight = 40;

	// starBox
	CBox starBox;
	starBox.iX = box.iX +10;
	starBox.iY = box.iY + titleBox.iHeight + 40 + 10;
	starBox.iWidth = 25;
	starBox.iHeight = 25;

	// playBox
	CBox playBox;
	playBox.iWidth = 300;
	playBox.iHeight = 60;
	playBox.iX = box.iX + 10;
	playBox.iY = box.iY + box.iHeight - 10 - 40 - 60;

	// textBox
	CBox textBox;
	textBox.iWidth = box.iWidth/2 - 20;
	textBox.iHeight = box.iHeight - playBox.iHeight - 80 - titleBox.iHeight - starBox.iHeight - 4*10 - 100;
	textBox.iX = box.iX + 10 + 40;
	textBox.iY = starBox.iY + 20;

	// head
	CFrame *headFrame = new CFrame();
	headFrame->setMode(FRAME_HEAD);
	headFrame->setPosition(box.iX, box.iY, box.iWidth, 40);
	headFrame->setIconName(NEUTRINO_ICON_MOVIE);
	headFrame->setTitle("single Widget");
	headFrame->enablePaintDate();
	headFrame->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	headFrame->setActive(false);

	frameBoxWidget->addFrame(headFrame);

	// artFrame
	CFrame * artFrame = new CFrame();
	artFrame->setMode(FRAME_PICTURE);
	artFrame->setPosition(box.iX + box.iWidth/2, box.iY + 40, box.iWidth/2, box.iHeight - 2*40);
	artFrame->setIconName(m_vMovieInfo[0].tfile.c_str());
	artFrame->setActive(false);

	frameBoxWidget->addFrame(artFrame);

	// title
	CFrame *titleFrame = new CFrame();
	titleFrame->setMode(FRAME_LABEL);
	titleFrame->setPosition(&titleBox);
	titleFrame->disablePaintFrame();
	titleFrame->setTitle(m_vMovieInfo[0].epgTitle.c_str());
	titleFrame->setCaptionFont(g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]);
	titleFrame->setActive(false);

	frameBoxWidget->addFrame(titleFrame);

	// star1
	CFrame *star1Frame = new CFrame();
	star1Frame->setMode(FRAME_ICON);
	star1Frame->setPosition(&starBox);
	star1Frame->setIconName(NEUTRINO_ICON_STAR_ON);
	star1Frame->disablePaintFrame();
	star1Frame->setActive(false);

	frameBoxWidget->addFrame(star1Frame);

	// star2
	CFrame *star2Frame = new CFrame();
	star2Frame->setMode(FRAME_ICON);
	star2Frame->setPosition(starBox.iX + 25, starBox.iY, starBox.iWidth, starBox.iHeight);
	star2Frame->setIconName(NEUTRINO_ICON_STAR_ON);
	star2Frame->disablePaintFrame();
	star2Frame->setActive(false);

	frameBoxWidget->addFrame(star2Frame);

	// star3
	CFrame *star3Frame = new CFrame();
	star3Frame->setMode(FRAME_ICON);
	star3Frame->setPosition(starBox.iX + 2*25, starBox.iY, starBox.iWidth, starBox.iHeight);
	star3Frame->setIconName(NEUTRINO_ICON_STAR_ON);
	star3Frame->disablePaintFrame();
	star3Frame->setActive(false);

	frameBoxWidget->addFrame(star3Frame);

	// star4
	CFrame *star4Frame = new CFrame();
	star4Frame->setMode(FRAME_ICON);
	star4Frame->setPosition(starBox.iX + 3*25, starBox.iY, starBox.iWidth, starBox.iHeight);
	star4Frame->setIconName(NEUTRINO_ICON_STAR_OFF);
	star4Frame->disablePaintFrame();
	star4Frame->setActive(false);

	frameBoxWidget->addFrame(star4Frame);

	// text
	CFrame *textFrame = new CFrame();
	textFrame->setMode(FRAME_TEXT);
	textFrame->setPosition(&textBox);
	std::string buffer;
	buffer = m_vMovieInfo[0].epgInfo1;
	buffer += "\n";
	buffer += m_vMovieInfo[0].epgInfo2;

	textFrame->setTitle(buffer.c_str());
	textFrame->disablePaintFrame();
	textFrame->setActive(false);
	
	frameBoxWidget->addFrame(textFrame);

	// infoFrame
	CFrame * infoFrame = new CFrame();
	infoFrame->setPosition(playBox.iX + 300 + 10, playBox.iY, 300, 60);
	infoFrame->setCaptionFont(g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1]);
	infoFrame->setTitle("Movie Details");
	infoFrame->setIconName(NEUTRINO_ICON_INFO);
	infoFrame->setActionKey(this, "minfo");

	frameBoxWidget->addFrame(infoFrame);

	// play
	CFrame *playFrame = new CFrame();
	playFrame->setPosition(&playBox);
	playFrame->setCaptionFont(g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1]);
	playFrame->setTitle("Movie abspielen");
	playFrame->setIconName(NEUTRINO_ICON_PLAY);
	playFrame->setActionKey(this, "mplay");

	frameBoxWidget->addFrame(playFrame);

	// foot
	CFrame *footFrame = new CFrame();
	footFrame->setMode(FRAME_FOOT);
	footFrame->setPosition(box.iX, box.iY + box.iHeight - 40, box.iWidth, 40);
	footFrame->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);
	footFrame->setActive(false);

	frameBoxWidget->addFrame(footFrame);

	//testWidget->addItem(testFrame);

	frameBoxWidget->exec(NULL, "");
	
	delete frameBoxWidget;
	frameBoxWidget = NULL;
}

void CTestMenu::testFireTV()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:testFireTV:\n");
	
	// mainBox
	CBox box;
	box.iX = CFrameBuffer::getInstance()->getScreenX();
	box.iY = CFrameBuffer::getInstance()->getScreenY();
	box.iWidth = CFrameBuffer::getInstance()->getScreenWidth();
	box.iHeight = CFrameBuffer::getInstance()->getScreenHeight();

	int pic_w = box.iWidth/6;

	//testWidget = new CWidget(&box);
	//testWidget->enablePaintMainFrame();

	// frameBox
	frameBoxWidget = new CFrameBox(&box);
	//frameBoxWidget->disablePaintFrame();

	CHintBox loadBox("FireTV", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	
	loadMoviePlaylist();
	
	loadBox.hide();

	// home
	CFrame *homeFrame = new CFrame();
	homeFrame->setMode(FRAME_LABEL);
	homeFrame->setCaptionFont(g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]);
	int h_w = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getRenderWidth("Home");
	int h_h = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight();
	homeFrame->setPosition(box.iX + 10, box.iY + 40, h_w + 10, h_h);
	homeFrame->setTitle("Home");
	homeFrame->disablePaintFrame();
	homeFrame->setActionKey(this, "home");

	frameBoxWidget->addFrame(homeFrame);

	// setup
	CFrame *setupFrame = new CFrame();
	setupFrame->setMode(FRAME_LABEL);
	setupFrame->setCaptionFont(g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]);
	int s_w = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getRenderWidth("Setup");
	int s_h = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight();
	setupFrame->setPosition(box.iX + 10 + 5 + h_w + 20, box.iY + 40, s_w + 10, h_h);
	setupFrame->setTitle("Setup");
	setupFrame->disablePaintFrame();
	setupFrame->setActionKey(this, "setup");

	frameBoxWidget->addFrame(setupFrame);

	// help
	CFrame *helpFrame = new CFrame();
	helpFrame->setMode(FRAME_LABEL);
	int i_w = 0;
	int i_h = 0;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_INFO, &i_w, &i_h);
	helpFrame->setPosition(box.iX + 10 + 5 + h_w + 10 + s_w + 40, box.iY + 40, i_w + 4, h_h);
	//helpFrame->setIconName(NEUTRINO_ICON_INFO);
	helpFrame->setTitle("?");
	helpFrame->disablePaintFrame();
	helpFrame->setActionKey(this, "help");

	frameBoxWidget->addFrame(helpFrame);

	// frameBox1
	if (!m_vMovieInfo.empty())
	{
		// title
		CFrame * titleFrame = new CFrame();
		titleFrame->setMode(FRAME_LABEL);
		titleFrame->setCaptionFont(g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]);
		int t_w = 200;
		int t_h = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight();
		titleFrame->setPosition(box.iX + 10, box.iY + 40 + h_h + 10, box.iWidth - 20 - pic_w - 20, t_h);
		titleFrame->setTitle((m_vMovieInfo[0].epgTitle.empty())? "" : m_vMovieInfo[0].epgTitle.c_str());
		titleFrame->disablePaintFrame();
		titleFrame->setActive(false);

		frameBoxWidget->addFrame(titleFrame);

		// text
		CFrame *textFrame = new CFrame();
		textFrame->setMode(FRAME_TEXT);
		textFrame->setPosition(box.iX + 10, box.iY + 40 + h_h + 10 + t_h + 10, box.iWidth - 20 - pic_w - 20, 250 - t_h - 10 - 10);
		std::string buffer;
		buffer = m_vMovieInfo[0].epgInfo1;
		buffer += "\n";
		buffer += m_vMovieInfo[0].epgInfo2;
		textFrame->setTitle(buffer.c_str());
		textFrame->disablePaintFrame();
		textFrame->setActive(false);

		frameBoxWidget->addFrame(textFrame);

		// pic
		CFrame * artFrame = new CFrame();
		artFrame->setMode(FRAME_PICTURE);
		artFrame->setPosition(box.iX + 10 + box.iWidth - 10 - pic_w - 20, box.iY + 40 + h_h + 10, pic_w - 20, 250);
		artFrame->setIconName(m_vMovieInfo[0].tfile.c_str());
		artFrame->disablePaintFrame();
		artFrame->setActionKey(this, "fire1play");

		frameBoxWidget->addFrame(artFrame);
		

		// other
		CFrame *otherFrame = new CFrame();
		otherFrame->setMode(FRAME_LABEL);
		otherFrame->setCaptionFont(g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]);
		int o_w = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getRenderWidth("andere Filme:");
		int o_h = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight();
		otherFrame->setPosition(box.iX + 10, box.iY + 40 + h_h + 10 + 250 + 10, o_w + 10, o_h);
		otherFrame->setTitle("andere Filme:");
		otherFrame->disablePaintFrame();
		otherFrame->setActive(false);

		frameBoxWidget->addFrame(otherFrame);

		//
		CFrame * art1Frame = NULL;
		for (int i = 1; i < 7; i++)
		{
			art1Frame = new CFrame();
			art1Frame->setMode(FRAME_PICTURE);
			art1Frame->setPosition(box.iX + 10 + (i - 1)*((box.iWidth - 20)/6), box.iY + 40 + h_h + 10 + 250 + 10 + o_h + 10, (box.iWidth - 20)/6,box.iHeight - 40 - h_h - 10 - 250 - 10 - 40);
			art1Frame->setIconName(m_vMovieInfo[i].tfile.c_str());
			art1Frame->disablePaintFrame();
			art1Frame->setActionKey(this, "fireplay");
			art1Frame->setTitle(m_vMovieInfo[i].epgTitle.c_str());

			frameBoxWidget->addFrame(art1Frame);
		}
	}

	//testWidget->addItem(frameBoxWidget);
	
	//testWidget->exec(NULL, "");
	frameBoxWidget->exec(NULL, "");

	delete frameBoxWidget;
	frameBoxWidget = NULL;

	//delete testWidget;
	//testWidget = NULL;
}

void CTestMenu::testListFrameWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestTMenu:testListFrameWidget:\n");
	
	testWidget = new CWidget(frameBuffer->getScreenX(), frameBuffer->getScreenY(), frameBuffer->getScreenWidth(), frameBuffer->getScreenHeight());

	testWidget->setBackgroundColor(COL_DARK_TURQUOISE);

	// head
	headBox.iWidth = frameBuffer->getScreenWidth();
	headBox.iHeight = 40;
	headBox.iX = frameBuffer->getScreenX();
	headBox.iY = frameBuffer->getScreenY();

	headers = new CHeaders(headBox.iX, headBox.iY, headBox.iWidth, headBox.iHeight, "CWidget(ClistFrame)", NEUTRINO_ICON_MP3);

	headers->setButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	headers->enablePaintDate();

	// foot
	footBox.iWidth = frameBuffer->getScreenWidth();
	footBox.iHeight = 40;
	footBox.iX = frameBuffer->getScreenX();
	footBox.iY = frameBuffer->getScreenY() + frameBuffer->getScreenHeight() - footBox.iHeight;

	footers = new CFooters(footBox.iX, footBox.iY, footBox.iWidth, footBox.iHeight);

	footers->setButtons(FootButtons, FOOT_BUTTONS_COUNT);
	footers->setCorner(RADIUS_MID, CORNER_BOTTOM);

	//
	CBox listFrameBox;
	LF_LINES listFrameLines;
	int selected = 0;
	
	listFrameBox.iX = frameBuffer->getScreenX();
	listFrameBox.iY = frameBuffer->getScreenY() + headBox.iHeight;
	listFrameBox.iWidth = frameBuffer->getScreenWidth();
	listFrameBox.iHeight = frameBuffer->getScreenHeight() - headBox.iHeight - footBox.iHeight;

	//
#define MAX_ROWS 		LF_MAX_ROWS //6

	// init
	listFrameLines.rows = MAX_ROWS;

	for(int row = 0; row < MAX_ROWS; row++)
	{
		listFrameLines.lineArray[row].clear();
	}

	// rowwidth
	listFrameLines.rowWidth[0] = MAX_WINDOW_WIDTH / 20;
	listFrameLines.rowWidth[1] = MAX_WINDOW_WIDTH / 3;
	listFrameLines.rowWidth[2] = MAX_WINDOW_WIDTH / 10;
	listFrameLines.rowWidth[3] = MAX_WINDOW_WIDTH / 8;
	listFrameLines.rowWidth[4] = MAX_WINDOW_WIDTH / 5;
	listFrameLines.rowWidth[5] = MAX_WINDOW_WIDTH / 10;

	// headertitle
	listFrameLines.lineHeader[0] = "Nr";
	listFrameLines.lineHeader[1] = "title";
	listFrameLines.lineHeader[2] = "duration";
	listFrameLines.lineHeader[3] = "genre";
	listFrameLines.lineHeader[4] = "artist";
	listFrameLines.lineHeader[5] = "date";
	

	listFrame = new CListFrame(&listFrameLines, NULL, CListFrame::CListFrame::HEADER_LINE | CListFrame::SCROLL, &listFrameBox);

	// title
	//listFrame->setTitle("CWidget(ClistFrame)", NEUTRINO_ICON_MOVIE);

	CHintBox loadBox("ListFrame Widget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	
	loadAudioPlaylist();
	
	loadBox.hide();

	//
	int count = 0;
	for (unsigned int i = 0; i < AudioPlaylist.size(); i++)
	{
		std::string title;
		std::string artist;
		std::string genre;
		std::string date;
		char duration[9] = "";

		title = AudioPlaylist[i].MetaData.title;
		artist = AudioPlaylist[i].MetaData.artist;
		genre = AudioPlaylist[i].MetaData.genre;	
		date = AudioPlaylist[i].MetaData.date;

		snprintf(duration, 8, "(%ld:%02ld)", AudioPlaylist[i].MetaData.total_time / 60, AudioPlaylist[i].MetaData.total_time % 60);

		listFrameLines.lineArray[0].push_back(to_string(i + 1));
		listFrameLines.lineArray[1].push_back(title);
		listFrameLines.lineArray[2].push_back(duration);
		listFrameLines.lineArray[3].push_back(genre);
		listFrameLines.lineArray[4].push_back(artist);
		listFrameLines.lineArray[5].push_back(date);
	}
	
	// fill lineArrays list
	listFrame->setLines(&listFrameLines);

	// set selected line
	listFrame->setSelectedLine(selected);
	
	// paint
	listFrame->showSelection(true);

	//
	testWidget->addItem(headers);
	testWidget->addItem(listFrame);
	testWidget->addItem(footers);

	testWidget->addKey(RC_ok, this, "aok");

	testWidget->exec(NULL, "");

	delete testWidget;
	testWidget = NULL;

	delete listFrame;
	listFrame = NULL;

	delete headers;
	headers = NULL;

	delete footers;
	footers = NULL;
}

void CTestMenu::testListBoxWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:testClistBoxWidget:\n");

	//
	rightBox.iWidth = frameBuffer->getScreenWidth();
	rightBox.iHeight = frameBuffer->getScreenHeight();
	rightBox.iX = frameBuffer->getScreenX();
	rightBox.iY = frameBuffer->getScreenY();

	rightWidget = new ClistBox(&rightBox);

	rightWidget->setWidgetType(WIDGET_TYPE_FRAME);
	rightWidget->enableWidgetChange();
	rightWidget->addWidget(WIDGET_TYPE_STANDARD);
	rightWidget->addWidget(WIDGET_TYPE_CLASSIC);
	rightWidget->addWidget(WIDGET_TYPE_EXTENDED);
	rightWidget->enableShrinkMenu();
	rightWidget->enableCenterPos();
	rightWidget->setItemsPerPage(6,2);
	rightWidget->setSelected(selected);
	rightWidget->enablePaintHead();
	rightWidget->setTitle("CWidget(ClistBox)", NEUTRINO_ICON_MP3);
	rightWidget->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	rightWidget->enablePaintDate();
	rightWidget->enablePaintFoot();
	rightWidget->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);
	rightWidget->enablePaintFootInfo(); //FIXME:
	rightWidget->setFootInfoMode(FOOT_HINT_MODE);

	// loadPlaylist
	CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	
	loadMoviePlaylist();
	
	loadBox.hide();

	// load items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "mmwplay");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

		item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->set2lines();

		std::string tmp = m_vMovieInfo[i].epgTitle;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo1;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo2;

		item->setHint(tmp.c_str());

		rightWidget->addItem(item);
	}

	rightWidget->addKey(RC_info, this, "linfo");
	//rightWidget->addKey(RC_setup, this, "lsetup");

	rightWidget->exec(NULL, "");

	delete rightWidget;
	rightWidget = NULL;
}

void CTestMenu::testTextBoxWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu:testCTextBoxWidget:\n");
	
	testWidget = new CWidget(frameBuffer->getScreenX(), frameBuffer->getScreenY(), frameBuffer->getScreenWidth(), frameBuffer->getScreenHeight());

	testWidget->setBackgroundColor(COL_DARK_TURQUOISE);

	// head
	headBox.iWidth = frameBuffer->getScreenWidth();
	headBox.iHeight = 40;
	headBox.iX = frameBuffer->getScreenX();
	headBox.iY = frameBuffer->getScreenY();

	headers = new CHeaders(headBox.iX, headBox.iY, headBox.iWidth, headBox.iHeight, "CWidget(CTextBox)", NEUTRINO_ICON_MP3);

	headers->setButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	//ĥeadersWidget->setCorner(RADIUS_MID, CORNER_TOP);
	headers->enablePaintDate();

	// foot
	footBox.iWidth = frameBuffer->getScreenWidth();
	footBox.iHeight = 40;
	footBox.iX = frameBuffer->getScreenX();
	footBox.iY = frameBuffer->getScreenY() + frameBuffer->getScreenHeight() - footBox.iHeight;

	footers = new CFooters(footBox.iX, footBox.iY, footBox.iWidth, footBox.iHeight);

	footers->setButtons(FootButtons, FOOT_BUTTONS_COUNT);
	footers->setCorner(RADIUS_MID, CORNER_BOTTOM);

	//textBox
	textWidget = new CTextBox(frameBuffer->getScreenX(), frameBuffer->getScreenY() + headBox.iHeight, frameBuffer->getScreenWidth(), frameBuffer->getScreenHeight() - headBox.iHeight - footBox.iHeight);
	
	const char *buffer = NULL;
	
	// prepare print buffer
	buffer = "CTextBox\ntesting CTextBox with CWidget\n";  
		
	std::string tname = PLUGINDIR "/netzkino/netzkino.png";
	
	// scale pic
	int p_w = 0;
	int p_h = 0;
	
	CFrameBuffer::getInstance()->scaleImage(tname, &p_w, &p_h);
	
	textWidget->setText(buffer, tname.c_str(), p_w, p_h);

	testWidget->addKey(RC_info, this, "txtinfo");

	testWidget->addItem(headers);
	testWidget->addItem(textWidget);
	testWidget->addItem(footers);

	testWidget->exec(NULL, "");

	delete testWidget;
	testWidget = NULL;

	delete textWidget;
	textWidget = NULL;

	delete headers;
	headers = NULL;

	delete footers;
	footers = NULL;
}

void CTestMenu::testMultiWidget()
{
	dprintf(DEBUG_NORMAL, "\ntestMultiWidget\n");

	CBox mainBox(frameBuffer->getScreenX(), frameBuffer->getScreenY(), frameBuffer->getScreenWidth(), frameBuffer->getScreenHeight());

	CBox headBox;
	headBox.iX = mainBox.iX;
	headBox.iY = mainBox.iY;
	headBox.iWidth = mainBox.iWidth;
	headBox.iHeight = 40;

	CBox footBox;
	footBox.iHeight = 40;
	footBox.iX = mainBox.iX;
	footBox.iY = mainBox.iY + mainBox.iHeight - footBox.iHeight;
	footBox.iWidth = mainBox.iWidth;

	headers = new CHeaders(headBox, "multiWidget", NEUTRINO_ICON_MP3);

	headers->enablePaintDate();
	headers->setButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	//headers->setColor(COL_DARK_TURQUOISE);
	//headers->setCorner();
	//headers->setGradient(NOGRADIENT);

	footers = new CFooters(footBox);

	footers->setButtons(FootButtons, FOOT_BUTTONS_COUNT);
	//footers->setColor(COL_DARK_TURQUOISE);
	//footers->setCorner();
	//footers->setGradient(NOGRADIENT);
	
	////
	// leftWidget
	leftBox.iWidth = 200;
	leftBox.iHeight = mainBox.iHeight - headBox.iHeight - footBox.iHeight;
	leftBox.iX = mainBox.iX;
	leftBox.iY = mainBox.iY + headBox.iHeight;

	left_selected = 0;

	leftWidget = new ClistBox(&leftBox);

	leftWidget->setSelected(left_selected);
	//leftWidget->setOutFocus();

	ClistBoxItem *item1 = new ClistBoxItem("In den Kinos", true, NULL, this, "movie_in_cinema");
	ClistBoxItem *item2 = new ClistBoxItem("Am", true, NULL, this, "movie_popular");
	item2->setOption("populärsten");
	item2->set2lines();
	ClistBoxItem *item3 = new ClistBoxItem("Am besten", true, NULL, this, "movie_top_rated");
	item3->setOption("bewertet");
	item3->set2lines();
	ClistBoxItem *item4 = new ClistBoxItem("Neue Filme", true, NULL, this, "movie_new");
	CMenuSeparator *item5 = new CMenuSeparator();
	CMenuSeparator *item6 = new CMenuSeparator();
	CMenuSeparator *item7 = new CMenuSeparator();
	CMenuSeparator *item8 = new CMenuSeparator();
	ClistBoxItem *item9 = new ClistBoxItem("Beenden", true, NULL, this, "exit");

	leftWidget->addItem(item1);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item2);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item3);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item4);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item5);
	leftWidget->addItem(item6);
	leftWidget->addItem(item7);
	leftWidget->addItem(item8);
	leftWidget->addItem(new CMenuSeparator(LINE));
	leftWidget->addItem(item9);
	leftWidget->addItem(new CMenuSeparator(LINE));
	
	// rightWidget
	rightBox.iWidth = mainBox.iWidth - leftBox.iWidth;
	rightBox.iHeight = mainBox.iHeight - headBox.iHeight - footBox.iHeight;
	rightBox.iX = mainBox.iX + leftBox.iWidth;
	rightBox.iY = mainBox.iY + headBox.iHeight;
	
	frameBoxWidget = new CFrameBox(&rightBox);
	//frameBoxWidget->disablePaintFrame();
	
	testWidget = new CWidget(&mainBox);
	
	testWidget->addItem(headers);
	testWidget->addItem(footers);
	testWidget->addItem(leftWidget);
	testWidget->addItem(frameBoxWidget);
	
	testWidget->exec(NULL, "");
	
	delete testWidget;
	testWidget = NULL;
	
	delete leftWidget;
	leftWidget = NULL;
}

// test
void CTestMenu::test()
{
	dprintf(DEBUG_NORMAL, "\ntest\n");

	top_selected = 0;
	left_selected = 0;
	right_selected = 0;

	CBox mainBox(frameBuffer->getScreenX(), frameBuffer->getScreenY(), frameBuffer->getScreenWidth(), frameBuffer->getScreenHeight());

	CBox headBox;
	headBox.iX = mainBox.iX;
	headBox.iY = mainBox.iY;
	headBox.iWidth = mainBox.iWidth;
	headBox.iHeight = 40;

	CBox footBox;
	footBox.iHeight = 40;
	footBox.iX = mainBox.iX;
	footBox.iY = mainBox.iY + mainBox.iHeight - footBox.iHeight;
	footBox.iWidth = mainBox.iWidth;

	headers = new CHeaders(headBox, "Movie Trailer", NEUTRINO_ICON_MP3);

	headers->enablePaintDate();
	headers->setButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	//headers->setColor(COL_DARK_TURQUOISE);
	//headers->setCorner();
	//headers->setGradient(NOGRADIENT);

	footers = new CFooters(footBox);

	footers->setButtons(FootButtons, FOOT_BUTTONS_COUNT);
	//footers->setColor(COL_DARK_TURQUOISE);
	//footers->setCorner();
	//footers->setGradient(NOGRADIENT);

	// top widget
	CBox topBox;
	
	topBox.iX = mainBox.iX;
	topBox.iY = headBox.iY + headBox.iHeight;
	topBox.iWidth = mainBox.iWidth;
	topBox.iHeight = 50;

	frameBoxWidget = new CFrameBox(&topBox);
	//frameBoxWidget->setMode(FRAMEBOX_MODE_HORIZONTAL);

	CFrame * frame = NULL;

	frame = new CFrame();
	frame->setPosition(topBox.iX, topBox.iY, topBox.iWidth/3, topBox.iHeight);
	frame->setTitle("Filme");
	frameBoxWidget->addFrame(frame);
	
	frame = new CFrame();
	frame->setPosition(topBox.iX + topBox.iWidth/3, topBox.iY, topBox.iWidth/3, topBox.iHeight);
	frame->setTitle("Serien");
	frameBoxWidget->addFrame(frame);

	frame = new CFrame();
	frame->setPosition(topBox.iX + 2*topBox.iWidth/3, topBox.iY, topBox.iWidth/3, topBox.iHeight);
	frame->setTitle("Suche");
	frame->setOption(tmdbsearch.c_str());
	frame->setActionKey(this, "search");
	frameBoxWidget->addFrame(frame);

	frameBoxWidget->setSelected(top_selected); 
	frameBoxWidget->setOutFocus();

	// leftWidget
	CBox leftBox;

	leftBox.iX = mainBox.iX;
	leftBox.iY = topBox.iY + topBox.iHeight + INTER_FRAME_SPACE;
	leftBox.iWidth = 200;
	leftBox.iHeight = mainBox.iHeight - headBox.iHeight - topBox.iHeight - 2*INTER_FRAME_SPACE - footBox.iHeight;

	leftWidget = new ClistBox(&leftBox);

	leftWidget->setSelected(left_selected);
	leftWidget->setOutFocus();

	//leftWidget->setBackgroundColor(COL_DARK_RED);

	ClistBoxItem *item1 = new ClistBoxItem("In den Kinos");
	ClistBoxItem *item2 = new ClistBoxItem("Am");
	item2->setOption("populärsten");
	item2->set2lines();
	ClistBoxItem *item3 = new ClistBoxItem("Am besten");
	item3->setOption("bewertet");
	item3->set2lines();
	ClistBoxItem *item4 = new ClistBoxItem("Neue Filme");
	ClistBoxItem *item5 = new ClistBoxItem(NULL, false);
	ClistBoxItem *item6 = new ClistBoxItem(NULL, false);
	ClistBoxItem *item7 = new ClistBoxItem(NULL, false);
	ClistBoxItem *item8 = new ClistBoxItem(NULL, false);
	ClistBoxItem *item9 = new ClistBoxItem("Beenden");

	leftWidget->addItem(item1);
	leftWidget->addItem(item2);
	leftWidget->addItem(item3);
	leftWidget->addItem(item4);
	leftWidget->addItem(item5);
	leftWidget->addItem(item6);
	leftWidget->addItem(item7);
	leftWidget->addItem(item8);
	leftWidget->addItem(item9);

	// right menu
	CBox rightBox;

	rightBox.iX = mainBox.iX + leftBox.iWidth + INTER_FRAME_SPACE;
	rightBox.iY = topBox.iY + topBox.iHeight + INTER_FRAME_SPACE;
	rightBox.iWidth = mainBox.iWidth - leftBox.iWidth - INTER_FRAME_SPACE;
	rightBox.iHeight = mainBox.iHeight - headBox.iHeight - topBox.iHeight - 2*INTER_FRAME_SPACE - footBox.iHeight;

	//
	rightWidget = new ClistBox(&rightBox);
	rightWidget->setWidgetType(WIDGET_TYPE_FRAME);
	rightWidget->setItemsPerPage(6,2);
	rightWidget->setSelected(right_selected);
	//rightWidget->setBackgroundColor(COL_LIGHT_BLUE);
	rightWidget->enablePaintFootInfo();

	enum {
		WIDGET_TOP,
		WIDGET_LEFT,
		WIDGET_RIGHT
	};
	
	int focus = WIDGET_RIGHT; // frameBox

	thumbnail_dir = "/tmp/nfilm";
	page = 1;
	plist = "popular";

	//
	tmdb = new CTmdb();

	TVShows = "movie";

DOFILM:
	fileHelper.removeDir(thumbnail_dir.c_str());
	fileHelper.createDir(thumbnail_dir.c_str(), 0755);

	CHintBox loadBox("TEST", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();

	tmdb->clearMovieList();

	tmdb->getMovieTVList(TVShows, plist, page);

	std::vector<tmdbinfo> &mvlist = tmdb->getMovies();

	m_vMovieInfo.clear();
	
	// fill our structure
	for(unsigned int i = 0; i < mvlist.size(); i++)
	{
		MI_MOVIE_INFO movieInfo;
		m_movieInfo.clearMovieInfo(&movieInfo); 

		movieInfo.epgTitle = mvlist[i].title;

		movieInfo.epgInfo1 = mvlist[i].overview;
		movieInfo.ytdate = mvlist[i].release_date;
		movieInfo.vote_average = mvlist[i].vote_average;
		movieInfo.vote_count = mvlist[i].vote_count;
		movieInfo.original_title = mvlist[i].original_title;
		movieInfo.release_date = mvlist[i].release_date;
		movieInfo.media_type = mvlist[i].media_type;
		movieInfo.length = mvlist[i].runtime;
		movieInfo.runtimes = mvlist[i].runtimes;
		movieInfo.genres = mvlist[i].genres;
		movieInfo.cast = mvlist[i].cast;
		movieInfo.seasons = mvlist[i].seasons;
		movieInfo.episodes = mvlist[i].episodes;
			
		std::string tname = thumbnail_dir;
		tname += "/";
		tname += movieInfo.epgTitle;
		tname += ".jpg";

		tmdb->getSmallCover(mvlist[i].poster_path, tname);

		if(!tname.empty())
			movieInfo.tfile = tname;

		// video url
		tmdb->clearVideoInfo();
		tmdb->getVideoInfo("movie", mvlist[i].id);

		std::vector<tmdbinfo>& videoInfo_list = tmdb->getVideoInfos();

		movieInfo.vid = videoInfo_list[0].vid;
		movieInfo.vkey = videoInfo_list[0].vkey;
		movieInfo.vname = videoInfo_list[0].vname;

		m_vMovieInfo.push_back(movieInfo);
	}

	loadBox.hide();

	// load items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

		item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		rightWidget->addItem(item);
	}

REPAINT:
	// background
	bool usedBackground = CFrameBuffer::getInstance()->getuseBackground();
	if (usedBackground)
	{
		//CFrameBuffer::getInstance()->saveBackgroundImage();

		std::string fname = PLUGINDIR "/test/ard_mediathek.jpg";
		//CFrameBuffer::getInstance()->loadBackgroundPic(fname);

		CFrameBuffer::getInstance()->setBackgroundColor(COL_RED);
		CFrameBuffer::getInstance()->paintBackground();
	}

	// paint all widget
	headers->paint();
	footers->paint();
	frameBoxWidget->paint();
	leftWidget->paint();
	rightWidget->paint();

	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;

	uint32_t sec_timer_id = 0;

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(1*1000*1000, false);

	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec

		if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
		{
			headers->paint();
		} 
		else if (msg == RC_home) 
		{
			loop = false;
		}
		else if(msg == RC_right)
		{
			if(focus == WIDGET_TOP)
				frameBoxWidget->swipRight();
			else if(focus == WIDGET_RIGHT)
				rightWidget->swipRight();
			else if (focus == WIDGET_LEFT)
			{
				focus = WIDGET_RIGHT;

				leftWidget->setOutFocus();
				frameBoxWidget->setOutFocus();

				rightWidget->setSelected(right_selected);
				rightWidget->setOutFocus(false);

				goto REPAINT;
			}
		}
		else if(msg == RC_left)
		{
			if(focus == WIDGET_TOP)
				frameBoxWidget->swipLeft();
			else if(focus == WIDGET_RIGHT)
				rightWidget->swipLeft();
		}
		else if(msg == RC_up)
		{
			if(focus == WIDGET_LEFT)
				leftWidget->scrollLineUp();
			else if(focus == WIDGET_RIGHT)
				rightWidget->scrollLineUp();
		}
		else if(msg == RC_down)
		{
			if(focus == WIDGET_LEFT)
				leftWidget->scrollLineDown();
			else if(focus == WIDGET_RIGHT)
				rightWidget->scrollLineDown();
			if(focus == WIDGET_TOP)
			{
				focus = WIDGET_LEFT;

				frameBoxWidget->setOutFocus();
				rightWidget->setOutFocus();

				leftWidget->setSelected(left_selected);
				leftWidget->setOutFocus(false);

				goto REPAINT;
			}
		}
		else if(msg == RC_yellow)
		{
			if(focus == WIDGET_TOP)
			{
				focus = WIDGET_LEFT;

				frameBoxWidget->setOutFocus();
				rightWidget->setOutFocus();

				leftWidget->setSelected(left_selected);
				leftWidget->setOutFocus(false);
			}
			else if (focus == WIDGET_LEFT)
			{
				focus = WIDGET_RIGHT;

				leftWidget->setOutFocus();
				frameBoxWidget->setOutFocus();

				rightWidget->setSelected(right_selected);
				rightWidget->setOutFocus(false);
			}
			else if (focus == WIDGET_RIGHT)
			{
				focus = WIDGET_TOP;

				leftWidget->setOutFocus();
				rightWidget->setOutFocus();

				frameBoxWidget->setSelected(top_selected);
				frameBoxWidget->setOutFocus(false);
			}

			goto REPAINT;
		}
		else if(msg == RC_red)
		{
			page++;
			right_selected = 0;
			hide();
			rightWidget->clearItems();
			goto DOFILM; // include REPAINT
		}
		else if(msg == RC_green)
		{
			page--;

			if(page <= 1)
				page = 1;

			right_selected = 0;

			hide();
			rightWidget->clearItems();
			goto DOFILM; // include REPAINT
		}
		else if(msg == RC_blue)
		{
			right_selected = 0;
			left_selected = 3;
			plist = "upcoming";
			page = 1;
			hide();
			rightWidget->clearItems();
			goto DOFILM;
		}
		else if(msg == RC_ok)
		{
			if(focus == WIDGET_RIGHT)
			{
				hide();

				right_selected = rightWidget->getSelected();

				///
				ytparser.Cleanup();

				// setregion
				ytparser.SetRegion("DE");

				// set max result
				ytparser.SetMaxResults(1);
			
				// parse feed
				if (ytparser.ParseFeed(cYTFeedParser::SEARCH_BY_ID, m_vMovieInfo[right_selected].vname, m_vMovieInfo[right_selected].vkey))
				{
					yt_video_list_t &ylist = ytparser.GetVideoList();
	
					for (unsigned int j = 0; j < ylist.size(); j++) 
					{
						m_vMovieInfo[right_selected].ytid = ylist[j].id;
						m_vMovieInfo[right_selected].file.Name = ylist[j].GetUrl();
					}
				} 
				///

				if (&m_vMovieInfo[right_selected].file != NULL) 
				{
					//tmpMoviePlayerGui.addToPlaylist(m_vMovieInfo[right_selected]);
					//tmpMoviePlayerGui.exec(NULL, "");

					CMovieInfoWidget movieInfoWidget;
					movieInfoWidget.setMovie(m_vMovieInfo[right_selected]);
		
					movieInfoWidget.exec(NULL, "");
				}

				goto REPAINT;
			}
			else if(focus == WIDGET_LEFT)
			{
				left_selected = leftWidget->getSelected();

				if(top_selected == 0) // movies
				{
					if(left_selected == 0)
					{
						right_selected = 0;
						plist = "now_playing";
						page = 1;
						hide();
						rightWidget->clearItems();
						goto DOFILM; // include REPAINT
					}
					else if(left_selected == 1)
					{
						right_selected = 0;
						plist = "popular";
						page = 1;
						hide();
						rightWidget->clearItems();
						goto DOFILM; // include REPAINT
					}
					else if(left_selected == 2)
					{
						right_selected = 0;
						plist = "top_rated";
						page = 1;
						hide();
						rightWidget->clearItems();
						goto DOFILM; // include REPAINT
					}
					else if(left_selected == 3)
					{
						right_selected = 0;
						plist = "upcoming";
						page = 1;
						hide();
						rightWidget->clearItems();
						goto DOFILM; // include REPAINT
					}
					else if(left_selected == 8)
						loop = false;
				}
				else if(top_selected == 1) // tv
				{
					if(left_selected == 0)
					{
						right_selected = 0;
						plist = "airing_today";
						page = 1;
						hide();
						rightWidget->clearItems();
						goto DOFILM; // include REPAINT
					}
					else if(left_selected == 1)
					{
						right_selected = 0;
						plist = "on_the_air";
						page = 1;
						hide();
						rightWidget->clearItems();
						goto DOFILM; // include REPAINT
					}
					else if(left_selected == 2)
					{
						right_selected = 0;
						plist = "popular";
						page = 1;
						hide();
						rightWidget->clearItems();
						goto DOFILM; // include REPAINT
					}
					else if(left_selected == 3)
					{
						right_selected = 0;
						plist = "top_rated";
						page = 1;
						hide();
						rightWidget->clearItems();
						goto DOFILM; // include REPAINT
					}
					else if(left_selected == 8)
						loop = false;
				}
			}
			else if(focus == WIDGET_TOP)
			{
				top_selected = frameBoxWidget->getSelected();

				if(top_selected == 1)
				{
					right_selected = 0;
					left_selected = 0;

					TVShows = "tv";
					plist = "airing_today";
					page = 1;
					hide();
					rightWidget->clearItems();
					//
					leftWidget->clearItems();
					ClistBoxItem *item1 = new ClistBoxItem("Heute auf Sendung");
					ClistBoxItem *item2 = new ClistBoxItem("Auf Sendung");
					ClistBoxItem *item3 = new ClistBoxItem("Am");
					item3->setOption("populärsten");
					item3->set2lines();
					ClistBoxItem *item4 = new ClistBoxItem("am");
					item4->setOption("besten bewertet");
					item4->set2lines();
					ClistBoxItem *item5 = new ClistBoxItem(NULL, false);
					ClistBoxItem *item6 = new ClistBoxItem(NULL, false);
					ClistBoxItem *item7 = new ClistBoxItem(NULL, false);
					ClistBoxItem *item8 = new ClistBoxItem(NULL, false);
					ClistBoxItem *item9 = new ClistBoxItem("Beenden");

					leftWidget->addItem(item1);
					leftWidget->addItem(item2);
					leftWidget->addItem(item3);
					leftWidget->addItem(item4);
					leftWidget->addItem(item5);
					leftWidget->addItem(item6);
					leftWidget->addItem(item7);
					leftWidget->addItem(item8);
					leftWidget->addItem(item9);
					//
					goto DOFILM; // include REPAINT
				}
				else if(top_selected == 0)
				{
					right_selected = 0;
					left_selected = 0;

					TVShows = "movie";
					plist = "popular";
					page = 1;
					hide();
					rightWidget->clearItems();
					leftWidget->clearItems();

					ClistBoxItem *item1 = new ClistBoxItem("In den Kinos");
					ClistBoxItem *item2 = new ClistBoxItem("Am");
					item2->setOption("populärsten");
					item2->set2lines();
					ClistBoxItem *item3 = new ClistBoxItem("Am besten");
					item3->setOption("bewertet");
					item3->set2lines();
					ClistBoxItem *item4 = new ClistBoxItem("Neue Filme");
					ClistBoxItem *item5 = new ClistBoxItem(NULL, false);
					ClistBoxItem *item6 = new ClistBoxItem(NULL, false);
					ClistBoxItem *item7 = new ClistBoxItem(NULL, false);
					ClistBoxItem *item8 = new ClistBoxItem(NULL, false);
					ClistBoxItem *item9 = new ClistBoxItem("Beenden");

					leftWidget->addItem(item1);
					leftWidget->addItem(item2);
					leftWidget->addItem(item3);
					leftWidget->addItem(item4);
					leftWidget->addItem(item5);
					leftWidget->addItem(item6);
					leftWidget->addItem(item7);
					leftWidget->addItem(item8);
					leftWidget->addItem(item9);
					
					goto DOFILM; // include REPAINT
				}
				else if(top_selected == 2)
				{
					frameBoxWidget->oKKeyPressed(this);
					goto DOFILM;
				}
			}
		}
		else if(msg == RC_info)
		{
			if(focus == WIDGET_RIGHT)
			{
				hide();
				right_selected = rightWidget->getSelected();
				m_movieInfo.showMovieInfo(m_vMovieInfo[right_selected]);
				goto REPAINT;
			}
		}
		else if(msg == RC_page_down)
		{
			if(focus == WIDGET_RIGHT)
			{
				rightWidget->scrollPageDown();
			}
		}
		else if(msg == RC_page_up)
		{
			if(focus == WIDGET_RIGHT)
			{
				rightWidget->scrollPageUp();
			}
		}

		CFrameBuffer::getInstance()->blit();
	}

	//restore previous background
	if (usedBackground)
	{
		//CFrameBuffer::getInstance()->restoreBackgroundImage();
		//CFrameBuffer::getInstance()->useBackground(usedBackground);
		CFrameBuffer::getInstance()->setBackgroundColor(COL_BACKGROUND);
	}

	hide();

	delete frameBoxWidget;
	frameBoxWidget = NULL;

	delete leftWidget;
	leftWidget = NULL;

	delete rightWidget;
	rightWidget = NULL;

	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;

	fileHelper.removeDir(thumbnail_dir.c_str());
	m_vMovieInfo.clear();
	
	delete tmdb;
	tmdb = NULL;

	if(headers)
	{
		delete headers;
		headers = NULL;
	}

	if(footers)
	{
		delete footers;
		footers = NULL;
	}
}

// CBox
void CTestMenu::testCBox()
{
	dprintf(DEBUG_NORMAL, "\ntestCBox\n");

	CBox Box;
	
	Box.iX = g_settings.screen_StartX + 10;
	Box.iY = g_settings.screen_StartY + 10;
	Box.iWidth = (g_settings.screen_EndX - g_settings.screen_StartX - 20)/2;
	Box.iHeight = 40; //(g_settings.screen_EndY - g_settings.screen_StartY - 20);
	
	Box.iRadius = RADIUS_MID;
	Box.iCorner = CORNER_ALL;
	Box.iColor = COL_MENUHEAD_PLUS_0;
	Box.iGradient = DARK2LIGHT2DARK;

	Box.paint();

	CFrameBuffer::getInstance()->blit();

	// loop
	testWidget = new CWidget();

	testWidget->exec(NULL, "");

	hide();

	delete testWidget;
	testWidget = NULL;
}

// CIcon
void CTestMenu::testCIcon()
{
	dprintf(DEBUG_NORMAL, "\ntestCIcon\n");

	//CIcon testIcon(NEUTRINO_ICON_BUTTON_RED);
	CIcon testIcon;

	// paint testIcon
	testIcon.setIcon(NEUTRINO_ICON_BUTTON_RED);
	
	dprintf(DEBUG_NORMAL, "\ntestCIcon: icon:%s iw:%d ih:%d\n", testIcon.iconName.c_str(), testIcon.iWidth, testIcon.iHeight);

	testIcon.paint(150 + BORDER_LEFT, 150);

	CFrameBuffer::getInstance()->blit();

	// loop
	testWidget = new CWidget();

	testWidget->exec(NULL, "");

	hide();

	delete testWidget;
	testWidget = NULL;
}

// CImage
void CTestMenu::testCImage()
{
	dprintf(DEBUG_NORMAL, "\ntestCImage\n");

	//CImage testImage(PLUGINDIR "/netzkino/netzkino.png");
	CImage testImage;

	// paint testImage
	testImage.setImage(PLUGINDIR "/netzkino/netzkino.png");
	
	dprintf(DEBUG_NORMAL, "\ntestCImahe: image:%s iw:%d ih:%d nbp:%d\n", testImage.imageName.c_str(), testImage.iWidth, testImage.iHeight, testImage.iNbp);
	
	testImage.paint(150 + BORDER_LEFT, 150, testImage.iWidth, testImage.iHeight);

	CFrameBuffer::getInstance()->blit();

	testWidget = new CWidget();

	testWidget->exec(NULL, "");

	hide();

	delete testWidget;
	testWidget = NULL;
}

// Cwindow
void CTestMenu::testCWindow()
{
	dprintf(DEBUG_NORMAL, "\ntestCWindow\n");

	CBox Box;
	
	Box.iX = g_settings.screen_StartX + 10;
	Box.iY = g_settings.screen_StartY + 10;
	Box.iWidth = (g_settings.screen_EndX - g_settings.screen_StartX - 20);
	Box.iHeight = (g_settings.screen_EndY - g_settings.screen_StartY - 20);

	//
	CWindow* window = new CWindow();

	window->setPosition(Box.iX, Box.iY, Box.iWidth, Box.iHeight);

	window->setColor(COL_MENUCONTENT_PLUS_0);
	window->setCorner(RADIUS_MID, CORNER_ALL);
	//window->setGradient(DARK2LIGHT2DARK);

	window->paint();
	CFrameBuffer::getInstance()->blit();

	// loop
	testWidget = new CWidget();

	testWidget->exec(NULL, "");

	window->hide();
	delete window;
	window = NULL;

	delete testWidget;
	testWidget = NULL;
}

// CWindow
void CTestMenu::testCWindowShadow()
{
	dprintf(DEBUG_NORMAL, "\ntestCWindowShadow\n");

	CBox Box;
	
	Box.iX = g_settings.screen_StartX + 10;
	Box.iY = g_settings.screen_StartY + 10;
	Box.iWidth = (g_settings.screen_EndX - g_settings.screen_StartX - 20);
	Box.iHeight = (g_settings.screen_EndY - g_settings.screen_StartY - 20);

	//
	CWindow* window = new CWindow(&Box);

	window->setColor(COL_MENUCONTENT_PLUS_0);
	window->setCorner(RADIUS_MID, CORNER_ALL);
	window->enableShadow();
	window->enableSaveScreen();

	window->paint();
	CFrameBuffer::getInstance()->blit();

	testWidget = new CWidget();

	testWidget->exec(NULL, "");

	window->hide();
	delete window;
	window = NULL;

	delete testWidget;
	testWidget = NULL;
}

// custom Color
void CTestMenu::testCWindowCustomColor()
{
	dprintf(DEBUG_NORMAL, "\ntestCWindowCustomColor\n");

	CBox Box;
	
	Box.iX = g_settings.screen_StartX + 10;
	Box.iY = g_settings.screen_StartY + 10;
	Box.iWidth = (g_settings.screen_EndX - g_settings.screen_StartX - 20);
	Box.iHeight = (g_settings.screen_EndY - g_settings.screen_StartY - 20);

	//
	CWindow* window = new CWindow(&Box);

	window->setColor(COL_DARK_GREEN);
	window->setCorner(RADIUS_MID, CORNER_ALL);
	window->enableShadow();
	window->enableSaveScreen();

	window->paint();
	CFrameBuffer::getInstance()->blit();

	testWidget = new CWidget();

	testWidget->exec(NULL, "");

	window->hide();
	delete window;
	window = NULL;

	delete testWidget;
	testWidget = NULL;
}

void CTestMenu::testCHeaders()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testCHeaders\n");
	
	CBox headBox;
	headBox.iX = g_settings.screen_StartX + 10;
	headBox.iY = g_settings.screen_StartY + 10;
	headBox.iWidth = (g_settings.screen_EndX - g_settings.screen_StartX - 20);
	headBox.iHeight = 40;

	CBox footBox;
	footBox.iHeight = 40;
	footBox.iX = g_settings.screen_StartX + 10;
	footBox.iY = g_settings.screen_EndY - 10 - footBox.iHeight;
	footBox.iWidth = (g_settings.screen_EndX - g_settings.screen_StartX - 20);

	headers = new CHeaders(headBox, "test CHeaders", NEUTRINO_ICON_MP3);

	headers->enablePaintDate();
	headers->setButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	//headers->setColor(COL_BLUE);
	//headers->setCorner();
	//headers->setGradient(NOGRADIENT);

	footers = new CFooters(footBox);

	footers->setButtons(FootButtons, FOOT_BUTTONS_COUNT);
	//headers->setColor(COL_BLUE);
	//headers->setCorner();
	//headers->setGradient(NOGRADIENT);

	//headers->paintHead(headBox, "test CHeaders", NEUTRINO_ICON_MP3);
	//headers->paintFoot(footBox, FOOT_BUTTONS_COUNT, FootButtons);	
	headers->paint();
	footers->paint();
	CFrameBuffer::getInstance()->blit();

	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;

	uint32_t sec_timer_id = 0;

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(1*1000*1000, false);

	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec

		if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
		{
			//headers->paintHead(headBox, "test CHeaders", NEUTRINO_ICON_MP3);
			headers->paint();
		} 
		else if (msg == RC_home) 
		{
			loop = false;
		}

		CFrameBuffer::getInstance()->blit();
	}
	
	hide();

	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;

	if(headers)
	{
		delete headers;
		headers = NULL;
	}

	if(footers)
	{
		delete footers;
		footers = NULL;
	}
}

// CStringInput
void CTestMenu::testCStringInput()
{
	dprintf(DEBUG_NORMAL, "\ntestCStringInput\n");

	std::string value;
	CStringInput * stringInput = new CStringInput("CStringInput", value.c_str());
	
	stringInput->exec(NULL, "");
	stringInput->hide();
	delete stringInput;
	stringInput = NULL;
	value.clear();
}

// CStringInputSMS
void CTestMenu::testCStringInputSMS()
{
	dprintf(DEBUG_NORMAL, "\ntestCStringInputSMS\n");

	std::string value;
	CStringInputSMS * stringInputSMS = new CStringInputSMS("CStringInputSMS", value.c_str());
	
	stringInputSMS->exec(NULL, "");
	stringInputSMS->hide();
	delete stringInputSMS;
	value.clear();
}

// CPINInput
void CTestMenu::testCPINInput()
{
	dprintf(DEBUG_NORMAL, "\ntestCPINInput\n");

	std::string value;
	CPINInput * pinInput = new CPINInput("CPINInput", value.c_str());
	
	pinInput->exec(NULL, "");
	pinInput->hide();
	delete pinInput;
	pinInput = NULL;
	value.clear();
}

// CPLPINInput
void CTestMenu::testCPLPINInput()
{
	dprintf(DEBUG_NORMAL, "\ntestCPLPINInput\n");

	std::string value;
	CPLPINInput * pinInput = new CPLPINInput("CPLPINInput", value.c_str());
	
	pinInput->exec(NULL, "");
	pinInput->hide();
	delete pinInput;
	pinInput = NULL;
	value.clear();
}

// CPINChangeWidget
void CTestMenu::testCPINChangeWidget()
{
	dprintf(DEBUG_NORMAL, "\ntestCPINChangeWidget\n");

	std::string value;
	CPINChangeWidget * pinInput = new CPINChangeWidget("CPINChangeWidget", value.c_str());
	
	pinInput->exec(NULL, "");
	pinInput->hide();
	delete pinInput;
	pinInput = NULL;
	value.clear();
}

// CIPInput
void CTestMenu::testCIPInput()
{
	dprintf(DEBUG_NORMAL, "\ntestCIPInput\n");

	std::string value;
	CIPInput * ipInput = new CIPInput(LOCALE_STREAMINGMENU_SERVER_IP, value);
	
	ipInput->exec(NULL, "");
	ipInput->hide();
	delete ipInput;
	value.clear();
}

// CMACInput
void CTestMenu::testCMACInput()
{
	dprintf(DEBUG_NORMAL, "\ntestCMACInput\n");

	std::string value;
	CMACInput * macInput = new CMACInput(LOCALE_RECORDINGMENU_SERVER_MAC, (char *)value.c_str());
	
	macInput->exec(NULL, "");
	macInput->hide();
	delete macInput;
	macInput = NULL;
	value.clear();
}

// CDateInput
void CTestMenu::testCDateInput()
{
	dprintf(DEBUG_NORMAL, "\ntestCDateInput\n");

	time_t value;
	CDateInput * dateInput = new CDateInput(LOCALE_FILEBROWSER_SORT_DATE, &value);
	
	dateInput->exec(NULL, "");
	dateInput->hide();
	delete dateInput;
}

// CTimeInput
void CTestMenu::testCTimeInput()
{
	dprintf(DEBUG_NORMAL, "\ntestCTimeInput\n");

	std::string value;
	CTimeInput * timeInput = new CTimeInput(LOCALE_FILEBROWSER_SORT_DATE, (char *)value.c_str());
	
	timeInput->exec(NULL, "");
	timeInput->hide();
	delete timeInput;
	timeInput = NULL;
	value.clear();
}

// CIntInput
void CTestMenu::testCIntInput()
{
	dprintf(DEBUG_NORMAL, "\ntestCIntInput\n");

	int value;
	CIntInput * intInput = new CIntInput(LOCALE_FILEBROWSER_SORT_DATE, value);
	
	intInput->exec(NULL, "");
	intInput->hide();
	delete intInput;
	intInput = NULL;	
}

// CInfoBox
void CTestMenu::testCInfoBox()
{
	dprintf(DEBUG_NORMAL, "\ntestCInfoBox\n");

	std::string buffer;
	
	// prepare print buffer  
	buffer = "CInfoBox";
	buffer += "\n";
	buffer += "testing CInfoBox";
	buffer += "\n";

	// thumbnail
	std::string tname = PLUGINDIR "/netzkino/netzkino.png";
	
	// scale pic
	int p_w = 0;
	int p_h = 0;

	CFrameBuffer::getInstance()->scaleImage(tname, &p_w, &p_h);
		
	
	CBox position(g_settings.screen_StartX + 50, g_settings.screen_StartY + 50, g_settings.screen_EndX - g_settings.screen_StartX - 100, g_settings.screen_EndY - g_settings.screen_StartY - 100); 
	
	CInfoBox * infoBox = new CInfoBox(g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1], SCROLL, &position, "CInfoBox", g_Font[SNeutrinoSettings::FONT_TYPE_EPG_TITLE], NEUTRINO_ICON_INFO);

	infoBox->setText(buffer.c_str(), tname.c_str(), p_w, p_h);
	infoBox->exec();
	delete infoBox;
	infoBox = NULL;
}

// CMessageBox
void CTestMenu::testCMessageBox()
{
	dprintf(DEBUG_NORMAL, "\ntestCMessageBox\n");

	CMessageBox * messageBox = new CMessageBox(LOCALE_MESSAGEBOX_INFO, "testing CMessageBox"/*, 600, NEUTRINO_ICON_INFO, mbrYes, mbNone*/);
	
	int res = messageBox->exec();

	printf("res:%d messageBox->result:%d\n", res, messageBox->result);

	delete messageBox;
	messageBox = NULL;
}

// MessageBox
void CTestMenu::testCMessageBoxInfoMsg()
{
	dprintf(DEBUG_NORMAL, "\ntestCMessageBox\n");

	MessageBox(LOCALE_MESSAGEBOX_INFO, "testing CMessageBoxInfoMsg", mbrBack, mbBack, NEUTRINO_ICON_INFO);
}

// MessageBox
void CTestMenu::testCMessageBoxErrorMsg()
{
	dprintf(DEBUG_NORMAL, "\ntestCMessageBox\n");

	MessageBox(LOCALE_MESSAGEBOX_ERROR, "testing CMessageBoxErrorMsg", mbrCancel, mbCancel, NEUTRINO_ICON_ERROR);
}

// CHintBox
void CTestMenu::testCHintBox()
{
	dprintf(DEBUG_NORMAL, "\ntestCHintBox\n");

	CHintBox * hintBox = new CHintBox(LOCALE_MESSAGEBOX_INFO, "testing CHintBox");
	
	hintBox->exec(10);

	delete hintBox;
	hintBox = NULL;
}

// HintBox
void CTestMenu::testCHintBoxInfo()
{
	dprintf(DEBUG_NORMAL, "\ntestCHintBox\n");

	HintBox(LOCALE_MESSAGEBOX_INFO, "testing CHintBoxInfo", HINTBOX_WIDTH, 10, NEUTRINO_ICON_INFO);
}

// CHelpBox
void CTestMenu::testCHelpBox()
{
	dprintf(DEBUG_NORMAL, "\ntestCHelpBox\n");

	CHelpBox * helpBox = new CHelpBox();
	
	// text
	helpBox->addLine("helpBox");

	// icon|text
	helpBox->addLine(NEUTRINO_ICON_BUTTON_RED, "Huhu :-P", g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1], COL_RED0, true);

	//
	helpBox->addLine(NEUTRINO_ICON_BUTTON_GREEN, "Huhu :-)", g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1], COL_BLUE0, true);

	helpBox->addSeparator();

	//
	helpBox->addLine("neutrinoHD2 the best GUI :-P", g_Font[SNeutrinoSettings::FONT_TYPE_MENU], COL_MENUCONTENTINACTIVE, true);
	

	//
	helpBox->addSeparator();
	
	helpBox->add2Line("Gui: ", "neutrinoHD2 the best GUI :-P", g_Font[SNeutrinoSettings::FONT_TYPE_MENU], COL_MENUCONTENTINACTIVE, true, g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1], COL_BLUE0, true);

	// icon
	helpBox->addLine(NEUTRINO_ICON_BUTTON_YELLOW, "Huhu :-P", g_Font[SNeutrinoSettings::FONT_TYPE_MENU], COL_GREEN0, true);

	helpBox->addSeparator();

	//
	helpBox->addLine("Huhu :-)");

	//
	helpBox->addLine(NEUTRINO_ICON_BUTTON_BLUE, "neutrinoHD2 the best GUI :-P", g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1], COL_YELLOW0, true);

	helpBox->addSeparator();
		
	helpBox->addLine(NEUTRINO_ICON_MENUITEM_HDDSETTINGS, "neutrinoHD2 the best GUI :-P", g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1], COL_YELLOW0, true);
		
	helpBox->addLine(NEUTRINO_ICON_MENUITEM_SCANSETTINGS, "neutrinoHD2 the best GUI :-P", g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1], COL_YELLOW0, true);

	helpBox->show(LOCALE_MESSAGEBOX_INFO, HELPBOX_WIDTH, -1, mbrBack, mbNone);
	
	delete helpBox;
	helpBox = NULL;
}

// CTextBox
void CTestMenu::testCTextBox()
{
	dprintf(DEBUG_NORMAL, "\ntestCTextBox\n");

	CBox Box;
	
	Box.iX = g_settings.screen_StartX + 10;
	Box.iY = g_settings.screen_StartY + 10;
	Box.iWidth = g_settings.screen_EndX - g_settings.screen_StartX - 20;
	Box.iHeight = (g_settings.screen_EndY - g_settings.screen_StartY - 20);
	
	CTextBox * textBox = new CTextBox();

	textBox->setPosition(&Box);
	
	const char * buffer = NULL;
	
	// prepare print buffer  
	buffer = "CTextBox\ntesting CTextBox\n";
		
	std::string tname = PLUGINDIR "/netzkino/netzkino.png";
	
	// scale pic
	int p_w = 0;
	int p_h = 0;
	
	CFrameBuffer::getInstance()->scaleImage(tname, &p_w, &p_h);
	
	textBox->setText(buffer, tname.c_str(), p_w, p_h);
	
	textBox->paint();
	CFrameBuffer::getInstance()->blit();
	
	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec
		
		if (msg == RC_home) 
		{
			loop = false;
		}
		else if(msg == RC_info)
		{
			textBox->setBigFonts();
		}

		CFrameBuffer::getInstance()->blit();
	}

	textBox->hide();
	
	delete textBox;
	textBox = NULL;
}

// ClistFrame
void CTestMenu::testCListFrame()
{
	dprintf(DEBUG_NORMAL, "\ntestClistFrame\n");

	CBox listFrameBox;
	LF_LINES listFrameLines;
	int selected = 0;
	
	listFrameBox.iX = g_settings.screen_StartX + 10;
	listFrameBox.iY = g_settings.screen_StartY + 10;
	listFrameBox.iWidth = w_max ( (CFrameBuffer::getInstance()->getScreenWidth() / 20 * 17), (CFrameBuffer::getInstance()->getScreenWidth() / 20 ));
	listFrameBox.iHeight = h_max ( (CFrameBuffer::getInstance()->getScreenHeight() / 20 * 16), (CFrameBuffer::getInstance()->getScreenHeight() / 20));

	// recalculate x and y
	listFrameBox.iX = CFrameBuffer::getInstance()->getScreenX() + ((CFrameBuffer::getInstance()->getScreenWidth() - (listFrameBox.iWidth)) / 2);
	listFrameBox.iY = CFrameBuffer::getInstance()->getScreenY() + ((CFrameBuffer::getInstance()->getScreenHeight() - listFrameBox.iHeight) / 2);

	//
	#define MAX_ROWS 		LF_MAX_ROWS //6

	

	// init
	listFrameLines.rows = MAX_ROWS;

	for(int row = 0; row < MAX_ROWS; row++)
	{
		listFrameLines.lineArray[row].clear();
	}

	// rowwidth
	listFrameLines.rowWidth[0] = MAX_WINDOW_WIDTH / 20;
	listFrameLines.rowWidth[1] = MAX_WINDOW_WIDTH / 4;
	listFrameLines.rowWidth[2] = MAX_WINDOW_WIDTH / 12;
	listFrameLines.rowWidth[3] = MAX_WINDOW_WIDTH / 8;
	listFrameLines.rowWidth[4] = MAX_WINDOW_WIDTH / 5;
	listFrameLines.rowWidth[5] = MAX_WINDOW_WIDTH / 10;

	// headertitle
	listFrameLines.lineHeader[0] = "Nr";
	listFrameLines.lineHeader[1] = "title";
	listFrameLines.lineHeader[2] = "duration";
	listFrameLines.lineHeader[3] = "genre";
	listFrameLines.lineHeader[4] = "artist";
	listFrameLines.lineHeader[5] = "date";
	

	CListFrame * listFrame = new CListFrame(&listFrameLines, NULL, CListFrame::TITLE | CListFrame::HEADER_LINE | CListFrame::SCROLL, &listFrameBox);

	// title
	listFrame->setTitle("listFrame (AudioPlayer)", NEUTRINO_ICON_MP3);
	
	// fill lineArrays list
	CHintBox loadBox("listFrame", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	
	loadAudioPlaylist();
	
	loadBox.hide();

	//
	int count = 0;
	for (unsigned int i = 0; i < AudioPlaylist.size(); i++)
	{
		std::string title;
		std::string artist;
		std::string genre;
		std::string date;
		char duration[9] = "";

		title = AudioPlaylist[i].MetaData.title;
		artist = AudioPlaylist[i].MetaData.artist;
		genre = AudioPlaylist[i].MetaData.genre;	
		date = AudioPlaylist[i].MetaData.date;

		snprintf(duration, 8, "(%ld:%02ld)", AudioPlaylist[i].MetaData.total_time / 60, AudioPlaylist[i].MetaData.total_time % 60);

		listFrameLines.lineArray[0].push_back(to_string(i + 1));
		listFrameLines.lineArray[1].push_back(title);
		listFrameLines.lineArray[2].push_back(duration);
		listFrameLines.lineArray[3].push_back(genre);
		listFrameLines.lineArray[4].push_back(artist);
		listFrameLines.lineArray[5].push_back(date);
	}
	//
	listFrame->setLines(&listFrameLines);
	
	// paint
	listFrame->paint();
	listFrame->showSelection(true);

	CFrameBuffer::getInstance()->blit();

	CAudioPlayer::getInstance()->init();
	
	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;

	bool loop = true;

REPEAT:
	listFrame->refresh();
	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec
		
		if (msg == RC_home) 
		{
			//
			if (CAudioPlayer::getInstance()->getState() != CBaseDec::STOP)
			{
				CAudioPlayer::getInstance()->stop();
			}

			//
			for(int i = 0; i < MAX_ROWS; i++)
			{
				listFrameLines.lineArray[i].clear();
			}

			listFrame->hide();

			loop = false;
		}
		else if(msg == RC_down)
		{
			listFrame->scrollLineDown(1);
			listFrame->refresh();
		}
		else if(msg == RC_up)
		{
			listFrame->scrollLineUp(1);
			listFrame->refresh();
		}
		else if(msg == RC_page_down)
		{
			listFrame->scrollPageDown(1);
			listFrame->refresh();
		}
		else if(msg == RC_page_up)
		{
			listFrame->scrollPageUp(1);
			listFrame->refresh();
		}
		else if(msg == RC_ok)
		{
			if(AudioPlaylist.size() > 0)
			{
				selected = listFrame->getSelectedLine();

				for (unsigned int i = 0; i < AudioPlaylist.size(); i++)
				{
					tmpAudioPlayerGui.addToPlaylist(AudioPlaylist[i]);
				}

				tmpAudioPlayerGui.setCurrent(selected);
				tmpAudioPlayerGui.exec(NULL, "");

				//listFrame->refresh();
				goto REPEAT;
			}
		}

		//listFrame->refresh();
		CFrameBuffer::getInstance()->blit();
	}
	
	delete listFrame;
	listFrame = NULL;
}

// CProgressBar
void CTestMenu::testCProgressBar()
{
	dprintf(DEBUG_NORMAL, "\ntestCProgressBar\n");

	CProgressBar *timescale = NULL;
	
	CBox Box;
	
	Box.iX = g_settings.screen_StartX + 10;
	Box.iY = g_settings.screen_StartY + 10;
	Box.iWidth = (g_settings.screen_EndX - g_settings.screen_StartX - 20);
	Box.iHeight = (g_settings.screen_EndY - g_settings.screen_StartY - 20)/40;
	
	timescale = new CProgressBar(Box.iWidth, Box.iHeight);
	timescale->reset();
	
	timescale->paint(Box.iX, Box.iY, 10);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	timescale->paint(Box.iX, Box.iY, 20);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	timescale->paint(Box.iX, Box.iY, 30);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	timescale->paint(Box.iX, Box.iY, 40);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	timescale->paint(Box.iX, Box.iY, 50);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	timescale->paint(Box.iX, Box.iY, 60);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	timescale->paint(Box.iX, Box.iY, 70);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	timescale->paint(Box.iX, Box.iY, 80);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	timescale->paint(Box.iX, Box.iY, 90);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	timescale->paint(Box.iX, Box.iY, 100);
	CFrameBuffer::getInstance()->blit();
	
	delete timescale;
	timescale = NULL;
	//
	hide();
}

// CProgressWindow
void CTestMenu::testCProgressWindow()
{
	dprintf(DEBUG_NORMAL, "\ntestCProgressWindow\n");

	CProgressWindow * progress = new CProgressWindow();
	progress->setTitle("CProgressWindow");
	progress->paint();
	
	progress->showStatusMessageUTF("testing CProgressWindow:0");
	progress->showGlobalStatus(0);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showGlobalStatus(10);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showGlobalStatus(20);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showStatusMessageUTF("testing CProgressWindow:30");
	progress->showGlobalStatus(30);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showGlobalStatus(40);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showGlobalStatus(50);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showStatusMessageUTF("testing CProgressWindow:60");
	progress->showGlobalStatus(60);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showGlobalStatus(70);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showStatusMessageUTF("testing CProgressWindow:80");
	progress->showGlobalStatus(80);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showGlobalStatus(90);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	progress->showStatusMessageUTF("testing CProgressWindow:100");
	progress->showGlobalStatus(100);
	CFrameBuffer::getInstance()->blit();
	usleep(1000000);
	
	progress->hide();
	delete progress;
	progress = NULL;
        
}

// CButtons
void CTestMenu::testCButtons()
{
	dprintf(DEBUG_NORMAL, "\ntestCButtons\n");

	int icon_w, icon_h;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_RED, &icon_w, &icon_h);
	buttons.paintFootButtons(g_settings.screen_StartX + 50 + BORDER_LEFT, g_settings.screen_StartY + 50, (g_settings.screen_EndX - g_settings.screen_StartX - 100), icon_h, FOOT_BUTTONS_COUNT, FootButtons);

	CFrameBuffer::getInstance()->blit();

	// loop
	g_RCInput->messageLoop();
	hide();
}

// ClistBox(standard)
void CTestMenu::testClistBox()
{
	dprintf(DEBUG_NORMAL, "\ntestClistBox(standard)\n");

	CBox Box;
	
	Box.iWidth = g_settings.screen_EndX - g_settings.screen_StartX - 20;
	Box.iHeight = g_settings.screen_EndY - g_settings.screen_StartY - 20;

	Box.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - Box.iWidth ) >> 1 );
	Box.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - Box.iHeight) >> 1 );

	rightWidget = new ClistBox(&Box);
	
	CHintBox loadBox("ClistBox", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	loadMoviePlaylist();
	loadBox.hide();

	// load items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "mmwplay");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());
		//item->setOptionInfo("OptionInfo");

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());
		//item->setOptionInfo1("OptionInfo1");

		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());
		//item->setOptionInfo2("OptionInfo2");

		item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->set2lines();

		std::string tmp = m_vMovieInfo[i].epgInfo1;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo2;

		item->setHint(tmp.c_str());
		
		rightWidget->addItem(item);
	}

	// mode
	rightWidget->setWidgetType(WIDGET_TYPE_STANDARD);
	rightWidget->setWidgetMode(MODE_LISTBOX);
	rightWidget->enableCenterPos();
	rightWidget->enableShrinkMenu();

	// head
	rightWidget->setTitle("ClistBox(standard)", NEUTRINO_ICON_MOVIE);
	rightWidget->enablePaintHead();
	rightWidget->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	rightWidget->enablePaintDate();

	// footer
	rightWidget->enablePaintFoot();
	rightWidget->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);

	// footinfo
	rightWidget->enablePaintFootInfo(80);
	rightWidget->setFootInfoMode(FOOT_HINT_MODE);

	rightWidget->setSelected(selected);
	rightWidget->paint();
	
	CFrameBuffer::getInstance()->blit();

	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
	uint32_t sec_timer_id = 0;

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(1*1000*1000, false);

	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec

		if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
		{
			rightWidget->paintHead();
		} 
		else if (msg == RC_up)
		{
			rightWidget->onUpKeyPressed();
		}
		else if (msg == RC_down)
		{
			rightWidget->onDownKeyPressed();
		}
		else if (msg == RC_right)
		{
			rightWidget->onRightKeyPressed();
		}
		else if (msg == RC_left)
		{
			rightWidget->onLeftKeyPressed();
		}
		else if (msg == RC_page_up)
		{
			rightWidget->onPageUpKeyPressed();
		}
		else if (msg == RC_page_down)
		{
			rightWidget->onPageDownKeyPressed();
		}
		else if (msg == RC_home) 
		{
			loop = false;
		}
		else if(msg == RC_ok)
		{
			rightWidget->oKKeyPressed(this);
			rightWidget->paint();
		}
		else if(msg == RC_info)
		{
			rightWidget->hide();

			selected = rightWidget->getSelected();
			m_movieInfo.showMovieInfo(m_vMovieInfo[selected]);

			rightWidget->paint();
		}

		CFrameBuffer::getInstance()->blit();
	}

	rightWidget->hide();
	delete rightWidget;
	rightWidget = NULL;

	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;
}

// ClistBox(classic)
void CTestMenu::testClistBox2()
{
	dprintf(DEBUG_NORMAL, "\ntestClistBox(classic)\n");

	CBox Box;
	
	Box.iWidth = g_settings.screen_EndX - g_settings.screen_StartX - 20;
	Box.iHeight = g_settings.screen_EndY - g_settings.screen_StartY - 20;

	Box.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - Box.iWidth ) >> 1 );
	Box.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - Box.iHeight) >> 1 );

	rightWidget = new ClistBox(&Box);

	CHintBox loadBox("ClistBox(classic)", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	loadMoviePlaylist();
	loadBox.hide();

	// load items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "mmwplay");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());
		//item->setOptionInfo("OptionInfo");

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());
		//item->setOptionInfo1("OptionInfo1");

		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());
		//item->setOptionInfo2("OptionInfo2");

		item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->set2lines();

		std::string tmp = m_vMovieInfo[i].epgInfo1;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo2;

		item->setHint(tmp.c_str());

		rightWidget->addItem(item);
	}

	// widgettype
	rightWidget->setWidgetType(WIDGET_TYPE_CLASSIC);
	rightWidget->setWidgetMode(MODE_LISTBOX);
	rightWidget->enableCenterPos();
	rightWidget->enableShrinkMenu();

	// head
	rightWidget->setTitle("ClistBox(classic)", NEUTRINO_ICON_MOVIE);
	rightWidget->enablePaintHead();
	rightWidget->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	rightWidget->enablePaintDate();

	// footer
	rightWidget->enablePaintFoot();
	rightWidget->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);

	// footinfo
	rightWidget->enablePaintFootInfo(80);
	rightWidget->setFootInfoMode(FOOT_HINT_MODE);

	rightWidget->setSelected(selected);
	rightWidget->paint();
	
	CFrameBuffer::getInstance()->blit();

	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
	uint32_t sec_timer_id = 0;

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(1*1000*1000, false);

	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec

		if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
		{
			rightWidget->paintHead();
		}
		else if (msg == RC_up)
		{
			rightWidget->onUpKeyPressed();
		}
		else if (msg == RC_down)
		{
			rightWidget->onDownKeyPressed();
		}
		else if (msg == RC_right)
		{
			rightWidget->onRightKeyPressed();
		}
		else if (msg == RC_left)
		{
			rightWidget->onLeftKeyPressed();
		}
		else if (msg == RC_page_up)
		{
			rightWidget->onPageUpKeyPressed();
		}
		else if (msg == RC_page_down)
		{
			rightWidget->onPageDownKeyPressed();
		} 
		else if (msg == RC_home) 
		{
			loop = false;
		}
		else if(msg == RC_ok)
		{
			rightWidget->oKKeyPressed(this);
			rightWidget->paint();
		}
		else if(msg == RC_info)
		{
			rightWidget->hide();

			selected = rightWidget->getSelected();
			m_movieInfo.showMovieInfo(m_vMovieInfo[selected]);
			
			rightWidget->paint();
		}

		CFrameBuffer::getInstance()->blit();
	}

	rightWidget->hide();
	delete rightWidget;
	rightWidget = NULL;

	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;
}

// ClistBox(extended)
void CTestMenu::testClistBox3()
{
	dprintf(DEBUG_NORMAL, "\ntestClistBox(extended)\n");

	CBox Box;
	
	Box.iWidth = g_settings.screen_EndX - g_settings.screen_StartX - 20;
	Box.iHeight = g_settings.screen_EndY - g_settings.screen_StartY - 20;

	Box.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - Box.iWidth ) >> 1 );
	Box.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - Box.iHeight) >> 1 );

	rightWidget = new ClistBox(&Box);

	CHintBox loadBox("ClistBox(extended)", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	loadMoviePlaylist();
	loadBox.hide();

	// load items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "mmwplay");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());
		//item->setOptionInfo("OptionInfo");

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());
		//item->setOptionInfo1("OptionInfo1");

		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());
		//item->setOptionInfo2("OptionInfo2");

		item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->set2lines();

		std::string tmp = m_vMovieInfo[i].epgTitle;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo1;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo2;

		item->setHint(tmp.c_str());

		rightWidget->addItem(item);
	}

	// widgettype
	rightWidget->setWidgetType(WIDGET_TYPE_EXTENDED);
	rightWidget->setWidgetMode(MODE_LISTBOX);
	rightWidget->enableCenterPos();
	rightWidget->enableShrinkMenu();

	// head
	rightWidget->setTitle("ClistBox(extended)", NEUTRINO_ICON_MOVIE);
	rightWidget->enablePaintHead();
	rightWidget->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	rightWidget->enablePaintDate();

	// footer
	rightWidget->enablePaintFoot();
	rightWidget->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);

	// footinfo
	rightWidget->enablePaintFootInfo(80);

	rightWidget->setSelected(selected);
	rightWidget->paint();
	
	CFrameBuffer::getInstance()->blit();

	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
	uint32_t sec_timer_id = 0;

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(1*1000*1000, false);

	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec

		if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
		{
			rightWidget->paintHead();
		} 
		else if (msg == RC_up)
		{
			rightWidget->onUpKeyPressed();
		}
		else if (msg == RC_down)
		{
			rightWidget->onDownKeyPressed();
		}
		else if (msg == RC_right)
		{
			rightWidget->onRightKeyPressed();
		}
		else if (msg == RC_left)
		{
			rightWidget->onLeftKeyPressed();
		}
		else if (msg == RC_page_up)
		{
			rightWidget->onPageUpKeyPressed();
		}
		else if (msg == RC_page_down)
		{
			rightWidget->onPageDownKeyPressed();
		}
		else if (msg == RC_home) 
		{
			loop = false;
		}
		else if(msg == RC_ok)
		{
			rightWidget->oKKeyPressed(this);
			rightWidget->paint();
		}
		else if(msg == RC_info)
		{
			rightWidget->hide();

			selected = rightWidget->getSelected();
			m_movieInfo.showMovieInfo(m_vMovieInfo[selected]);
			
			rightWidget->paint();
		}

		CFrameBuffer::getInstance()->blit();
	}

	rightWidget->hide();
	delete rightWidget;
	rightWidget = NULL;

	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;
}

// ClistBox(frame)
void CTestMenu::testClistBox4()
{
	dprintf(DEBUG_NORMAL, "\ntestClistBox(frame)\n");

	CBox Box;
	
	Box.iWidth = g_settings.screen_EndX - g_settings.screen_StartX - 20;
	Box.iHeight = g_settings.screen_EndY - g_settings.screen_StartY - 20;

	Box.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - Box.iWidth ) >> 1 );
	Box.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - Box.iHeight) >> 1 );

	rightWidget = new ClistBox(&Box);

	CHintBox loadBox("ClistBox(frame)", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	loadMoviePlaylist();
	loadBox.hide();

	// load items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "mmwplay");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

		item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->set2lines();

		std::string tmp = m_vMovieInfo[i].epgTitle;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo1;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo2;

		item->setHint(tmp.c_str());

		rightWidget->addItem(item);
	}

	// widgettype
	rightWidget->setWidgetType(WIDGET_TYPE_FRAME);
	rightWidget->setItemsPerPage(5,2);
	rightWidget->setWidgetMode(MODE_LISTBOX);
	rightWidget->enableCenterPos();
	rightWidget->enableShrinkMenu();

	rightWidget->disablePaintFrame();	

	rightWidget->setSelected(selected);
	rightWidget->paint();
	
	CFrameBuffer::getInstance()->blit();

	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
	uint32_t sec_timer_id = 0;

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(1*1000*1000, false);

	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec

		if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
		{
			rightWidget->paintHead();
		} 
		else if (msg == RC_up)
		{
			rightWidget->onUpKeyPressed();
		}
		else if (msg == RC_down)
		{
			rightWidget->onDownKeyPressed();
		}
		else if (msg == RC_right)
		{
			rightWidget->onRightKeyPressed();
		}
		else if (msg == RC_left)
		{
			rightWidget->onLeftKeyPressed();
		}
		else if (msg == RC_page_up)
		{
			rightWidget->onPageUpKeyPressed();
		}
		else if (msg == RC_page_down)
		{
			rightWidget->onPageDownKeyPressed();
		}
		else if (msg == RC_home) 
		{
			loop = false;
		}
		else if(msg == RC_ok)
		{
			rightWidget->oKKeyPressed(this);
			rightWidget->paint();
		}
		else if(msg == RC_info)
		{
			rightWidget->hide();

			selected = rightWidget->getSelected();
			m_movieInfo.showMovieInfo(m_vMovieInfo[selected]);
			
			rightWidget->paint();
		}

		CFrameBuffer::getInstance()->blit();
	}

	rightWidget->hide();
	delete leftWidget;
	rightWidget = NULL;

	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;
}

// ClistBox(mode menu)
void CTestMenu::testClistBox5()
{
	dprintf(DEBUG_NORMAL, "\ntestClistBox(menu mode)\n");

	CBox Box;
	
	Box.iWidth = g_settings.screen_EndX - g_settings.screen_StartX - 20;
	Box.iHeight = g_settings.screen_EndY - g_settings.screen_StartY - 20;

	Box.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - Box.iWidth ) >> 1 );
	Box.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - Box.iHeight) >> 1 );

	rightWidget = new ClistBox(&Box);

	CHintBox loadBox("ClistBox(menu mode)", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	loadMoviePlaylist();
	loadBox.hide();

	// load items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "mmwplay");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());
		//item->setOptionInfo("OptionInfo");

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());
		//item->setOptionInfo1("OptionInfo1");

		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());
		//item->setOptionInfo2("OptionInfo2");

		item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->set2lines();

		std::string tmp = m_vMovieInfo[i].epgTitle;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo1;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo2;

		item->setHint(tmp.c_str());
		
		rightWidget->addItem(item);
	}

	// mode
	rightWidget->setWidgetType(WIDGET_TYPE_STANDARD);
	rightWidget->setWidgetMode(MODE_MENU);
	rightWidget->enableCenterPos();
	rightWidget->enableShrinkMenu();

	//
	rightWidget->enableWidgetChange();

	rightWidget->setItemsPerPage(5, 2);

	// head
	rightWidget->setTitle("ClistBox(menu mode)", NEUTRINO_ICON_MOVIE);
	rightWidget->enablePaintHead();
	rightWidget->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	rightWidget->enablePaintDate();

	// footer
	rightWidget->enablePaintFoot();
	rightWidget->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);

	// footinfo
	rightWidget->enablePaintFootInfo(80);

	rightWidget->setSelected(selected);
	rightWidget->paint();

	CFrameBuffer::getInstance()->blit();

	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
	uint32_t sec_timer_id = 0;

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(1*1000*1000, false);

	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec

		if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
		{
			rightWidget->paintHead();
		} 
		else if (msg == RC_up)
		{
			rightWidget->onUpKeyPressed();
		}
		else if (msg == RC_down)
		{
			rightWidget->onDownKeyPressed();
		}
		else if (msg == RC_right)
		{
			rightWidget->onRightKeyPressed();
		}
		else if (msg == RC_left)
		{
			rightWidget->onLeftKeyPressed();
		}
		else if (msg == RC_page_up)
		{
			rightWidget->onPageUpKeyPressed();
		}
		else if (msg == RC_page_down)
		{
			rightWidget->onPageDownKeyPressed();
		}
		else if (msg == RC_home) 
		{
			loop = false;
		}
		else if(msg == RC_ok)
		{
			rightWidget->oKKeyPressed(this);
			rightWidget->paint();
		}
		else if(msg == RC_info)
		{
			rightWidget->hide();

			selected = rightWidget->getSelected();
			m_movieInfo.showMovieInfo(m_vMovieInfo[selected]);
			
			rightWidget->paint();
		}
		else if(msg == RC_setup)
		{
			rightWidget->changeWidgetType();
		}

		CFrameBuffer::getInstance()->blit();
	}

	rightWidget->hide();
	delete rightWidget;
	rightWidget = NULL;

	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;
}

// ClistBox(listBox mode)
void CTestMenu::testClistBox6()
{
	dprintf(DEBUG_NORMAL, "\ntestClistBox(listBox mode)\n");

	CBox Box;
	
	Box.iWidth = g_settings.screen_EndX - g_settings.screen_StartX - 20;
	Box.iHeight = g_settings.screen_EndY - g_settings.screen_StartY - 20;

	Box.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - Box.iWidth ) >> 1 );
	Box.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - Box.iHeight) >> 1 );

	rightWidget = new ClistBox(&Box);

	CHintBox loadBox("ClistBox(list mode)", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	loadMoviePlaylist();
	loadBox.hide();

	// load items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "mplay");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());
		//item->setOptionInfo("OptionInfo");

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());
		//item->setOptionInfo1("OptionInfo1");

		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());
		//item->setOptionInfo2("OptionInfo2");

		item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->set2lines();

		std::string tmp = m_vMovieInfo[i].epgTitle;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo1;
		tmp += "\n";
		tmp += m_vMovieInfo[i].epgInfo2;

		item->setHint(tmp.c_str());
		
		rightWidget->addItem(item);
	}

	// mode
	rightWidget->setWidgetType(WIDGET_TYPE_STANDARD);
	rightWidget->setWidgetMode(MODE_LISTBOX);
	rightWidget->enableCenterPos();
	rightWidget->enableShrinkMenu();

	//
	rightWidget->addWidget(WIDGET_TYPE_CLASSIC);
	rightWidget->addWidget(WIDGET_TYPE_EXTENDED);
	rightWidget->addWidget(WIDGET_TYPE_FRAME);
	rightWidget->enableWidgetChange();

	rightWidget->setItemsPerPage(5, 2);

	// head
	rightWidget->setTitle("ClistBox(listBox mode)", NEUTRINO_ICON_MOVIE);
	rightWidget->enablePaintHead();
	rightWidget->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	rightWidget->enablePaintDate();

	// footer
	rightWidget->enablePaintFoot();
	rightWidget->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);

	// footinfo
	rightWidget->enablePaintFootInfo(80);

	rightWidget->setSelected(selected);
	rightWidget->paint();
	
	CFrameBuffer::getInstance()->blit();

	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
	uint32_t sec_timer_id = 0;

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(1*1000*1000, false);

	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec

		if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
		{
			rightWidget->paintHead();
		} 
		else if (msg == RC_up)
		{
			rightWidget->onUpKeyPressed();
		}
		else if (msg == RC_down)
		{
			rightWidget->onDownKeyPressed();
		}
		else if (msg == RC_right)
		{
			rightWidget->onRightKeyPressed();
		}
		else if (msg == RC_left)
		{
			rightWidget->onLeftKeyPressed();
		}
		else if (msg == RC_page_up)
		{
			rightWidget->onPageUpKeyPressed();
		}
		else if (msg == RC_page_down)
		{
			rightWidget->onPageDownKeyPressed();
		}
		else if (msg == RC_home) 
		{
			loop = false;
		}
		else if(msg == RC_ok)
		{
			rightWidget->oKKeyPressed(this);
			rightWidget->paint();
		}
		else if(msg == RC_info)
		{
			rightWidget->hide();

			selected = rightWidget->getSelected();
			m_movieInfo.showMovieInfo(m_vMovieInfo[selected]);
			
			rightWidget->paint();
		}
		else if(msg == RC_setup)
		{
			rightWidget->changeWidgetType();
		}

		CFrameBuffer::getInstance()->blit();
	}

	rightWidget->hide();
	delete rightWidget;
	rightWidget = NULL;

	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;
}

void CTestMenu::testCFrameBox()
{
	dprintf(DEBUG_NORMAL, "\ntestCFrameBox\n");

	// frameBox
	CBox topBox;
	
	topBox.iWidth = (g_settings.screen_EndX - g_settings.screen_StartX - 20)/4;
	topBox.iHeight = (g_settings.screen_EndY - g_settings.screen_StartY - 20);
	topBox.iX = g_settings.screen_StartX + ((g_settings.screen_EndX - g_settings.screen_StartX - 20) - topBox.iWidth )>> 1;
	topBox.iY = g_settings.screen_StartY + 10;

	frameBoxWidget = new CFrameBox(&topBox);
	frameBoxWidget->enablePaintShadow();
	frameBoxWidget->enableSaveScreen();
	frameBoxWidget->setColor(/*COL_DARK_BLUE*/0);

	CFrame * frame = NULL;

	frame = new CFrame();
	frame->setPosition(topBox.iX + 10, topBox.iY + 10, topBox.iWidth - 20, 60);
	frame->setTitle("Neu Filme");
	frame->setIconName(NEUTRINO_ICON_MOVIE);
	frame->setOption("in allen Kinos");
	frame->setActionKey(this, "help");
	frame->setColor(COL_RED);
	frame->setGradient(DARK2LIGHT2DARK);
	frameBoxWidget->addFrame(frame);
	
	frame = new CFrame();
	frame->setPosition(topBox.iX + 10, topBox.iY + 10 + 60 + 10, topBox.iWidth - 20, 60);
	frame->setTitle("Im Kino");
	frame->setActionKey(this, "help");
	frame->setColor(COL_GREEN);
	frame->setGradient(LIGHT2DARK);
	//frame->setCorner(RADIUS_LARGE, CORNER_ALL);
	//frame->disableShadow();
	frameBoxWidget->addFrame(frame);

	frame = new CFrame();
	frame->setPosition(topBox.iX + 10, topBox.iY + 2*(10 + 60) +10, topBox.iWidth - 20, 60);
	frame->setTitle("Am populärsten");
	frame->setOption("(2019)");
	frame->setActionKey(this, "help");
	frame->setColor(COL_LIGHT_GRAY);
	frameBoxWidget->addFrame(frame);

	frame = new CFrame();
	frame->setPosition(topBox.iX + 10, topBox.iY + topBox.iHeight - 60 - 10, topBox.iWidth - 20, 60);
	frame->setTitle("Exit");
	frame->setActionKey(this, "exit");
	frame->setGradient(LIGHT2DARK);
	frame->setColor(COL_BLUE);
	//frame->setCorner(RADIUS_LARGE, CORNER_ALL);
	//frame->disableShadow();
	frameBoxWidget->addFrame(frame);
	
	// pic
	frame = new CFrame();
	frame->setMode(FRAME_PICTURE);
	frame->setPosition(topBox.iX + 10, topBox.iY + 3*(10 + 60) +50, topBox.iWidth - 20, 200);
	frame->setActive(false);
	frame->setIconName(DATADIR "/neutrino/icons/nopreview.jpg");
	frameBoxWidget->addFrame(frame);

	frameBoxWidget->setSelected(selected);

REPEAT:
	frameBoxWidget->paint();
	
	CFrameBuffer::getInstance()->blit();

	// loop
	neutrino_msg_t msg;
	neutrino_msg_data_t data;

	bool loop = true;

	while(loop)
	{
		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec

		if ( (msg == RC_home) || (frameBoxWidget->getSelected() == 9) )
		{
			loop = false;
		}
		else if(msg == RC_right)
		{
			frameBoxWidget->swipRight();
		}
		else if(msg == RC_left)
		{
			frameBoxWidget->swipLeft();
		}
		else if(msg == RC_down)
		{
			frameBoxWidget->scrollLineDown();
		}
		else if(msg == RC_up)
		{
			frameBoxWidget->scrollLineUp();
		}
		else if(msg == RC_ok)
		{
			int rv = frameBoxWidget->oKKeyPressed(this);

			//FIXME:review this
			switch ( rv ) 
			{
				case RETURN_EXIT_ALL:
					loop = false;
				case RETURN_EXIT:
					loop = false;
					break;
				case RETURN_REPAINT:
					hide();
					frameBoxWidget->paint();
					break;
			}
		}

		CFrameBuffer::getInstance()->blit();
	}

	hide();

	delete frameBoxWidget;
	frameBoxWidget = NULL;
}

// play Movie Url
void CTestMenu::testPlayMovieURL()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testPlayMovieURL\n");
	
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
		
	CFileBrowser * fileBrowser;
	
	fileBrowser = new CFileBrowser();

	fileFilter.clear();
	
	fileFilter.addFilter("ts");
	fileFilter.addFilter("mpg");
	fileFilter.addFilter("mpeg");
	fileFilter.addFilter("divx");
	fileFilter.addFilter("avi");
	fileFilter.addFilter("mkv");
	fileFilter.addFilter("asf");
	fileFilter.addFilter("aiff");
	fileFilter.addFilter("m2p");
	fileFilter.addFilter("mpv");
	fileFilter.addFilter("m2ts");
	fileFilter.addFilter("vob");
	fileFilter.addFilter("mp4");
	fileFilter.addFilter("mov");	
	fileFilter.addFilter("flv");	
	fileFilter.addFilter("dat");
	fileFilter.addFilter("trp");
	fileFilter.addFilter("vdr");
	fileFilter.addFilter("mts");
	fileFilter.addFilter("wmv");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("wma");
	fileFilter.addFilter("ogg");

	fileBrowser->Multi_Select    = false;
	fileBrowser->Dirs_Selectable = false;
	fileBrowser->Filter = &fileFilter;
	
	std::string Path_local = g_settings.network_nfs_moviedir;

BROWSER:
	if (fileBrowser->exec(Path_local.c_str()))
	{
		Path_local = fileBrowser->getCurrentDir();
		
		CFile * file;
		
		if ((file = fileBrowser->getSelectedFile()) != NULL) 
		{		
			tmpMoviePlayerGui.addToPlaylist(*file);
			tmpMoviePlayerGui.exec(NULL, "");
		}

		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec
		
		if (msg != RC_home) 
		{
			goto BROWSER;
		}
	}
	
	delete fileBrowser;
}

// play audio Url
void CTestMenu::testPlayAudioURL()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testPlayAudioURL\n");
	
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
		
	CFileBrowser * fileBrowser;
	
	fileBrowser = new CFileBrowser();

	fileFilter.clear();
	
	fileFilter.addFilter("cdr");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("mpa");
	fileFilter.addFilter("mp2");
	fileFilter.addFilter("ogg");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("aac");
	fileFilter.addFilter("dts");
	fileFilter.addFilter("m4a");
	
	fileBrowser->Multi_Select = false;
	fileBrowser->Dirs_Selectable = false;
	fileBrowser->Filter = &fileFilter;
	
	std::string Path_local = g_settings.network_nfs_audioplayerdir;

BROWSER:
	if (fileBrowser->exec(Path_local.c_str()))
	{
		Path_local = fileBrowser->getCurrentDir();
		
		CFile * file;
		
		if ((file = fileBrowser->getSelectedFile()) != NULL) 
		{	
			if (file->getType() == CFile::FILE_AUDIO)
			{
				tmpAudioPlayerGui.addToPlaylist(*file);
				tmpAudioPlayerGui.exec(NULL, "urlplayback");
			}
		}

		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec
		
		if (msg != RC_home) 
		{
			goto BROWSER;
		}
	}
	
	delete fileBrowser;
}

// show pic url
void CTestMenu::testShowPictureURL()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testShowPictureURL\n");
	
	neutrino_msg_t msg;
	neutrino_msg_data_t data;

	CFileBrowser * fileBrowser;
	
	fileBrowser = new CFileBrowser();

	fileFilter.clear();
	
	fileFilter.addFilter("png");
	fileFilter.addFilter("bmp");
	fileFilter.addFilter("jpg");
	fileFilter.addFilter("jpeg");
	
	fileBrowser->Multi_Select    = false;
	fileBrowser->Dirs_Selectable = false;
	fileBrowser->Filter = &fileFilter;
	
	std::string Path_local = g_settings.network_nfs_picturedir;

BROWSER:
	if (fileBrowser->exec(Path_local.c_str()))
	{
		Path_local = fileBrowser->getCurrentDir();
		
		CFile * file;
		
		if ((file = fileBrowser->getSelectedFile()) != NULL) 
		{					
			tmpPictureViewerGui.addToPlaylist(*file);
			tmpPictureViewerGui.exec(NULL, "");
		}

		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec
		
		if (msg != RC_home) 
		{
			goto BROWSER;
		}
	}
	
	delete fileBrowser;
}

// play movie folder
void CTestMenu::testPlayMovieFolder()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testPlayMovieFolder\n");
	
	CFileBrowser * fileBrowser;
	
	fileBrowser = new CFileBrowser();

	fileFilter.clear();
	
	fileFilter.addFilter("ts");
	fileFilter.addFilter("mpg");
	fileFilter.addFilter("mpeg");
	fileFilter.addFilter("divx");
	fileFilter.addFilter("avi");
	fileFilter.addFilter("mkv");
	fileFilter.addFilter("asf");
	fileFilter.addFilter("aiff");
	fileFilter.addFilter("m2p");
	fileFilter.addFilter("mpv");
	fileFilter.addFilter("m2ts");
	fileFilter.addFilter("vob");
	fileFilter.addFilter("mp4");
	fileFilter.addFilter("mov");	
	fileFilter.addFilter("flv");	
	fileFilter.addFilter("dat");
	fileFilter.addFilter("trp");
	fileFilter.addFilter("vdr");
	fileFilter.addFilter("mts");
	fileFilter.addFilter("wmv");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("wma");
	fileFilter.addFilter("ogg");

	fileBrowser->Multi_Select = true;
	fileBrowser->Filter = &fileFilter;
	
	std::string Path_local = g_settings.network_nfs_moviedir;

BROWSER:
	if (fileBrowser->exec(Path_local.c_str()))
	{
		Path_local = fileBrowser->getCurrentDir();
		
		CFileList::const_iterator files = fileBrowser->getSelectedFiles().begin();
		for(; files != fileBrowser->getSelectedFiles().end(); files++)
		{		
			tmpMoviePlayerGui.addToPlaylist(*files);
		}
		
		tmpMoviePlayerGui.exec(NULL, "urlplayback");
		
		neutrino_msg_t msg;
		neutrino_msg_data_t data;

		g_RCInput->getMsg_ms(&msg, &data, 10);
		
		if (msg != RC_home) 
		{
			goto BROWSER;
		}
	}
	
	delete fileBrowser;
}

// play audio folder
void CTestMenu::testPlayAudioFolder()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testPlayAudioFolder\n");
	
	neutrino_msg_t msg;
	neutrino_msg_data_t data;
		
	CFileBrowser * fileBrowser;

	fileFilter.clear();
	
	fileFilter.addFilter("cdr");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("m2a");
	fileFilter.addFilter("mpa");
	fileFilter.addFilter("mp2");
	fileFilter.addFilter("ogg");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("aac");
	fileFilter.addFilter("dts");
	fileFilter.addFilter("m4a");
	
	fileBrowser = new CFileBrowser();
	fileBrowser->Multi_Select = true;
	fileBrowser->Dirs_Selectable = false;
	fileBrowser->Filter = &fileFilter;
	
	std::string Path_local = g_settings.network_nfs_audioplayerdir;

BROWSER:
	if (fileBrowser->exec(Path_local.c_str()))
	{
		Path_local = fileBrowser->getCurrentDir();

		CFileList::const_iterator files = fileBrowser->getSelectedFiles().begin();
		
		for(; files != fileBrowser->getSelectedFiles().end(); files++)
		{

			if ( (files->getExtension() == CFile::EXTENSION_CDR)
					||  (files->getExtension() == CFile::EXTENSION_MP3)
					||  (files->getExtension() == CFile::EXTENSION_WAV)
					||  (files->getExtension() == CFile::EXTENSION_FLAC)
			)
			{
				tmpAudioPlayerGui.addToPlaylist(*files);
			}
		}
		
		tmpAudioPlayerGui.exec(NULL, "urlplayback");

		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec
		
		if (msg != RC_home) 
		{
			goto BROWSER;
		}
	}
	
	delete fileBrowser;
}

// show pic folder
void CTestMenu::testShowPictureFolder()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testShowPictureFolder\n");
	
	neutrino_msg_t msg;
	neutrino_msg_data_t data;

	CFileBrowser * fileBrowser;
	
	fileFilter.clear();
	
	fileFilter.addFilter("png");
	fileFilter.addFilter("bmp");
	fileFilter.addFilter("jpg");
	fileFilter.addFilter("jpeg");
	
	fileBrowser = new CFileBrowser();
	fileBrowser->Multi_Select    = true;
	fileBrowser->Dirs_Selectable = true;
	fileBrowser->Filter = &fileFilter;
	
	std::string Path_local = g_settings.network_nfs_picturedir;

BROWSER:
	if (fileBrowser->exec(Path_local.c_str()))
	{
		Path_local = fileBrowser->getCurrentDir();
				
		CFileList::const_iterator files = fileBrowser->getSelectedFiles().begin();
		
		for(; files != fileBrowser->getSelectedFiles().end(); files++)
		{

			if (files->getType() == CFile::FILE_PICTURE)
			{
				tmpPictureViewerGui.addToPlaylist(*files);
			}
		}
		
		tmpPictureViewerGui.exec(NULL, "urlplayback");

		g_RCInput->getMsg_ms(&msg, &data, 10); // 1 sec
		
		if (msg != RC_home) 
		{
			goto BROWSER;
		}
	}
	
	delete fileBrowser;
}

// start plugin
void CTestMenu::testStartPlugin()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testStartPlugin\n");
	
	g_PluginList->startPlugin("youtube");
}

// show actuell epg
void CTestMenu::testShowActuellEPG()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testShowActuelEPG\n");
	
	std::string title = "testShowActuellEPG:";
	std::string buffer;

	// get EPG
	CEPGData epgData;
	event_id_t epgid = 0;
			
	if(sectionsd_getActualEPGServiceKey(CNeutrinoApp::getInstance()->channelList->getActiveChannel_ChannelID(), &epgData))
		epgid = epgData.eventID;

	if(epgid != 0) 
	{
		CShortEPGData epgdata;
				
		if(sectionsd_getEPGidShort(epgid, &epgdata)) 
		{
			title += CNeutrinoApp::getInstance()->channelList->getActiveChannelName();
			title += ":";
			title += epgdata.title;

			buffer = epgdata.info1;
			buffer += "\n";
			buffer += epgdata.info2;	
		}
	}

	title += getNowTimeStr("%d.%m.%Y %H:%M");
	//
	
	CBox position(g_settings.screen_StartX + 50, g_settings.screen_StartY + 50, g_settings.screen_EndX - g_settings.screen_StartX - 100, g_settings.screen_EndY - g_settings.screen_StartY - 100); 
	
	CInfoBox * infoBox = new CInfoBox(g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1], SCROLL, &position, title.c_str(), g_Font[SNeutrinoSettings::FONT_TYPE_EPG_TITLE], NULL);

	infoBox->setText(buffer.c_str());
	
	infoBox->exec();
	delete infoBox;
	infoBox = NULL;	
}

// CChannelSelect
void CTestMenu::testChannelSelectWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testChannelSelectWidget\n");
	
	CSelectChannelWidget * CSelectChannelWidgetHandler = new CSelectChannelWidget();

	std::string mode = "tv";

	if(CNeutrinoApp::getInstance()->getMode() == NeutrinoMessages::mode_tv)
		mode = "tv";
	else if(CNeutrinoApp::getInstance()->getMode() == NeutrinoMessages::mode_radio)
		mode = "radio";
	else if(CNeutrinoApp::getInstance()->getMode() == NeutrinoMessages::mode_webtv)
		mode = "webtv";

	CSelectChannelWidgetHandler->exec(NULL, mode);
		
	//CSelectChannelWidget_TVChanID;
	//CSelectChannelWidget_TVChanName.c_str();

	printf("testChannelSelectWidget: chan_id:%llx chan_name:%s\n", CSelectChannelWidgetHandler->getChannelID(), g_Zapit->getChannelName(CSelectChannelWidgetHandler->getChannelID()).c_str());
		
	delete CSelectChannelWidgetHandler;
	CSelectChannelWidgetHandler = NULL;
}

// Bedit
void CTestMenu::testBEWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testBEWidget\n");
		
	CBEBouquetWidget* BEWidget = new CBEBouquetWidget();

	BEWidget->exec(NULL, "");

	delete BEWidget;
	BEWidget = NULL;
}

// AVSelect
void CTestMenu::testAVSelectWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testAVSelectWidget\n");
	
	CAVPIDSelectWidget * AVSelectHandler = new CAVPIDSelectWidget();
	AVSelectHandler->exec(NULL, "");
		
	delete AVSelectHandler;
	AVSelectHandler = NULL;
}

// CAudioSelect
void CTestMenu::testAudioSelectWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testAudioSelectWidget\n");
	
	CAudioSelectMenuHandler * ASelectHandler = new CAudioSelectMenuHandler();
	ASelectHandler->exec(NULL, "");
	delete ASelectHandler;
	ASelectHandler = NULL;
}

// DVBSubSelect
void CTestMenu::testDVBSubSelectWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testDVBSelectWidget\n");
	
	CDVBSubSelectMenuHandler * dvbSubSelectHandler = new CDVBSubSelectMenuHandler();
	dvbSubSelectHandler->exec(NULL, "");
	delete dvbSubSelectHandler;
	dvbSubSelectHandler = NULL;
}

void CTestMenu::testAlphaSetupWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testAlphaSetup\n");
	
	CAlphaSetup * alphaSetup = new CAlphaSetup(LOCALE_COLORMENU_GTX_ALPHA, &g_settings.gtx_alpha);
	alphaSetup->exec(NULL, "");
	delete alphaSetup;
	alphaSetup = NULL;
}

void CTestMenu::testPSISetup()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testPSISetup\n");
	
	CPSISetup * psiSetup = new CPSISetup(LOCALE_VIDEOMENU_PSISETUP, &g_settings.contrast, &g_settings.saturation, &g_settings.brightness, &g_settings.tint);
	psiSetup->exec(NULL, "");
	delete psiSetup;
	psiSetup = NULL;
}

void CTestMenu::testRCLock()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testRCLock\n");
	
	CRCLock * rcLock = new CRCLock();
	rcLock->exec(NULL, CRCLock::NO_USER_INPUT);
	delete rcLock;
	rcLock = NULL;
}

void CTestMenu::testSleepTimerWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testSleepTimerWidget\n");
	
	CSleepTimerWidget * sleepTimerHandler = new CSleepTimerWidget();
	sleepTimerHandler->exec(NULL, "");
	delete sleepTimerHandler;
	sleepTimerHandler = NULL;
}

void CTestMenu::testMountGUI()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testMountGUI\n");
	
	CNFSMountGui * mountGUI = new CNFSMountGui();
	mountGUI->exec(NULL, "");
	delete mountGUI;
	mountGUI = NULL;
}

void CTestMenu::testUmountGUI()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testUmountGUI\n");
	
	CNFSUmountGui * umountGUI = new CNFSUmountGui();
	umountGUI->exec(NULL, "");
	delete umountGUI;
	umountGUI = NULL;
}

void CTestMenu::testMountSmallMenu()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testMountSmallMenu\n");
	
	CNFSSmallMenu * mountSmallMenu = new CNFSSmallMenu();
	mountSmallMenu->exec(NULL, "");
	delete mountSmallMenu;
	mountSmallMenu = NULL;
}

void CTestMenu::testVFDController()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testVFDController\n");
	
	CVfdControler * vfdControllerHandler = new CVfdControler(LOCALE_LCDMENU_HEAD, NULL);
	vfdControllerHandler->exec(NULL, "");
	delete vfdControllerHandler;
	vfdControllerHandler = NULL;
}

void CTestMenu::testColorChooser()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testColorChooser\n");
	
	CColorChooser * colorChooserHandler = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Head_red, &g_settings.menu_Head_green, &g_settings.menu_Head_blue, &g_settings.menu_Head_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	colorChooserHandler->exec(NULL, "");
	//delete colorChooserHandler;
	//colorChooserHandler = NULL;
}

void CTestMenu::testKeyChooser()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testKeyChooser\n");
	
	CKeyChooserItem * keyChooser = new CKeyChooserItem("testing CKeyChooser", &g_settings.key_screenshot);

	keyChooser->exec(NULL, "");
	delete keyChooser;
	keyChooser = NULL;
}

void CTestMenu::testMountChooser()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testMountChooser\n");
	
	CMountChooser * mountChooser = new CMountChooser("testing CMountChooser", NEUTRINO_ICON_SETTINGS, NULL, g_settings.network_nfs_moviedir, g_settings.network_nfs_recordingdir);

	mountChooser->exec(NULL, "");
	delete mountChooser;
	mountChooser = NULL;
}

void CTestMenu::testPluginsList()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testPluginlist\n");
	
	CPluginList * pluginList = new CPluginList();
	pluginList->exec(NULL, "");
	delete pluginList;
	pluginList = NULL;
}

void CTestMenu::testPlayMovieDir()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testPlayMovieDi\n");
	
	filelist.clear();
	fileFilter.clear();
	
	fileFilter.addFilter("ts");
	fileFilter.addFilter("mpg");
	fileFilter.addFilter("mpeg");
	fileFilter.addFilter("divx");
	fileFilter.addFilter("avi");
	fileFilter.addFilter("mkv");
	fileFilter.addFilter("asf");
	fileFilter.addFilter("aiff");
	fileFilter.addFilter("m2p");
	fileFilter.addFilter("mpv");
	fileFilter.addFilter("m2ts");
	fileFilter.addFilter("vob");
	fileFilter.addFilter("mp4");
	fileFilter.addFilter("mov");	
	fileFilter.addFilter("flv");	
	fileFilter.addFilter("dat");
	fileFilter.addFilter("trp");
	fileFilter.addFilter("vdr");
	fileFilter.addFilter("mts");
	fileFilter.addFilter("wmv");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("wma");
	fileFilter.addFilter("ogg");
	
	std::string Path_local = g_settings.network_nfs_moviedir;

	if(CFileHelpers::getInstance()->readDir(Path_local, &filelist, &fileFilter))
	{
		CFileList::iterator files = filelist.begin();
		for(; files != filelist.end() ; files++)
		{
			tmpMoviePlayerGui.addToPlaylist(*files);
		}
		
		tmpMoviePlayerGui.exec(NULL, "urlplayback");
	}
}

void CTestMenu::testPlayAudioDir()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testPlayAudioDir\n");
	
	filelist.clear();
	fileFilter.clear();
	
	fileFilter.addFilter("cdr");
	fileFilter.addFilter("mp3");
	fileFilter.addFilter("m2a");
	fileFilter.addFilter("mpa");
	fileFilter.addFilter("mp2");
	fileFilter.addFilter("ogg");
	fileFilter.addFilter("wav");
	fileFilter.addFilter("flac");
	fileFilter.addFilter("aac");
	fileFilter.addFilter("dts");
	fileFilter.addFilter("m4a");
	
	std::string Path_local = g_settings.network_nfs_audioplayerdir;

	if(CFileHelpers::getInstance()->readDir(Path_local, &filelist, &fileFilter))
	{
		// filter them
		CFileList::iterator files = filelist.begin();
		for(; files != filelist.end() ; files++)
		{
			if ( (files->getExtension() == CFile::EXTENSION_CDR)
					||  (files->getExtension() == CFile::EXTENSION_MP3)
					||  (files->getExtension() == CFile::EXTENSION_WAV)
					||  (files->getExtension() == CFile::EXTENSION_FLAC)
			)
			{
				tmpAudioPlayerGui.addToPlaylist(*files);
			}
		}
		
		tmpAudioPlayerGui.exec(NULL, "");
	}
}

void CTestMenu::testShowPictureDir()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testShowPictureDir\n");
	
	filelist.clear();
	fileFilter.clear();

	fileFilter.addFilter("png");
	fileFilter.addFilter("bmp");
	fileFilter.addFilter("jpg");
	fileFilter.addFilter("jpeg");
	
	std::string Path_local = g_settings.network_nfs_picturedir;

	if(CFileHelpers::getInstance()->readDir(Path_local, &filelist, &fileFilter))
	{		
		CFileList::iterator files = filelist.begin();
		for(; files != filelist.end() ; files++)
		{
			if (files->getType() == CFile::FILE_PICTURE)
			{
				tmpPictureViewerGui.addToPlaylist(*files);
			}
		}
		
		tmpPictureViewerGui.exec(NULL, "urlplayback");
	}
}

// CMenuWidget (listBox mode)
void CTestMenu::testClistBoxWidget()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testCMenuWidget (listBox mode)\n");
	
	// our listBox
	listMenu = new CMenuWidget("CMenuWidget(listBox Mode)", NEUTRINO_ICON_MOVIE, w_max ( (CFrameBuffer::getInstance()->getScreenWidth() / 20 * 17), (CFrameBuffer::getInstance()->getScreenWidth() / 20 )), h_max ( (CFrameBuffer::getInstance()->getScreenHeight() / 20 * 17), (CFrameBuffer::getInstance()->getScreenHeight() / 20)));

	//
	CHintBox loadBox("CMenuWidget(listBox mode)", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	loadMoviePlaylist();
	loadBox.hide();
	
	// add items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "mplay", RC_nokey, NULL, file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());
		item->set2lines();

		item->setHint(m_vMovieInfo[i].epgInfo2.c_str());

		// standard | classic
		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());
		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

		listMenu->addItem(item);
	}

	listMenu->setWidgetMode(MODE_LISTBOX);
	listMenu->setWidgetType(WIDGET_TYPE_STANDARD);
	listMenu->addWidget(WIDGET_TYPE_CLASSIC);
	listMenu->addWidget(WIDGET_TYPE_EXTENDED);
	listMenu->addWidget(WIDGET_TYPE_FRAME);
	listMenu->enableWidgetChange();
	listMenu->setItemsPerPage(6, 2);
	//listMenu->enableShrinkMenu();

	listMenu->setSelected(selected);

	// head
	listMenu->enablePaintDate();
	listMenu->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);

	// foot
	listMenu->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);
	
	// footInfo
	listMenu->enablePaintFootInfo(80);
	listMenu->setFootInfoMode(FOOT_HINT_MODE);

	listMenu->addKey(RC_info, this, "minfo");

	listMenu->exec(NULL, "");
	listMenu->hide();
	delete listMenu;
	listMenu = NULL;
}

// CMenuWidget (menu mode)
void CTestMenu::testClistBoxWidget1()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testCMenuWidget (menu mode)\n");
	
	// our listBox
	listMenu = new CMenuWidget("CMenuWidget(Menu Mode)", NEUTRINO_ICON_MOVIE, w_max ( (CFrameBuffer::getInstance()->getScreenWidth() / 20 * 17), (CFrameBuffer::getInstance()->getScreenWidth() / 20 )), h_max ( (CFrameBuffer::getInstance()->getScreenHeight() / 20 * 17), (CFrameBuffer::getInstance()->getScreenHeight() / 20)));

	//
	CHintBox loadBox("CMenuWidget(menu mode)", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();
	loadMoviePlaylist();
	loadBox.hide();
	
	// add items
	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "mplay", RC_nokey, NULL, file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->setOption(m_vMovieInfo[i].epgChannel.c_str());

		item->set2lines();

		item->setHint(m_vMovieInfo[i].epgInfo2.c_str());

		item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());
		item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

		listMenu->addItem(item);
	}

	listMenu->setWidgetMode(MODE_MENU);
	listMenu->setItemsPerPage(6, 2);
	listMenu->enableWidgetChange();
	//listMenu->enableShrinkMenu();

	listMenu->setSelected(selected);

	// head
	listMenu->enablePaintDate();
	listMenu->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);

	// foot
	listMenu->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);

	// foot info in menu mode are always enabled
	listMenu->enablePaintFootInfo();
	listMenu->setFootInfoMode(FOOT_HINT_MODE);

	listMenu->addKey(RC_info, this, "minfo");

	listMenu->exec(NULL, "");
	listMenu->hide();
	delete listMenu;
	listMenu = NULL;
}

// CMenuWidget (setup mode)
void CTestMenu::testClistBoxWidget2()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testCMenuWidget (setup mode)\n");
	
	CAudioPlayerSettings * audioPlayerSettingsMenu = new CAudioPlayerSettings();

	audioPlayerSettingsMenu->exec(this, "");
	delete audioPlayerSettingsMenu;
	audioPlayerSettingsMenu = NULL;	
}

// CChannellist
void CTestMenu::testChannellist()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testChannellist\n");
	
	webTVchannelList = new CChannelList("CTestMenu::testWebTVChannellist:");

	for (tallchans_iterator it = allchans.begin(); it != allchans.end(); it++) 
	{
		if (it->second.isWebTV) 
		{
			webTVchannelList->addChannel(&(it->second));
		}
	}

	webTVchannelList->exec(); // with zap
}


// CBouquetlist
void CTestMenu::testBouquetlist()
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::testBouquetlist\n");
	
	webTVBouquetList = new CBouquetList("CTestMenu::testWebTVBouquetlist");

	for (int i = 0; i < g_bouquetManager->Bouquets.size(); i++) 
	{
		if (g_bouquetManager->Bouquets[i]->bWebTV && !g_bouquetManager->Bouquets[i]->webtvChannels.empty())
		{
			CBouquet *ltmp = webTVBouquetList->addBouquet(g_bouquetManager->Bouquets[i]);

			ZapitChannelList *channels = &(g_bouquetManager->Bouquets[i]->webtvChannels);
			ltmp->channelList->setSize(channels->size());

			for(int j = 0; j < (int) channels->size(); j++) 
			{
				ltmp->channelList->addChannel((*channels)[j]);
			}
		}
	}

	webTVBouquetList->exec(true); // with zap
}

// exec
int CTestMenu::exec(CMenuTarget *parent, const std::string &actionKey)
{
	dprintf(DEBUG_NORMAL, "\nCTestMenu::exec: actionKey:%s\n", actionKey.c_str());

	//int cnt = 0;
	
	if(parent)
		hide();
	
	if(actionKey == "testing")
	{
		test();

		return RETURN_REPAINT;
	}
	else if(actionKey == "widget")
	{
		testCWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "menuforwarder")
	{
		MessageBox(LOCALE_MESSAGEBOX_INFO, "testing CMenuForwarder", mbrBack, mbBack, NEUTRINO_ICON_INFO);

		return RETURN_REPAINT;
	}
	else if(actionKey == "listboxitem")
	{
		MessageBox(LOCALE_MESSAGEBOX_INFO, "testing ClistBoxItem", mbrBack, mbBack, NEUTRINO_ICON_INFO);

		return RETURN_REPAINT;
	}
	else if(actionKey == "box")
	{
		testCBox();

		return RETURN_REPAINT;
	}
	else if(actionKey == "icon")
	{
		testCIcon();

		return RETURN_REPAINT;
	}
	else if(actionKey == "image")
	{
		testCImage();

		return RETURN_REPAINT;
	}
	else if(actionKey == "window")
	{
		testCWindow();

		return RETURN_REPAINT;
	}
	else if(actionKey == "windowshadow")
	{
		testCWindowShadow();

		return RETURN_REPAINT;
	}
	else if(actionKey == "windowcustomcolor")
	{
		testCWindowCustomColor();
		return RETURN_REPAINT;
	}
	else if(actionKey == "headers")
	{
		testCHeaders();
		return RETURN_REPAINT;
	}
	else if(actionKey == "stringinput")
	{
		testCStringInput();

		return RETURN_REPAINT;
	}
	else if(actionKey == "stringinputsms")
	{
		testCStringInputSMS();

		return RETURN_REPAINT;
	}
	else if(actionKey == "pininput")
	{
		testCPINInput();

		return RETURN_REPAINT;
	}
	else if(actionKey == "plpininput")
	{
		testCPLPINInput();

		return RETURN_REPAINT;
	}
	else if(actionKey == "pinchangewidget")
	{
		testCPINChangeWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "ipinput")
	{
		testCIPInput();

		return RETURN_REPAINT;
	}
	else if(actionKey == "macinput")
	{
		testCMACInput();

		return RETURN_REPAINT;
	}
	else if(actionKey == "dateinput")
	{
		testCDateInput();

		return RETURN_REPAINT;
	}
	else if(actionKey == "timeinput")
	{
		testCTimeInput();

		return RETURN_REPAINT;
	}
	else if(actionKey == "intinput")
	{
		testCIntInput();

		return RETURN_REPAINT;
	}
	else if(actionKey == "infobox")
	{
		testCInfoBox();

		return RETURN_REPAINT;
	}
	else if(actionKey == "messagebox")
	{
		testCMessageBox();

		return RETURN_REPAINT;
	}
	else if(actionKey == "messageboxinfomsg")
	{
		testCMessageBoxInfoMsg();

		return RETURN_REPAINT;
	}
	else if(actionKey == "messageboxerrormsg")
	{
		testCMessageBoxErrorMsg();

		return RETURN_REPAINT;
	}
	else if(actionKey == "hintbox")
	{
		testCHintBox();

		return RETURN_REPAINT;
	}
	else if(actionKey == "hintboxinfo")
	{
		testCHintBoxInfo();

		return RETURN_REPAINT;
	}
	else if(actionKey == "helpbox")
	{
		testCHelpBox();

		return RETURN_REPAINT;
	}
	else if(actionKey == "textbox")
	{
		testCTextBox();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listframe")
	{
		testCListFrame();

		return RETURN_REPAINT;
	}
	else if(actionKey == "progressbar")
	{
		testCProgressBar();

		return RETURN_REPAINT;
	}
	else if(actionKey == "progresswindow")
	{
		testCProgressWindow();

		return RETURN_REPAINT;
	}
	else if(actionKey == "buttons")
	{
		testCButtons();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listbox")
	{
		testClistBox();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listbox2")
	{
		testClistBox2();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listbox3")
	{
		testClistBox3();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listbox4")
	{
		testClistBox4();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listbox5")
	{
		testClistBox5();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listbox6")
	{
		testClistBox6();

		return RETURN_REPAINT;
	}
	else if(actionKey == "framebox")
	{
		testCFrameBox();

		return RETURN_REPAINT;
	}
	else if(actionKey == "playmovieurl")
	{
		testPlayMovieURL();

		return RETURN_REPAINT;
	}
	else if(actionKey == "playaudiourl")
	{
		testPlayAudioURL();

		return RETURN_REPAINT;
	}
	else if(actionKey == "showpictureurl")
	{
		testShowPictureURL();

		return RETURN_REPAINT;
	}
	else if(actionKey == "playmoviefolder")
	{
		testPlayMovieFolder();

		return RETURN_REPAINT;
	}
	else if(actionKey == "playaudiofolder")
	{
		testPlayAudioFolder();

		return RETURN_REPAINT;
	}
	else if(actionKey == "showpicturefolder")
	{
		testShowPictureFolder();

		return RETURN_REPAINT;
	}
	else if(actionKey == "startplugin")
	{
		testStartPlugin();

		return RETURN_REPAINT;
	}
	else if(actionKey == "showepg")
	{
		testShowActuellEPG();

		return RETURN_REPAINT;
	}
	else if(actionKey == "channelselect")
	{
		testChannelSelectWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "bewidget")
	{
		testBEWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "avselect")
	{
		testAVSelectWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "aselect")
	{
		testAudioSelectWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "dvbsubselect")
	{
		testDVBSubSelectWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "alphasetup")
	{
		testAlphaSetupWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "psisetup")
	{
		testPSISetup();

		return RETURN_REPAINT;
	}
	else if(actionKey == "rclock")
	{
		testRCLock();

		return RETURN_REPAINT;
	}
	else if(actionKey == "sleeptimer")
	{
		testSleepTimerWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "mountgui")
	{
		testMountGUI();

		return RETURN_REPAINT;
	}
	else if(actionKey == "umountgui")
	{
		testUmountGUI();

		return RETURN_REPAINT;
	}
	else if(actionKey == "mountsmallmenu")
	{
		testMountSmallMenu();

		return RETURN_REPAINT;
	}
	else if(actionKey == "vfdcontroller")
	{
		testVFDController();

		return RETURN_REPAINT;
	}
	else if(actionKey == "colorchooser")
	{
		testColorChooser();

		return RETURN_REPAINT;
	}
	else if(actionKey == "keychooser")
	{
		testKeyChooser();

		return RETURN_REPAINT;
	}
	else if(actionKey == "mountchooser")
	{
		testMountChooser();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listboxwidget")
	{
		testClistBoxWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "pluginslist")
	{
		testPluginsList();

		return RETURN_REPAINT;
	}
	else if(actionKey == "playmoviedir")
	{
		testPlayMovieDir();

		return RETURN_REPAINT;
	}
	else if(actionKey == "playaudiodir")
	{
		testPlayAudioDir();

		return RETURN_REPAINT;
	}
	else if(actionKey == "showpicturedir")
	{
		testShowPictureDir();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listboxwidget1")
	{
		testClistBoxWidget1();

		return RETURN_REPAINT;
	}
	else if(actionKey == "listboxwidget2")
	{
		testClistBoxWidget2();

		return RETURN_REPAINT;
	}
	else if(actionKey == "mplay")
	{
		if (listMenu)
			selected = listMenu->getSelected();
		else
			selected = 0;

		if (&m_vMovieInfo[selected].file != NULL) 
		{
			//tmpMoviePlayerGui.addToPlaylist(m_vMovieInfo[selected]);
			//tmpMoviePlayerGui.exec(NULL, "");

			CMovieInfoWidget movieInfoWidget;
			movieInfoWidget.setMovie(m_vMovieInfo[selected]);
		
			movieInfoWidget.exec(NULL, "");
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "minfo")
	{
		if (listMenu)
			selected = listMenu->getSelected();
		else
			selected = 0;

		m_movieInfo.showMovieInfo(m_vMovieInfo[selected]);

		return RETURN_REPAINT;
	}
	else if(actionKey == "channellist")
	{
		testChannellist();

		return RETURN_REPAINT;
	}
	else if(actionKey == "bouquetlist")
	{
		testBouquetlist();

		return RETURN_REPAINT;
	}
	else if(actionKey == "winfo")
	{
		//if(testWidget->getSelected() == 3) // rightWidget onFocus
		if (rightWidget)
		{
			if (rightWidget->inFocus)
			{
				right_selected = rightWidget->getSelected();
				m_movieInfo.showMovieInfo(m_vMovieInfo[right_selected]);
			}
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "wok")
	{
		//int focus = testWidget->getSelected();

		//if(focus == 3)
		if (rightWidget->inFocus)
		{
			//hide();
/*
			right_selected = rightWidget->getSelected();

			///
			ytparser.Cleanup();

			// setregion
			ytparser.SetRegion("DE");

			// set max result
			ytparser.SetMaxResults(1);
			
			// parse feed
			if (ytparser.ParseFeed(cYTFeedParser::SEARCH_BY_ID, m_vMovieInfo[right_selected].vname, m_vMovieInfo[right_selected].vkey))
			{
				yt_video_list_t &ylist = ytparser.GetVideoList();
	
				for (unsigned int j = 0; j < ylist.size(); j++) 
				{
					m_vMovieInfo[right_selected].ytid = ylist[j].id;
					m_vMovieInfo[right_selected].file.Name = ylist[j].GetUrl();
				}
			} 
				///

			if (&m_vMovieInfo[right_selected].file != NULL) 
			{
				tmpMoviePlayerGui.addToPlaylist(m_vMovieInfo[right_selected]);
				tmpMoviePlayerGui.exec(NULL, "");
			}
*/
		}
		//else if(focus == 2)
		else if (leftWidget->inFocus)
		{
			left_selected = leftWidget->getSelected();

			if(top_selected == 0) // movies
			{
				if(left_selected == 0)
				{
					right_selected = 0;
					rightWidget->clearItems();
					//loadTMDBPlaylist("movie", "now_playing", 1);
					TVShows = "movie";
					plist = "now_playing";
					page = 1;

					CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
					loadBox.paint();
					loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
					loadBox.hide();

					// load items
					for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
					{
						item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

						item->setOption(m_vMovieInfo[i].epgChannel.c_str());

						item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

						item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

						item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

						rightWidget->addItem(item);
					}
				}
				else if(left_selected == 1)
				{
					right_selected = 0;
					rightWidget->clearItems();
					//loadTMDBPlaylist("movie", "popular", 1);
					TVShows = "movie";
					plist = "popular";
					page = 1;

					CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
					loadBox.paint();
					loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
					loadBox.hide();

					// load items
					for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
					{
						item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

						item->setOption(m_vMovieInfo[i].epgChannel.c_str());

						item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

						item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

						item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

						rightWidget->addItem(item);
					}
				}
				else if(left_selected == 2)
				{
					right_selected = 0;
					rightWidget->clearItems();
					//loadTMDBPlaylist("movie", "top_rated", 1);
					TVShows = "movie";
					plist = "top_rated";
					page = 1;

					CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
					loadBox.paint();
					loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
					loadBox.hide();
					
					// load items
					for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
					{
						item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

						item->setOption(m_vMovieInfo[i].epgChannel.c_str());

						item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

						item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

						item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

						rightWidget->addItem(item);
					}
				}
				else if(left_selected == 3)
				{
					right_selected = 0;
					rightWidget->clearItems();

					TVShows = "movie";
					plist = "upcoming";
					page = 1;

					CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
					loadBox.paint();
					loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
					loadBox.hide();

					// load items
					for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
					{
						item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

						item->setOption(m_vMovieInfo[i].epgChannel.c_str());

						item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

						item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

						item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

						rightWidget->addItem(item);
					}
				}
			}
			else if(top_selected == 1) // tv
			{
				if(left_selected == 0)
				{
					right_selected = 0;
					rightWidget->clearItems();
					//loadTMDBPlaylist("tv", "airing_today", 1);
					TVShows = "tv";
					plist = "airing_today";
					page = 1;

					CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
					loadBox.paint();
					loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
					loadBox.hide();

					// load items
					for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
					{
						item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

						item->setOption(m_vMovieInfo[i].epgChannel.c_str());

						item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

						item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

						item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

						rightWidget->addItem(item);
					}
				}
				else if(left_selected == 1)
				{
					right_selected = 0;
					rightWidget->clearItems();
					//loadTMDBPlaylist("tv", "on_the_air", 1);
					TVShows = "tv";
					plist = "on_the_air";
					page = 1;

					CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
					loadBox.paint();
					loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
					loadBox.hide();

					// load items
					for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
					{
						item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

						item->setOption(m_vMovieInfo[i].epgChannel.c_str());

						item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

						item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

						item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

						rightWidget->addItem(item);
					}
				}
				else if(left_selected == 2)
				{
					right_selected = 0;
					rightWidget->clearItems();
					//loadTMDBPlaylist("tv", "popular", 1);
					TVShows = "tv";
					plist = "popular";
					page = 1;

					CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
					loadBox.paint();
					loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
					loadBox.hide();

					// load items
					for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
					{
						item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

						item->setOption(m_vMovieInfo[i].epgChannel.c_str());

						item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

						item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

						item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

						rightWidget->addItem(item);
					}
				}
				else if(left_selected == 3)
				{
					right_selected = 0;
					rightWidget->clearItems();
					//loadTMDBPlaylist("tv", "top_rated", 1);
					TVShows = "tv";
					plist = "top_rated";
					page = 1;

					CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
					loadBox.paint();
					loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
					loadBox.hide();

					// load items
					for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
					{
						item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

						item->setOption(m_vMovieInfo[i].epgChannel.c_str());

						item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

						item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

						item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

						rightWidget->addItem(item);
					}
				}
			}

			if(left_selected == 8)
				return RETURN_EXIT_ALL;
		}
		//else if(focus == 1)
		else if (frameBoxWidget->inFocus)
		{
			top_selected = frameBoxWidget->getSelected();

			if(top_selected == 1) //tv
			{
				right_selected = 0;
				left_selected = 0;

				rightWidget->clearItems();
				//
				leftWidget->clearItems();

				ClistBoxItem *item1 = new ClistBoxItem("Heute auf");
				item1->setOption("Sendung");
				item1->set2lines();
				ClistBoxItem *item2 = new ClistBoxItem("Auf Sendung");
				ClistBoxItem *item3 = new ClistBoxItem("Am");
				item3->setOption("populärsten");
				item3->set2lines();
				ClistBoxItem *item4 = new ClistBoxItem("am");
				item4->setOption("besten bewertet");
				item4->set2lines();
				ClistBoxItem *item5 = new ClistBoxItem(NULL, false);
				ClistBoxItem *item6 = new ClistBoxItem(NULL, false);
				ClistBoxItem *item7 = new ClistBoxItem(NULL, false);
				ClistBoxItem *item8 = new ClistBoxItem(NULL, false);
				ClistBoxItem *item9 = new ClistBoxItem("Beenden");

				leftWidget->addItem(item1);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item2);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item3);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item4);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item5);
				leftWidget->addItem(item6);
				leftWidget->addItem(item7);
				leftWidget->addItem(item8);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item9);
				leftWidget->addItem(new CMenuSeparator(LINE));

				//loadTMDBPlaylist("tv", "airing_today", 1);
				TVShows = "tv";
				plist = "airing_today";
				page = 1;

				CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
				loadBox.paint();
				loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
				loadBox.hide();

				// load items
				for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
				{
					item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

					item->setOption(m_vMovieInfo[i].epgChannel.c_str());

					item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

					item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

					item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

					rightWidget->addItem(item);
				}

				leftWidget->setSelected(0);
				rightWidget->setSelected(0);
			}
			else if(top_selected == 0) // movie
			{
				right_selected = 0;
				left_selected = 0;

				rightWidget->clearItems();
				leftWidget->clearItems();

				ClistBoxItem *item1 = new ClistBoxItem("In den Kinos");
				ClistBoxItem *item2 = new ClistBoxItem("Am");
				item2->setOption("populärsten");
				item2->set2lines();
				ClistBoxItem *item3 = new ClistBoxItem("Am besten");
				item3->setOption("bewertet");
				item3->set2lines();
				ClistBoxItem *item4 = new ClistBoxItem("Neue Filme");
				ClistBoxItem *item5 = new ClistBoxItem(NULL, false);
				ClistBoxItem *item6 = new ClistBoxItem(NULL, false);
				ClistBoxItem *item7 = new ClistBoxItem(NULL, false);
				ClistBoxItem *item8 = new ClistBoxItem(NULL, false);
				ClistBoxItem *item9 = new ClistBoxItem("Beenden");

				leftWidget->addItem(item1);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item2);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item3);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item4);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item5);
				leftWidget->addItem(item6);
				leftWidget->addItem(item7);
				leftWidget->addItem(item8);
				leftWidget->addItem(new CMenuSeparator(LINE));
				leftWidget->addItem(item9);
				leftWidget->addItem(new CMenuSeparator(LINE));

				CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
				loadBox.paint();
				loadTMDBPlaylist();
				loadBox.hide();

				// load items
				for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
				{
					item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str());

					item->setOption(m_vMovieInfo[i].epgChannel.c_str());

					item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

					item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

					item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

					rightWidget->addItem(item);
				}

				leftWidget->setSelected(0);
				rightWidget->setSelected(0);
			}
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "listframewidget")
	{
		testListFrameWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "aok")
	{
		if(AudioPlaylist.size() > 0)
		{
			for (unsigned int i = 0; i < AudioPlaylist.size(); i++)
			{
				tmpAudioPlayerGui.addToPlaylist(AudioPlaylist[i]);
			}

			tmpAudioPlayerGui.setCurrent(listFrame->getSelectedLine());
			tmpAudioPlayerGui.exec(NULL, "");
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "listboxmwidget")
	{
		testListBoxWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "mmwplay")
	{
		selected = rightWidget->getSelected();

		if (&m_vMovieInfo[selected].file != NULL) 
		{
			//tmpMoviePlayerGui.addToPlaylist(m_vMovieInfo[selected]);
			//tmpMoviePlayerGui.exec(NULL, "");

			CMovieInfoWidget movieInfoWidget;
			movieInfoWidget.setMovie(m_vMovieInfo[selected]);
		
			movieInfoWidget.exec(NULL, "");
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "linfo")
	{
		selected = rightWidget->getSelected();
		m_movieInfo.showMovieInfo(m_vMovieInfo[selected]);

		return RETURN_REPAINT;
	}
	else if(actionKey == "lsetup")
	{
		if (rightWidget)
			rightWidget->changeWidgetType();

		return RETURN_NONE;
	}
	else if(actionKey == "textboxwidget")
	{
		testTextBoxWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "exit")
	{
		return RETURN_EXIT_ALL;
	}
	else if(actionKey == "movie_in_cinema")
	{
		right_selected = 0;
		rightWidget->clearItems();
		//loadTMDBPlaylist("movie", "now_playing", 1);
		TVShows = "movie";
		plist = "now_playing";
		page = 1;

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "movie_popular")
	{
		right_selected = 0;
		rightWidget->clearItems();
		//loadTMDBPlaylist("movie", "now_playing", 1);
		TVShows = "movie";
		plist = "popular";
		page = 1;

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "movie_top_rated")
	{
		right_selected = 0;
		rightWidget->clearItems();
		//loadTMDBPlaylist("movie", "now_playing", 1);
		TVShows = "movie";
		plist = "top_rated";
		page = 1;

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "movie_new")
	{
		right_selected = 0;
		rightWidget->clearItems();
		//loadTMDBPlaylist("movie", "now_playing", 1);
		TVShows = "movie";
		plist = "upcoming";
		page = 1;

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "tv_today")
	{
		right_selected = 0;
		rightWidget->clearItems();
		//loadTMDBPlaylist("movie", "now_playing", 1);
		TVShows = "tv";
		plist = "airing_today";
		page = 1;

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
		loadBox.hide();
		
		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "tv_on_air")
	{
		right_selected = 0;
		rightWidget->clearItems();
		//loadTMDBPlaylist("movie", "now_playing", 1);
		TVShows = "tv";
		plist = "on_the_air";
		page = 1;

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "tv_popular")
	{
		right_selected = 0;
		rightWidget->clearItems();
		//loadTMDBPlaylist("movie", "now_playing", 1);
		TVShows = "tv";
		plist = "popular";
		page = 1;

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "tv_top_rated")
	{
		right_selected = 0;
		rightWidget->clearItems();
		//loadTMDBPlaylist("movie", "now_playing", 1);
		TVShows = "tv";
		plist = "top_rated";
		page = 1;

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "yplay")
	{
		right_selected = rightWidget->getSelected();

		///
		ytparser.Cleanup();

		// setregion
		ytparser.SetRegion("DE");

		// set max result
		ytparser.SetMaxResults(1);
			
		// parse feed
		if (ytparser.ParseFeed(cYTFeedParser::SEARCH_BY_ID, m_vMovieInfo[right_selected].vname, m_vMovieInfo[right_selected].vkey))
		{
			yt_video_list_t &ylist = ytparser.GetVideoList();
	
			for (unsigned int j = 0; j < ylist.size(); j++) 
			{
				m_vMovieInfo[right_selected].ytid = ylist[j].id;
				m_vMovieInfo[right_selected].file.Name = ylist[j].GetUrl();
			}
		} 
			///

		if (&m_vMovieInfo[right_selected].file != NULL) 
		{
			//tmpMoviePlayerGui.addToPlaylist(m_vMovieInfo[right_selected]);
			//tmpMoviePlayerGui.exec(NULL, "");
			CMovieInfoWidget movieInfoWidget;
			movieInfoWidget.setMovie(m_vMovieInfo[right_selected]);
		
			movieInfoWidget.exec(NULL, "");
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "movie")
	{
		top_selected = frameBoxWidget->getSelected();

		right_selected = 0;
		left_selected = 0;

		rightWidget->clearItems();
		leftWidget->clearItems();

		ClistBoxItem *item1 = new ClistBoxItem("In den Kinos", true, NULL, this, "movie_in_cinema");
		ClistBoxItem *item2 = new ClistBoxItem("Am", true, NULL, this, "movie_popular");
		item2->setOption("populärsten");
		item2->set2lines();
		ClistBoxItem *item3 = new ClistBoxItem("Am besten", true, NULL, this, "movie_top_rated");
		item3->setOption("bewertet");
		item3->set2lines();
		ClistBoxItem *item4 = new ClistBoxItem("Neue Filme", true, NULL, this, "movie_new");
		CMenuSeparator *item5 = new CMenuSeparator();
		CMenuSeparator *item6 = new CMenuSeparator();
		CMenuSeparator *item7 = new CMenuSeparator();
		CMenuSeparator *item8 = new CMenuSeparator();
		ClistBoxItem *item9 = new ClistBoxItem("Beenden", true, NULL, this, "exit");

		leftWidget->addItem(item1);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item2);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item3);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item4);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item5);
		leftWidget->addItem(item6);
		leftWidget->addItem(item7);
		leftWidget->addItem(item8);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item9);
		leftWidget->addItem(new CMenuSeparator(LINE));

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist();
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		leftWidget->setSelected(0);
		rightWidget->setSelected(0);

		return RETURN_REPAINT;
	}
	else if(actionKey == "tv")
	{
		right_selected = 0;
		left_selected = 0;

		rightWidget->clearItems();
				//
		leftWidget->clearItems();

		ClistBoxItem *item1 = new ClistBoxItem("Heute auf", true, NULL, this, "tv_today");
		item1->setOption("Sendung");
		item1->set2lines();
		ClistBoxItem *item2 = new ClistBoxItem("Auf Sendung", true, NULL, this, "tv_on_air");
		ClistBoxItem *item3 = new ClistBoxItem("Am", true, NULL, this, "tv_popular");
		item3->setOption("populärsten");
		item3->set2lines();
		ClistBoxItem *item4 = new ClistBoxItem("am", true, NULL, this, "tv_top_rated");
		item4->setOption("besten bewertet");
		item4->set2lines();
		CMenuSeparator *item5 = new CMenuSeparator();
		CMenuSeparator *item6 = new CMenuSeparator();
		CMenuSeparator *item7 = new CMenuSeparator();
		CMenuSeparator *item8 = new CMenuSeparator();
		ClistBoxItem *item9 = new ClistBoxItem("Beenden", true, NULL, this, "exit");

		leftWidget->addItem(item1);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item2);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item3);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item4);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item5);
		leftWidget->addItem(item6);
		leftWidget->addItem(item7);
		leftWidget->addItem(item8);
		leftWidget->addItem(new CMenuSeparator(LINE));
		leftWidget->addItem(item9);
		leftWidget->addItem(new CMenuSeparator(LINE));

		//loadTMDBPlaylist("tv", "airing_today", 1);
		TVShows = "tv";
		plist = "airing_today";
		page = 1;

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		leftWidget->setSelected(0);
		rightWidget->setSelected(0);

		return RETURN_REPAINT;
	}
	else if(actionKey == "nextPage")
	{
		top_selected = frameBoxWidget->getSelected();
		page++;
		right_selected = 0;
		rightWidget->clearItems();

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		if(top_selected == 2) // search
		{
			loadTMDBPlaylist("transformers", "", page, true);
		}
		else
			loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
			
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		rightWidget->setSelected(0);

		return RETURN_REPAINT;
	}
	else if(actionKey == "prevPage")
	{
		top_selected = frameBoxWidget->getSelected();

		page--;
		if(page <= 1)
			page = 1;
		right_selected = 0;
		rightWidget->clearItems();

		CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
		loadBox.paint();
		
		if(top_selected == 2) // search
		{
			loadTMDBPlaylist("transformers", "", page, true);
		}
		else
			loadTMDBPlaylist(TVShows.c_str(), plist.c_str(), page);
			
		loadBox.hide();

		// load items
		for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
		{
			item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

			item->setOption(m_vMovieInfo[i].epgChannel.c_str());

			item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

			item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

			item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

			item->set2lines();

			std::string tmp = m_vMovieInfo[i].epgTitle;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo1;
			tmp += "\n";
			tmp += m_vMovieInfo[i].epgInfo2;

			item->setHint(tmp.c_str());

			rightWidget->addItem(item);
		}

		rightWidget->setSelected(0);

		return RETURN_REPAINT;
	}
	else if(actionKey == "search")
	{
		leftWidget->clearItems();

		tmdbsearch.clear();

		CStringInputSMS stringInput(LOCALE_EVENTFINDER_SEARCH, tmdbsearch.c_str());
		stringInput.exec(NULL, "");

		if(!stringInput.getExitPressed())
		{
			rightWidget->clearItems();

			CHintBox loadBox("CWidget", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
			loadBox.paint();
			loadTMDBPlaylist(tmdbsearch.c_str(), "", 1, true);
			loadBox.hide();

			// load items
			for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
			{
				item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "yplay");

				item->setOption(m_vMovieInfo[i].epgChannel.c_str());

				item->setInfo1(m_vMovieInfo[i].epgInfo1.c_str());

				item->setInfo2(m_vMovieInfo[i].epgInfo2.c_str());

				item->setItemIcon(file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

				item->set2lines();

				std::string tmp = m_vMovieInfo[i].epgTitle;
				tmp += "\n";
				tmp += m_vMovieInfo[i].epgInfo1;
				tmp += "\n";
				tmp += m_vMovieInfo[i].epgInfo2;

				item->setHint(tmp.c_str());

				rightWidget->addItem(item);
			}

			rightWidget->setSelected(0);

			tmdbsearch.clear();
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "multiwidget")
	{
		testMultiWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "txtinfo")
	{
		if(textWidget)
			textWidget->setBigFonts();

		return RETURN_REPAINT;
	}
	else if(actionKey == "singleWidget")
	{
		testSingleWidget();

		return RETURN_REPAINT;
	}
	else if(actionKey == "firetv")
	{
		testFireTV();

		return RETURN_REPAINT;
	}
	else if(actionKey == "home")
	{
		return RETURN_REPAINT;
	}
	else if(actionKey == "setup")
	{
		CAudioPlayerSettings * audioPlayerSettingsMenu = new CAudioPlayerSettings();

		audioPlayerSettingsMenu->exec(this, "");
		delete audioPlayerSettingsMenu;
		audioPlayerSettingsMenu = NULL;	

		return RETURN_REPAINT;
	}
	else if(actionKey == "help")
	{
		testCHelpBox();

		return RETURN_REPAINT;
	}
	else if(actionKey == "fireplay")
	{
		if (frameBoxWidget)
			selected = frameBoxWidget->getSelected() - 6;
		else
			selected = 0;

		if (&m_vMovieInfo[selected].file != NULL) 
		{
			CMovieInfoWidget movieInfoWidget;
			movieInfoWidget.setMovie(m_vMovieInfo[selected]);
		
			movieInfoWidget.exec(NULL, "");
		}

		return RETURN_REPAINT;
	}
	else if(actionKey == "fire1play")
	{
		selected = 0;

		if (&m_vMovieInfo[selected].file != NULL) 
		{
			CMovieInfoWidget movieInfoWidget;
			movieInfoWidget.setMovie(m_vMovieInfo[selected]);
		
			movieInfoWidget.exec(NULL, "");
		}

		return RETURN_REPAINT;
	}

	showMenu();
	
	return RETURN_REPAINT;
}

#define MESSAGEBOX_NO_YES_OPTION_COUNT 2
const keyval MESSAGEBOX_NO_YES_OPTIONS[MESSAGEBOX_NO_YES_OPTION_COUNT] =
{
	{ 0, LOCALE_MESSAGEBOX_NO, NULL },
	{ 1, LOCALE_MESSAGEBOX_YES, NULL }
};

void CTestMenu::showMenu()
{
	dprintf(DEBUG_NORMAL, "CTestMenu::showMenu:\n");

	CMenuWidget * mainMenu = new CMenuWidget();

	mainMenu->setTitle("testMenu", NEUTRINO_ICON_BUTTON_SETUP);

	mainMenu->setWidgetMode(MODE_MENU);
	mainMenu->enableShrinkMenu(),
	mainMenu->enableMenuPosition();
	mainMenu->enablePaintFootInfo();
	
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "CWidget"));
	mainMenu->addItem(new CMenuForwarder("CWidget(ClistBox|CFrameBox)", true, NULL, this, "widget"));
	mainMenu->addItem(new CMenuForwarder("CWidget(listFrame)", true, NULL, this, "listframewidget"));
	mainMenu->addItem(new CMenuForwarder("CWidget(textBox)", true, NULL, this, "textboxwidget"));
	mainMenu->addItem(new CMenuForwarder("CWidget(multiWidget)", true, NULL, this, "multiwidget"));

	//
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "menu CMenuItems"));
	mainMenu->addItem(new CMenuForwarder("CMenuForwarder", true, NULL, this, "menuforwarder"));

	//
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "list CMenuItems"));
	mainMenu->addItem(new ClistBoxItem("listBoxItem", true, NULL, this, "listboxitem"));

	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "setup CMenuItems"));
	mainMenu->addItem(new CMenuOptionChooser("CMenuOptionChooser:", &selected, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true));

	mainMenu->addItem(new CMenuOptionNumberChooser("CMenuOptionNumberChooser:", &selected, true, 0, 100));
	
	//
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "widget Items Helpers"));
	mainMenu->addItem(new CMenuForwarder("CBox", true, NULL, this, "box"));
	mainMenu->addItem(new CMenuForwarder("CIcon", true, NULL, this, "icon"));
	mainMenu->addItem(new CMenuForwarder("CImage", true, NULL, this, "image"));
	mainMenu->addItem(new CMenuForwarder("CButtons", true, NULL, this, "buttons"));
	mainMenu->addItem(new CMenuForwarder("CProgressBar", true, NULL, this, "progressbar"));
	//mainMenu->addItem(new CMenuForwarder("CScrollBar", true, NULL, this, "scrollbar"));
	//mainMenu->addItem(new CMenuForwarder("CItems2DetailsLine", true, NULL, this, "detailsline"));
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "CWindow|CPIG|CGrid"));
	mainMenu->addItem(new CMenuForwarder("CWindow", true, NULL, this, "window"));
	mainMenu->addItem(new CMenuForwarder("CWindow(with shadow)", true, NULL, this, "windowshadow"));
	mainMenu->addItem(new CMenuForwarder("CWindow(customColor)", true, NULL, this, "windowcustomcolor"));
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "CProgressWindow"));
	mainMenu->addItem(new CMenuForwarder("CProgressWindow", true, NULL, this, "progresswindow"));
	
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "Widget Items"));
	mainMenu->addItem(new CMenuForwarder("CHeaders", true, NULL, this, "headers"));
	mainMenu->addItem(new CMenuForwarder("CTextBox", true, NULL, this, "textbox"));
	mainMenu->addItem(new CMenuForwarder("CListFrame", true, NULL, this, "listframe"));
	
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "WidgetItem (ClistBox)"));
	mainMenu->addItem(new CMenuForwarder("ClistBox(standard)", true, NULL, this, "listbox"));
	mainMenu->addItem(new CMenuForwarder("ClistBox(classic)", true, NULL, this, "listbox2"));
	mainMenu->addItem(new CMenuForwarder("ClistBox(extended)", true, NULL, this, "listbox3"));
	mainMenu->addItem(new CMenuForwarder("ClistBox(Frame)", true, NULL, this, "listbox4"));
	mainMenu->addItem(new CMenuForwarder("ClistBox(menu mode)", true, NULL, this, "listbox5"));
	mainMenu->addItem(new CMenuForwarder("ClistBox(listBox mode)", true, NULL, this, "listbox6"));
	
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "ClistBox (standalonbe)"));
	mainMenu->addItem(new CMenuForwarder("ClistBox", true, NULL, this, "listboxmwidget"));
	
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "CFrameBox"));
	mainMenu->addItem(new CMenuForwarder("CFrameBox", true, NULL, this, "framebox"));
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "CFrameBox(standalone)"));
	mainMenu->addItem(new CMenuForwarder("CFrameBox2", true, NULL, this, "singleWidget"));
	mainMenu->addItem(new CMenuForwarder("CFrameBox(Fire TV)", true, NULL, this, "firetv"));
	
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "CFrameBox|ClistBpx"));
	mainMenu->addItem(new CMenuForwarder("CFrameBox|ClistBox", true, NULL, this, "testing"));
	
	// CMenuWidhet
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "CMenuWidget"));
	mainMenu->addItem(new CMenuForwarder("CMenuWidget(MODE_LISTBOX)", true, NULL, this, "listboxwidget"));
	mainMenu->addItem(new CMenuForwarder("CMenuWidget(MODE_MENU)", true, NULL, this, "listboxwidget1"));
	mainMenu->addItem(new CMenuForwarder("CMenuWidget(MODE_SETUP)", true, NULL, this, "listboxwidget2"));

	// other widget
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "other Widget"));
	mainMenu->addItem(new CMenuForwarder("CStringInput", true, NULL, this, "stringinput"));
	mainMenu->addItem(new CMenuForwarder("CStringInputSMS", true, NULL, this, "stringinputsms"));
	mainMenu->addItem(new CMenuForwarder("CPINInput", true, NULL, this, "pininput"));
	mainMenu->addItem(new CMenuForwarder("CPLPINInput", true, NULL, this, "plpininput"));
	mainMenu->addItem(new CMenuForwarder("CPINChangeWidget", true, NULL, this, "pinchangewidget"));
	mainMenu->addItem(new CMenuForwarder("CIPInput", true, NULL, this, "ipinput"));
	mainMenu->addItem(new CMenuForwarder("CMACInput", true, NULL, this, "macinput"));
	//mainMenu->addItem(new CMenuForwarder("CDateInput", true, NULL, this, "dateinput"));
	//mainMenu->addItem(new CMenuForwarder("CTimeInput", true, NULL, this, "timeinput"));
	//mainMenu->addItem(new CMenuForwarder("CIntInput", true, NULL, this, "intinput"));
	mainMenu->addItem(new CMenuForwarder("CInfoBox", true, NULL, this, "infobox"));
	mainMenu->addItem(new CMenuForwarder("CMessageBox", true, NULL, this, "messagebox"));
	mainMenu->addItem(new CMenuForwarder("CMessageBoxInfoMsg", true, NULL, this, "messageboxinfomsg"));
	mainMenu->addItem(new CMenuForwarder("CMessageBoxErrorMsg", true, NULL, this, "messageboxerrormsg"));
	mainMenu->addItem(new CMenuForwarder("CHintBox", true, NULL, this, "hintbox"));
	mainMenu->addItem(new CMenuForwarder("CHintBoxInfo", true, NULL, this, "hintboxinfo"));
	mainMenu->addItem(new CMenuForwarder("CHelpBox", true, NULL, this, "helpbox"));
	//mainMenu->addItem(new CMenuForwarder("CProgressWindow", true, NULL, this, "progresswindow"));

	//
	mainMenu->addItem(new CMenuSeparator(LINE));
	mainMenu->addItem(new CMenuForwarder("ColorChooser", true, NULL, this, "colorchooser"));
	mainMenu->addItem(new CMenuForwarder("KeyChooser", true, NULL, this, "keychooser"));
	mainMenu->addItem(new CMenuForwarder("VFDController", true, NULL, this, "vfdcontroller"));
	mainMenu->addItem(new CMenuForwarder("MountChooser", true, NULL, this, "mountchooser"));
	
	//
	mainMenu->addItem(new CMenuSeparator(LINE));
	mainMenu->addItem(new CMenuForwarder("ChannelSelectWidget", true, NULL, this, "channelselect"));
	mainMenu->addItem(new CMenuForwarder("BEWidget", true, NULL, this, "bewidget"));
	mainMenu->addItem(new CMenuForwarder("AudioVideoSelectWidget", true, NULL, this, "avselect"));
	mainMenu->addItem(new CMenuForwarder("AudioSelectWidget", true, NULL, this, "aselect"));
	mainMenu->addItem(new CMenuForwarder("DVBSubSelectWidget", true, NULL, this, "dvbsubselect"));
	mainMenu->addItem(new CMenuForwarder("AlphaSetup", true, NULL, this, "alphasetup"));
	mainMenu->addItem(new CMenuForwarder("PSISetup", true, NULL, this, "psisetup"));
	mainMenu->addItem(new CMenuForwarder("RCLock", true, NULL, this, "rclock"));
	mainMenu->addItem(new CMenuForwarder("SleepTimerWidget", true, NULL, this, "sleeptimer"));
	mainMenu->addItem(new CMenuForwarder("MountGUI", true, NULL, this, "mountgui"));
	mainMenu->addItem(new CMenuForwarder("UmountGUI", true, NULL, this, "umountgui"));
	mainMenu->addItem(new CMenuForwarder("MountSmallMenu", true, NULL, this, "mountsmallmenu"));
	mainMenu->addItem(new CMenuForwarder("PluginsList", true, NULL, this, "pluginslist"));

	//
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "Players"));
	mainMenu->addItem(new CMenuForwarder("PlayMovieURL", true, NULL, this, "playmovieurl"));
	mainMenu->addItem(new CMenuForwarder("PlayAudioURL", true, NULL, this, "playaudiourl"));
	mainMenu->addItem(new CMenuForwarder("ShowPictureURL", true, NULL, this, "showpictureurl"));

	//
	mainMenu->addItem(new CMenuSeparator(LINE));
	mainMenu->addItem(new CMenuForwarder("PlayMovieFolder", true, NULL, this, "playmoviefolder"));
	mainMenu->addItem(new CMenuForwarder("PlayAudioFolder", true, NULL, this, "playaudiofolder"));
	mainMenu->addItem(new CMenuForwarder("ShowPictureFolder", true, NULL, this, "showpicturefolder"));

	//
	mainMenu->addItem(new CMenuSeparator(LINE));
	mainMenu->addItem(new CMenuForwarder("PlayMovieDir(without Browser)", true, NULL, this, "playmoviedir"));
	mainMenu->addItem(new CMenuForwarder("PlayAudioDir(without Browser)", true, NULL, this, "playaudiodir"));
	mainMenu->addItem(new CMenuForwarder("ShowPictureDir(without Browser)", true, NULL, this, "showpicturedir"));

	//
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "Plugins") );
	mainMenu->addItem(new CMenuForwarder("StartPlugin(e.g: youtube)", true, NULL, this, "startplugin"));

	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "EPG") );
	mainMenu->addItem(new CMenuForwarder("ShowActuellEPG", true, NULL, this, "showepg"));

	//
	mainMenu->addItem(new CMenuSeparator(LINE | STRING, "Channellist") );
	mainMenu->addItem(new CMenuForwarder("CChannelList:", true, NULL, this, "channellist"));
	mainMenu->addItem(new CMenuForwarder("CBouquetList:", true, NULL, this, "bouquetlist"));
	
	mainMenu->exec(NULL, "");
	//mainMenu->hide();

	selected = mainMenu->getSelected();
	printf("\nCTestMenu::exec: select:%d\n", selected);

	delete mainMenu;
	mainMenu = NULL;
}

void plugin_init(void)
{
	dprintf(DEBUG_NORMAL, "test: plugin_init\n");
}

void plugin_del(void)
{
	dprintf(DEBUG_NORMAL, "test: plugin_del\n");
}

void plugin_exec(void)
{
	CTestMenu *testMenu = new CTestMenu();
	
	testMenu->exec(NULL, "");
	
	delete testMenu;
	testMenu = NULL;
}


