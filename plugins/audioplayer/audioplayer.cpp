/*
  $Id: audioplayer.cpp 2018/07/10 mohousch Exp $

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
#include <fstream>


extern "C" void plugin_exec(void);
extern "C" void plugin_init(void);
extern "C" void plugin_del(void);

class CMP3Player : public CMenuTarget
{
	private:
		CFrameBuffer* frameBuffer;

		CMenuWidget* alist;
		CMenuItem* item;

		CAudioPlayerGui tmpAudioPlayerGui;
		CFileFilter fileFilter;
		CFileList filelist;
		CAudioPlayList playlist;
		std::string Path;
		int selected;

		void loadPlaylist();
		void openFileBrowser();

		//
		bool shufflePlaylist(void);

		void showMenu();
		
	public:
		CMP3Player();
		~CMP3Player();
		int exec(CMenuTarget* parent, const std::string& actionKey);
		void hide();
};

CMP3Player::CMP3Player()
{
	frameBuffer = CFrameBuffer::getInstance();

	alist = NULL;
	item = NULL;

	selected = 0;

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

	CAudioPlayer::getInstance()->init();
}

CMP3Player::~CMP3Player()
{
	playlist.clear();

	if(CAudioPlayer::getInstance()->getState() != CBaseDec::STOP)
		CAudioPlayer::getInstance()->stop();
}

void CMP3Player::hide()
{
	frameBuffer->paintBackground();
	frameBuffer->blit();
}

bool CMP3Player::shufflePlaylist(void)
{
	dprintf(DEBUG_NORMAL, "CMP3Player::shufflePlaylist\n");
	
	RandomNumber rnd;
	bool result = false;
	
	if (!(playlist.empty()))
	{
		if (selected > 0)
		{
			std::swap(playlist[0], playlist[selected]);
			selected = 0;
		}

		std::random_shuffle((selected != 0) ? playlist.begin() : playlist.begin() + 1, playlist.end(), rnd);

		selected = 0;

		result = true;
	}
	
	return(result);
}

void CMP3Player::loadPlaylist()
{
	playlist.clear();

	Path = g_settings.network_nfs_audioplayerdir;

	//if(CFileHelpers::getInstance()->/*readDir*/addRecursiveDir(Path, &filelist, &fileFilter))
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
				
				playlist.push_back(audiofile);
			}
		}
	}
}

void CMP3Player::openFileBrowser()
{
	CFileBrowser filebrowser((g_settings.filebrowser_denydirectoryleave) ? g_settings.network_nfs_picturedir : "");

	filebrowser.Multi_Select = true;
	filebrowser.Dirs_Selectable = true;
	filebrowser.Filter = &fileFilter;

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
				for (unsigned long i = 0; i < (unsigned long)playlist.size(); i++)
				{
					if(playlist[i].Filename == audiofile.Filename)
						playlist.erase(playlist.begin() + i); 
				}

				CAudioPlayer::getInstance()->init();

				int ret = CAudioPlayer::getInstance()->readMetaData(&audiofile, true);

				if (!ret || (audiofile.MetaData.artist.empty() && audiofile.MetaData.title.empty() ))
				{
					//remove extension (.mp3)
					std::string tmp = files->getFileName().substr(files->getFileName().rfind('/') + 1);
					tmp = tmp.substr(0, tmp.length() - 4);

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
		
				playlist.push_back(audiofile);
			}
		}
	}
}

#define HEAD_BUTTONS_COUNT 1
const struct button_label HeadButtons[HEAD_BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_SETUP, "" }
};

#define FOOT_BUTTONS_COUNT 4
const struct button_label AudioPlayerButtons[FOOT_BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_RED, _("Delete") },
	{ NEUTRINO_ICON_BUTTON_GREEN, _("Add") },
	{ NEUTRINO_ICON_BUTTON_YELLOW, _("Delete all") },
	{ NEUTRINO_ICON_BUTTON_BLUE, _("Shuffle") }
};

