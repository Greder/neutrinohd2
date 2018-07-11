/*
  $Id: icecast.cpp 2018/07/10 mohousch Exp $

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

#include <algorithm>
#include <sys/time.h>
#include <fstream>
#include <iostream>

# include <plugin.h>


#define NEUTRINO_ICON_ICECAST_SMALL		PLUGINDIR "/icecast/icecast_small.png"

#define SHOW_FILE_LOAD_LIMIT 50
#define AUDIOPLAYER_CHECK_FOR_DUPLICATES

const long int GET_PLAYLIST_TIMEOUT = 10;

//
std::string icecasturl = "http://dir.xiph.org/yp.xml";
const long int GET_ICECAST_TIMEOUT = 90; 		// list is about 500kB!

//
extern "C" void plugin_exec(void);
extern "C" void plugin_init(void);
extern "C" void plugin_del(void);

class CIceCast : public CMenuTarget
{
	private:
		CFrameBuffer* frameBuffer;

		ClistBox* ilist;
		CMenuItem* item;

		CAudioPlayerGui tmpAudioPlayerGui;
		CFileFilter fileFilter;
		CFileList filelist;
		CAudioPlayList playlist;
		std::string m_Path;
		int selected;

		//
		bool shufflePlaylist(void);

		//
		void addUrl2Playlist(const char *url, const char *name = NULL, const time_t bitrate = 0);
		void processPlaylistUrl(const char *url, const char *name = NULL, const time_t bitrate = 0);
		void scanXmlFile(std::string filename);
		void scanXmlData(xmlDocPtr answer_parser, const char *nametag, const char *urltag, const char *bitratetag = NULL, bool usechild = false);

		bool openFileBrowser(void);

		//
		void GetMetaData(CAudiofileExt &File);
		void getFileInfoToDisplay(std::string& fileInfo, CAudiofileExt &file);
		
	public:
		CIceCast();
		~CIceCast();
		int exec(CMenuTarget* parent, const std::string& actionKey);
		void hide();
		void showMenu(bool reload = true);
};

CIceCast::CIceCast()
{
	frameBuffer = CFrameBuffer::getInstance();

	ilist = NULL;
	item = NULL;

	selected = 0;


	fileFilter.addFilter("url");
	fileFilter.addFilter("xml");
	fileFilter.addFilter("m3u");
	fileFilter.addFilter("pls");

	CAudioPlayer::getInstance()->init();
}

CIceCast::~CIceCast()
{
	playlist.clear();
}

void CIceCast::hide()
{
	frameBuffer->paintBackground();
	frameBuffer->blit();
}

bool CIceCast::shufflePlaylist(void)
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

void CIceCast::addUrl2Playlist(const char *url, const char *name, const time_t bitrate) 
{
	dprintf(DEBUG_NORMAL, "CIceCast::addUrl2Playlist: name = %s, url = %s\n", name, url);
	
	CAudiofileExt mp3(url, CFile::EXTENSION_URL);
	
	if (name != NULL) 
	{
		mp3.MetaData.title = name;
	} 
	else 
	{
		std::string tmp = mp3.Filename.substr(mp3.Filename.rfind('/') + 1);
		mp3.MetaData.title = tmp;
	}
	
	if (bitrate)
		mp3.MetaData.total_time = bitrate;
	else
		mp3.MetaData.total_time = 0;

	if (url[0] != '#') 
	{
		playlist.push_back(mp3);
	}
}

void CIceCast::processPlaylistUrl(const char *url, const char *name, const time_t tim) 
{
	dprintf(DEBUG_NORMAL, "CIceCast::processPlaylistUrl\n");
	
	CURL *curl_handle;
	struct MemoryStruct chunk;
	
	chunk.memory = NULL; 	// we expect realloc(NULL, size) to work
	chunk.size = 0;    	// no data at this point

	curl_global_init(CURL_GLOBAL_ALL);

	// init the curl session
	curl_handle = curl_easy_init();

	// specify URL to get
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);

	// send all data to this function
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	// we pass our 'chunk' struct to the callback function
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

	// some servers don't like requests that are made without a user-agent field, so we provide one
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	// don't use signal for timeout
	curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, (long)1);

	// set timeout to 10 seconds
	curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, GET_PLAYLIST_TIMEOUT);
	
	if(strcmp(g_settings.softupdate_proxyserver, "") != 0)
	{
		curl_easy_setopt(curl_handle, CURLOPT_PROXY, g_settings.softupdate_proxyserver);
		
		if(strcmp(g_settings.softupdate_proxyusername, "") != 0)
		{
			char tmp[200];
			strcpy(tmp, g_settings.softupdate_proxyusername);
			strcat(tmp, ":");
			strcat(tmp, g_settings.softupdate_proxypassword);
			curl_easy_setopt(curl_handle, CURLOPT_PROXYUSERPWD, tmp);
		}
	}

	// get it! 
	curl_easy_perform(curl_handle);

	// cleanup curl stuff
	curl_easy_cleanup(curl_handle);

	long res_code;
	if (curl_easy_getinfo(curl_handle, CURLINFO_HTTP_CODE, &res_code ) ==  CURLE_OK) 
	{
		if (200 == res_code) 
		{
			//printf("\nchunk = %s\n", chunk.memory);
			std::istringstream iss;
			iss.str (std::string(chunk.memory, chunk.size));
			char line[512];
			char *ptr;
			
			while (iss.rdstate() == std::ifstream::goodbit) 
			{
				iss.getline(line, 512);
				if (line[0] != '#') 
				{
					//printf("chunk: line = %s\n", line);
					ptr = strstr(line, "http://");
					if (ptr != NULL) 
					{
						char *tmp;
						// strip \n and \r characters from url
						tmp = strchr(line, '\r');
						if (tmp != NULL)
							*tmp = '\0';
						tmp = strchr(line, '\n');
						if (tmp != NULL)
							*tmp = '\0';
						
						addUrl2Playlist(ptr, name, tim);
					}
				}
			}
		}
	}

	if(chunk.memory)
		free(chunk.memory);
 
	// we're done with libcurl, so clean it up
	curl_global_cleanup();
}

void CIceCast::scanXmlFile(std::string filename)
{
	dprintf(DEBUG_NORMAL, "CIceCast::scanXmlFile: %s\n", filename.c_str());

	xmlDocPtr answer_parser = parseXmlFile(filename.c_str());

	scanXmlData(answer_parser, "name", "url");
}

void CIceCast::scanXmlData(xmlDocPtr answer_parser, const char *nametag, const char *urltag, const char *bitratetag, bool usechild)
{
	dprintf(DEBUG_NORMAL, "CIceCast::scanXmlData\n");
	
#define IC_typetag "server_type"

	if (answer_parser != NULL) 
	{
		xmlNodePtr element = xmlDocGetRootElement(answer_parser);
		element = element->xmlChildrenNode;
		xmlNodePtr element_tmp = element;
		
		if (element == NULL) 
		{
			dprintf(DEBUG_NORMAL, "CIceCast::scanXmlData: No valid XML File.\n");
		} 
		else 
		{
			CProgressWindow progress;
			long maxProgress = 1;
			
			// count # of entries
			while (element) 
			{
				maxProgress++;
				element = element->xmlNextNode;
			}
			
			element = element_tmp;
			long listPos = -1;
			
			progress.setTitle(LOCALE_AUDIOPLAYER_READING_FILES);
			progress.exec(this, "");
			
			neutrino_msg_t      msg;
			neutrino_msg_data_t data;
			
			g_RCInput->getMsg(&msg, &data, 0);
			
			while (element && msg != CRCInput::RC_home) 
			{
				char *ptr = NULL;
				char *name = NULL;
				char *url = NULL;
				char *type = NULL;
				time_t bitrate = 0;
				bool skip = true;
				listPos++;
				
				// show status
				int global = 100*listPos / maxProgress;
				progress.showGlobalStatus(global);

				if (usechild) 
				{
					xmlNodePtr child = element->xmlChildrenNode;
					while (child) 
					{
						if (strcmp(xmlGetName(child), nametag) == 0)
							name = xmlGetData(child);
						else if (strcmp(xmlGetName(child), urltag) == 0)
							url = xmlGetData(child);
						else if (strcmp(xmlGetName(child), IC_typetag) == 0)
							type = xmlGetData(child);
						else if (bitratetag && strcmp(xmlGetName(child), bitratetag) == 0) {
							ptr = xmlGetData(child);
							if (ptr) 
								bitrate = atoi(ptr);
						}
						child = child->xmlNextNode;
					}
					if 	(strcmp("audio/mpeg", type) == 0) 	skip = false;
					else if (strcmp("mp3", type) == 0) 		skip = false;
					else if (strcmp("application/mp3", type) == 0) 	skip = false;
				} 
				else 
				{
					url = xmlGetAttribute(element, (char *) urltag);
					name = xmlGetAttribute(element, (char *) nametag);
					
					if (bitratetag) 
					{
						ptr = xmlGetAttribute(element, (char *) bitratetag);
						if (ptr)
							bitrate = atoi(ptr);
					}
					skip = false;
				}

				if ((url != NULL) && !skip) 
				{
					progress.showStatusMessageUTF(url);
					
					if (strstr(url, ".m3u") || strstr(url, ".pls"))
						processPlaylistUrl(url, name);
					else 
						addUrl2Playlist(url, name, bitrate);
				}
				element = element->xmlNextNode;
				g_RCInput->getMsg(&msg, &data, 0);

			}
			progress.hide();
		}
		xmlFreeDoc(answer_parser);
	}
}

bool CIceCast::openFileBrowser(void)
{
	dprintf(DEBUG_INFO, "CIceCast::openFilebrowser\n");
	
	bool result = false;
	CFileBrowser filebrowser((g_settings.filebrowser_denydirectoryleave) ? g_settings.network_nfs_audioplayerdir : "");

	filebrowser.Multi_Select = true;
	filebrowser.Dirs_Selectable = true;
	filebrowser.Filter = &fileFilter;

	m_Path = g_settings.network_nfs_audioplayerdir;

	hide();

	if (filebrowser.exec(m_Path.c_str()))
	{
		CProgressWindow progress;
		long maxProgress = (filebrowser.getSelectedFiles().size() > 1) ? filebrowser.getSelectedFiles().size() - 1 : 1;
		long currentProgress = -1;
		
		if (maxProgress > SHOW_FILE_LOAD_LIMIT)
		{
			progress.setTitle(LOCALE_AUDIOPLAYER_READING_FILES);
			progress.exec(this, "");	
		}

		m_Path = filebrowser.getCurrentDir();
		CFileList::const_iterator files = filebrowser.getSelectedFiles().begin();
		
		//
		for(; files != filebrowser.getSelectedFiles().end(); files++)
		{
			if (maxProgress > SHOW_FILE_LOAD_LIMIT)
			{
				currentProgress++;
				// show status
				int global = 100*currentProgress/maxProgress;
				progress.showGlobalStatus(global);
				progress.showStatusMessageUTF(files->Name);
			}
			
			//cdr/mp3/wav/flac
			if ( (files->getExtension() == CFile::EXTENSION_CDR)
					||  (files->getExtension() == CFile::EXTENSION_MP3)
					||  (files->getExtension() == CFile::EXTENSION_WAV)
					||  (files->getExtension() == CFile::EXTENSION_FLAC)
			)
			{
				CAudiofileExt audiofile(files->Name, files->getExtension());
				playlist.push_back(audiofile);
			}
			
			//url
			if(files->getType() == CFile::FILE_URL)
			{
				std::string filename = files->Name;
				FILE *fd = fopen(filename.c_str(), "r");
				
				if(fd)
				{
					char buf[512];
					unsigned int len = fread(buf, sizeof(char), 512, fd);
					fclose(fd);

					if (len && (strstr(buf, ".m3u") || strstr(buf, ".pls")))
					{
						dprintf(DEBUG_INFO, "CIceCast::openFilebrowser: m3u/pls Playlist found: %s\n", buf);
						
						filename = buf;
						processPlaylistUrl(files->Name.c_str());
					}
					else
					{
						addUrl2Playlist(filename.c_str());
					}
				}
			}
			else if(files->getType() == CFile::FILE_PLAYLIST)
			{
				std::string sPath = files->Name.substr(0, files->Name.rfind('/'));
				std::ifstream infile;
				char cLine[1024];
				char name[1024] = { 0 };
				int duration;
				
				infile.open(files->Name.c_str(), std::ifstream::in);

				while (infile.good())
				{
					infile.getline(cLine, sizeof(cLine));
					
					// remove CR
					if(cLine[strlen(cLine) - 1] == '\r')
						cLine[strlen(cLine) - 1] = 0;
					
					sscanf(cLine, "#EXTINF:%d,%[^\n]\n", &duration, name);
					
					if(strlen(cLine) > 0 && cLine[0] != '#')
					{
						char *url = strstr(cLine, "http://");
						if (url != NULL) 
						{
							if (strstr(url, ".m3u") || strstr(url, ".pls"))
								processPlaylistUrl(url);
							else
								addUrl2Playlist(url, name, duration);
						} 
						else if ((url = strstr(cLine, "icy://")) != NULL) 
						{
							addUrl2Playlist(url);
						} 
						else if ((url = strstr(cLine, "scast:://")) != NULL) 
						{
							addUrl2Playlist(url);
						}
						else
						{
							std::string filename = sPath + '/' + cLine;

							std::string::size_type pos;
							while((pos = filename.find('\\')) != std::string::npos)
								filename[pos] = '/';

							std::ifstream testfile;
							testfile.open(filename.c_str(), std::ifstream::in);
							
							if(testfile.good())
							{
#ifdef AUDIOPLAYER_CHECK_FOR_DUPLICATES
								// Check for duplicates and remove (new entry has higher prio)
								// this really needs some time :(
								for (unsigned long i = 0; i < playlist.size(); i++)
								{
									if(playlist[i].Filename == filename)
										//removeFromPlaylist(i);
										playlist.erase(playlist.begin() + i); 
								}
#endif
								if(strcasecmp(filename.substr(filename.length() - 3, 3).c_str(), "url") == 0)
								{
									addUrl2Playlist(filename.c_str());
								}
								else
								{
									CFile playlistItem;
									playlistItem.Name = filename;
									CFile::FileExtension fileExtension = playlistItem.getExtension();
									
									if (fileExtension == CFile::EXTENSION_CDR
											|| fileExtension == CFile::EXTENSION_MP3
											|| fileExtension == CFile::EXTENSION_WAV
											|| fileExtension == CFile::EXTENSION_FLAC
									)
									{
										CAudiofileExt audioFile(filename, fileExtension);
										playlist.push_back(audioFile);
									} 
									else
									{
										dprintf(DEBUG_NORMAL, "CIceCast::openFilebrowser: file type (%d) is *not* supported in playlists\n(%s)\n", fileExtension, filename.c_str());
									}
								}
							}
							testfile.close();
						}
					}
				}
				infile.close();
			}
			else if(files->getType() == CFile::FILE_XML)
			{
				if (!files->Name.empty())
				{
					scanXmlFile(files->Name);
				}
			}
		}
		
		progress.hide();
		
		result = true;
	}

	return ( result);
}

void CIceCast::GetMetaData(CAudiofileExt &File)
{
	dprintf(DEBUG_DEBUG, "CIceCast::GetMetaData: fileExtension:%d\n", File.FileExtension);
	
	bool ret = 1;

	if (CFile::EXTENSION_URL != File.FileExtension)
		ret = CAudioPlayer::getInstance()->readMetaData(&File, /*m_state != CAudioPlayerGui::STOP && !g_settings.audioplayer_highprio*/true);

	if (!ret || (File.MetaData.artist.empty() && File.MetaData.title.empty() ))
	{
		//Set from Filename
		std::string tmp = File.Filename.substr(File.Filename.rfind('/') + 1);
		tmp = tmp.substr(0, tmp.length() - 4);	//remove extension (.mp3)
		std::string::size_type i = tmp.rfind(" - ");
		
		if(i != std::string::npos)
		{ 
			// Trennzeichen " - " gefunden
			File.MetaData.artist = tmp.substr(0, i);
			File.MetaData.title = tmp.substr(i + 3);
		}
		else
		{
			i = tmp.rfind('-');
			if(i != std::string::npos)
			{ //Trennzeichen "-"
				File.MetaData.artist = tmp.substr(0, i);
				File.MetaData.title = tmp.substr(i + 1);
			}
			else
				File.MetaData.title = tmp;
		}
		
		File.MetaData.artist = FILESYSTEM_ENCODING_TO_UTF8(std::string(File.MetaData.artist).c_str());
		File.MetaData.title  = FILESYSTEM_ENCODING_TO_UTF8(std::string(File.MetaData.title).c_str());
	}
}

