/*
  Neutrino-GUI  -   DBoxII-Project
  
  $Id: audioplayer.h 2018/07/10 mohousch Exp $

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

#ifndef __audioplayergui__
#define __audioplayergui__


#include "driver/framebuffer.h"
#include "driver/audiofile.h"
#include "driver/audioplay.h"
#include "gui/filebrowser.h"
#include "gui/widget/menue.h"

#include <libxmltree/xmlinterface.h>

#include <string>
#include <set>
#include <map>
#include <cstdlib>
#include <ctime>


typedef std::set<long> CPosList;
typedef std::map<unsigned char, CPosList> CTitle2Pos;
typedef std::pair<unsigned char, CPosList> CTitle2PosItem;

class CAudiofileExt : public CAudiofile
{
	public:
		CAudiofileExt();
		CAudiofileExt(std::string name, CFile::FileExtension extension);
		CAudiofileExt(const CAudiofileExt& src);
		void operator=(const CAudiofileExt& src);

		char firstChar;
};

typedef std::vector<CAudiofileExt> CAudioPlayList;

class CAudioPlayerGui : public CMenuTarget
{
	public:
		enum State
		{
			PLAY = 0,
			STOP,
			PAUSE,
			FF,
			REV
		};

		enum DisplayOrder 
		{
			ARTIST_TITLE = 0, 
			TITLE_ARTIST = 1
		};

	private:
		CFrameBuffer * m_frameBuffer;

		//
		int            m_current;

		// gui
		int m_x, m_y;
		int m_title_height;
		int m_width;

		//
		State          m_state;
		time_t         m_time_total;
		time_t         m_time_played;
		std::string    m_metainfo;
		bool           m_select_title_by_name;
		bool           m_playlistHasChanged;

		//
		CAudioPlayList m_playlist;
		CTitle2Pos     m_title2Pos;
		CAudiofileExt  m_curr_audiofile;

		int            m_LastMode;
		int            m_idletime;
		bool           m_inetmode;
		uint32_t       stimer;
		
		//
		SMSKeyInput    m_SMSKeyInput;

		//
		bool updateMeta;
		bool updateLcd;
		bool updateScreen;

		//
		void Init(void);

		// gui
		void hide();
		void paintInfo();

		// lcd
		void paintLCD();

		//
		void get_id3(CAudiofileExt * audiofile);
		void get_mp3info(CAudiofileExt * audiofile);

		//
		int getNext();
		void GetMetaData(CAudiofileExt &File);
		void updateMetaData();
		void updateTimes(const bool force = false);
		void showMetaData();
		bool getNumericInput(neutrino_msg_t& msg,int& val);

		void selectTitle(unsigned char selectionChar);
		/**
		* Appends the file information to the given string.
		* @param fileInfo a string where the file information will be appended
		* @param file the file to return the information for
		*/
		void getFileInfoToDisplay(std::string& fileInfo, CAudiofileExt &file);
		void printSearchTree();
		void buildSearchTree();
		unsigned char getFirstChar(CAudiofileExt &file);

		/**
		* Saves the current playlist into a .m3u playlist file.
		*/
		void savePlaylist();

		/**
		* Converts an absolute filename to a relative one
		* as seen from a file in fromDir.
		* Example:
		* absFilename: /mnt/audio/A/abc.mp3
		* fromDir: /mnt/audio/B
		* => ../A/abc.mp3 will be returned 
		* @param fromDir the directory from where we want to
		* access the file
		* @param absFilename the file we want to access in a
		* relative way from fromDir (given as an absolute path)
		* @return the location of absFilename as seen from fromDir
		* (relative path)
		*/
		std::string absPath2Rel(const std::string& fromDir, const std::string& absFilename);
	
		/** 
		* Asks the user if the file filename should be overwritten or not
		* @param filename the name of the file
		* @return true if file should be overwritten, false otherwise
		*/
		bool askToOverwriteFile(const std::string& filename);

		//
		void pause();
		void ff(unsigned int seconds = 0);
		void rev(unsigned int seconds = 0);
		bool playNext(bool allow_rotate = false);
		bool playPrev(bool allow_rotate = false);
	
	public:
		CAudioPlayerGui();
		~CAudioPlayerGui();
		int show();
		int exec(CMenuTarget *parent, const std::string &actionKey);

		//
		void addToPlaylist(CAudiofileExt &file);
		void removeFromPlaylist(long pos);
		void clearPlaylist(void);

		//
		void setInetMode(void){m_inetmode = true;};

		//
		void play(unsigned int pos);
		void stop();

		void setCurrent(int pos = 0){m_current = pos;};
};

#endif