void CMP3Player::showMenu()
{	
	alist = new CMenuWidget(_("Audio Player"), NEUTRINO_ICON_MP3, frameBuffer->getScreenWidth() - 40, frameBuffer->getScreenHeight() - 40);

	for(unsigned int i = 0; i < (unsigned int)playlist.size(); i++)
	{
		std::string title;
		std::string artist;
		std::string genre;
		std::string date;
		char duration[9] = "";

		title = playlist[i].MetaData.title;
		artist = playlist[i].MetaData.artist;
		genre = playlist[i].MetaData.genre;	
		date = playlist[i].MetaData.date;
		std::string cover = playlist[i].MetaData.cover.empty()? DATADIR "/neutrino/icons/no_coverArt.png" : playlist[i].MetaData.cover;

		snprintf(duration, 8, "(%ld:%02ld)", playlist[i].MetaData.total_time / 60, playlist[i].MetaData.total_time % 60);

		//
		item = new ClistBoxItem(title.c_str(), true, artist.c_str(), this, "aplay");
			
		item->setOptionInfo(duration);
		item->setNumber(i + 1);

		// details Box
		item->setInfo1(genre.c_str());
		//item->setOptionInfo1(genre.c_str());
		item->setInfo2(date.c_str());
		//item->setOptionInfo2(date.c_str());

		/*
		std::string tmp = title.c_str();
		tmp += "\n";
		tmp += genre.c_str();
		tmp += "\n";
		tmp += artist.c_str();
		tmp += "\n";
		tmp += date.c_str();

		item->setHint(tmp.c_str());
		*/
		
		item->setHintIcon(cover.c_str());
		
		item->set2lines();
		item->enableItemShadow();

		alist->addItem(item);
	}
	
	alist->setWidgetMode(MODE_LISTBOX);
	alist->setWidgetType(WIDGET_TYPE_CLASSIC);
	alist->setItemsPerPage(10, 6);

	alist->setSelected(selected);

	alist->enablePaintDate();
	alist->setHeadButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	
	alist->setFootButtons(AudioPlayerButtons, FOOT_BUTTONS_COUNT);

	alist->addKey(RC_setup, this, CRCInput::getSpecialKeyName(RC_setup));
	alist->addKey(RC_red, this, CRCInput::getSpecialKeyName(RC_red));
	alist->addKey(RC_green, this, CRCInput::getSpecialKeyName(RC_green));
	alist->addKey(RC_yellow, this, CRCInput::getSpecialKeyName(RC_yellow));
	alist->addKey(RC_blue, this, CRCInput::getSpecialKeyName(RC_blue));

	alist->exec(NULL, "");
	delete alist;
	alist = NULL;
}

int CMP3Player::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CMP3Player::exec: actionKey:%s\n", actionKey.c_str());
	
	if(parent)
		hide();

	if(alist != NULL)
		selected = alist->getSelected();

	if(actionKey == "aplay")
	{
		for (unsigned int i = 0; i < (unsigned int)playlist.size(); i++)
		{
			tmpAudioPlayerGui.addToPlaylist(playlist[i]);
		}

		tmpAudioPlayerGui.setCurrent(selected);
		tmpAudioPlayerGui.exec(NULL, "");

		return RETURN_REPAINT;
	}
	else if(actionKey == "RC_setup")
	{
		CAudioPlayerSettings * audioPlayerSettingsMenu = new CAudioPlayerSettings();
		audioPlayerSettingsMenu->exec(this, "");
		delete audioPlayerSettingsMenu;
		audioPlayerSettingsMenu = NULL;	

		return RETURN_REPAINT;					
	}
	else if(actionKey == "RC_red")
	{
		CAudioPlayList::iterator p = playlist.begin() + alist->getSelected();
		playlist.erase(p);

		if (selected >= (int)playlist.size())
			selected = playlist.size() - 1;

		showMenu();

		return RETURN_EXIT_ALL;
	}
	else if(actionKey == "RC_green")
	{
		openFileBrowser();
		showMenu();

		return RETURN_EXIT_ALL;
	}
	else if(actionKey == "RC_yellow")
	{
		playlist.clear();
		selected = 0;
		showMenu();

		return RETURN_EXIT_ALL;
	}
	else if(actionKey == "RC_blue")
	{
		shufflePlaylist();
		showMenu();

		return RETURN_EXIT_ALL;
	}

	//
	loadPlaylist();
	showMenu();
	
	return RETURN_EXIT_ALL;
}

void plugin_init(void)
{
}

void plugin_del(void)
{
}

void plugin_exec(void)
{
	CMP3Player* audioPlayerHandler = new CMP3Player();
	
	audioPlayerHandler->exec(NULL, "");
	
	delete audioPlayerHandler;
	audioPlayerHandler = NULL;
}