void CIceCast::getFileInfoToDisplay(std::string &info, CAudiofileExt &file)
{
	std::string fileInfo;
	std::string artist;
	std::string title;

	if (!file.MetaData.bitrate)
	{
		GetMetaData(file);
	}

	if (!file.MetaData.artist.empty())
		artist = file.MetaData.artist;

	if (!file.MetaData.title.empty())
		title = file.MetaData.title;

	fileInfo += title;
	if (!title.empty() && !artist.empty()) 
		fileInfo += ", ";
		
	fileInfo += artist;

	if (!file.MetaData.album.empty())
	{
		fileInfo += " (";
		fileInfo += file.MetaData.album;
		fileInfo += ')';
	} 
	
	if (fileInfo.empty())
	{
		fileInfo += "Unknown";
	}
	
	file.firstChar = tolower(fileInfo[0]);
	info += fileInfo;
}

int CIceCast::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CIceCast::exec: actionKey:%s\n", actionKey.c_str());
	
	if(parent)
		hide();

	if(actionKey == "iplay")
	{
		selected = ilist->getSelected();

		tmpAudioPlayerGui.clearPlaylist();

		tmpAudioPlayerGui.addToPlaylist(playlist[selected]);

		tmpAudioPlayerGui.setCurrent(0);
		tmpAudioPlayerGui.setInetMode();
		tmpAudioPlayerGui.exec(NULL, "");
	}
	else if(actionKey == "RC_setup")
	{
		CAudioPlayerSettings * audioPlayerSettingsMenu = new CAudioPlayerSettings();
		audioPlayerSettingsMenu->exec(this, "");
		delete audioPlayerSettingsMenu;
		audioPlayerSettingsMenu = NULL;						
	}
	else if(actionKey == "RC_blue")
	{
		shufflePlaylist();
		showMenu();
		return menu_return::RETURN_EXIT_ALL;
	}
	else if(actionKey == "RC_green")
	{
		playlist.clear();

		openFileBrowser();

		showMenu(false);
		return menu_return::RETURN_EXIT_ALL;
	}
	else if(actionKey == "RC_yellow")
	{
		//tmpAudioPlayerGui.clearPlaylist();
		playlist.clear();
		showMenu(false);
		return menu_return::RETURN_EXIT_ALL;
	}
	else if(actionKey == "RC_red")
	{
		//tmpAudioPlayerGui.removeFromPlaylist(ilist->getSelected());
		CAudioPlayList::iterator p = playlist.begin() + ilist->getSelected();
		playlist.erase(p);

		if (selected >= playlist.size())
			selected = playlist.size() - 1;

		showMenu();
		return menu_return::RETURN_EXIT_ALL;
	}
	
	return menu_return::RETURN_REPAINT;
}

#define HEAD_BUTTONS_COUNT 2
const struct button_label HeadButtons[HEAD_BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_SETUP, NONEXISTANT_LOCALE, NULL },
	{ NEUTRINO_ICON_BUTTON_HELP, NONEXISTANT_LOCALE, NULL }
};

#define FOOT_BUTTONS_COUNT 4
const struct button_label AudioPlayerButtons[FOOT_BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_RED, LOCALE_AUDIOPLAYER_DELETE, NULL },
	{ NEUTRINO_ICON_BUTTON_GREEN, LOCALE_AUDIOPLAYER_ADD, NULL },
	{ NEUTRINO_ICON_BUTTON_YELLOW, LOCALE_AUDIOPLAYER_DELETEALL, NULL },
	{ NEUTRINO_ICON_BUTTON_BLUE, LOCALE_AUDIOPLAYER_SHUFFLE, NULL }
};

void CIceCast::showMenu(bool reload)
{
	ilist = new ClistBox("Ice Cast", NEUTRINO_ICON_ICECAST_SMALL, w_max ( (frameBuffer->getScreenWidth() / 20 * 17), (frameBuffer->getScreenWidth() / 20 )), h_max ( (frameBuffer->getScreenHeight() / 20 * 16), (frameBuffer->getScreenHeight() / 20)));
	
	//
	if(reload)
	{
		std::string answer = "";

		if(::getUrl(icecasturl, answer, " ", GET_ICECAST_TIMEOUT))
		{
			xmlDocPtr answer_parser = parseXml(answer.c_str());
		
			scanXmlData(answer_parser, "server_name", "listen_url", "bitrate", true);
		}
		else
			HintBox(LOCALE_MESSAGEBOX_INFO, "can't load icecast list");
	}

	for(unsigned int i = 0; i < playlist.size(); i++)
	{
		//
		if (playlist[i].FileExtension != CFile::EXTENSION_URL && !playlist[i].MetaData.bitrate)
		{
			// 
			GetMetaData(playlist[i]);
		}

		std::string tmp;
		
		//
		getFileInfoToDisplay(tmp, playlist[i]);

		char duration[9];
		snprintf(duration, 8, "%ldk", playlist[i].MetaData.total_time);

		//
		item = new ClistBoxItem(tmp.c_str(), true, "", this, "iplay");
			
		item->setOptionInfo(duration);
		item->setNumber(i + 1);

		// details Box
		item->setInfo1(tmp.c_str());
		//item->setOptionInfo1(genre.c_str());
		//item->setInfo2(artist.c_str());
		//item->setOptionInfo2(date.c_str());

		ilist->addItem(item);
	}

	//ilist->setTimeOut(g_settings.timing[SNeutrinoSettings::TIMING_CHANLIST]);
	ilist->setSelected(selected);

	ilist->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	ilist->setFooterButtons(AudioPlayerButtons, FOOT_BUTTONS_COUNT);
	
	ilist->enableFootInfo();
	ilist->enablePaintDate();

	ilist->addKey(CRCInput::RC_setup, this, CRCInput::getSpecialKeyName(CRCInput::RC_setup));
	ilist->addKey(CRCInput::RC_info, this, CRCInput::getSpecialKeyName(CRCInput::RC_info));
	ilist->addKey(CRCInput::RC_red, this, CRCInput::getSpecialKeyName(CRCInput::RC_red));
	ilist->addKey(CRCInput::RC_green, this, CRCInput::getSpecialKeyName(CRCInput::RC_green));
	ilist->addKey(CRCInput::RC_yellow, this, CRCInput::getSpecialKeyName(CRCInput::RC_yellow));
	ilist->addKey(CRCInput::RC_blue, this, CRCInput::getSpecialKeyName(CRCInput::RC_blue));

	ilist->exec(NULL, "");
	//ilist->hide();
	delete ilist;
	ilist = NULL;
}


void plugin_init(void)
{
}

void plugin_del(void)
{
}

void plugin_exec(void)
{
/*
	std::string answer = "";

	if(::getUrl(icecasturl, answer, " ", GET_ICECAST_TIMEOUT))
	{
		xmlDocPtr answer_parser = parseXml(answer.c_str());
		
		//tmpAudioPlayerGui.scanXmlData(answer_parser, "server_name", "listen_url", "bitrate", true);
		tmpAudioPlayerGui.setTitle("Ice Cast");
		tmpAudioPlayerGui.setInetMode();
		tmpAudioPlayerGui.exec(NULL, "");
	}
	else
		HintBox(LOCALE_MESSAGEBOX_INFO, "can't load icecast list");
*/
	CIceCast* iceCastHandler = new CIceCast();
	
	iceCastHandler->showMenu();
	
	delete iceCastHandler;
	iceCastHandler = NULL;
}
