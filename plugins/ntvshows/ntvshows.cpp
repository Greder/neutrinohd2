/*
  $Id: ntvshows.cpp 2018/08/02 mohousch Exp $

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
#include <jsoncpp/include/json/json.h>
#include <system/ytparser.h>

#include "nseasons.h"


extern "C" void plugin_exec(void);
extern "C" void plugin_init(void);
extern "C" void plugin_del(void);

class CTVShows : public CMenuTarget
{
	private:
		//
		int selected;

		//
		CMenuWidget* mlist;
		CMenuItem* item;

		std::string caption;

		//
		CTmdb* tmdb;
		std::string thumbnail_dir;
		CFileHelpers fileHelper;
		cYTFeedParser ytparser;

		CMovieInfo m_movieInfo;
		std::vector<MI_MOVIE_INFO> m_vMovieInfo;
		std::vector<MI_MOVIE_INFO> list;

		//
		std::vector<tmdbinfo> db_movies;

		std::string plist;
		unsigned int page;
		int season_id;
		
		CMoviePlayerGui tmpMoviePlayerGui;

		void loadPlaylist();
		void createThumbnailDir();
		void removeThumbnailDir();
		void showMovieInfo(MI_MOVIE_INFO& movie);

		void loadMoviesTitle(void);
		int showCategoriesMenu();

		void showMenu();
	
	public:
		CTVShows(std::string tvlist = "popular");
		~CTVShows();
		int exec(CMenuTarget* parent, const std::string& actionKey);
		void hide();
};

CTVShows::CTVShows(std::string tvlist)
{
	//
	mlist = NULL;
	item = NULL;

	tmdb = NULL;
	thumbnail_dir = "/tmp/ntvshows";
	fileHelper.createDir(thumbnail_dir.c_str(), 0755);

	//
	selected = 0;

	plist = tvlist;
	page = 1;
	season_id = 0;
}

CTVShows::~CTVShows()
{
	m_vMovieInfo.clear();
	list.clear();
	db_movies.clear();

	fileHelper.removeDir(thumbnail_dir.c_str());
}

void CTVShows::hide()
{
	CFrameBuffer::getInstance()->clearFrameBuffer();
	CFrameBuffer::getInstance()->blit();
}

void CTVShows::createThumbnailDir()
{
	fileHelper.createDir(thumbnail_dir.c_str(), 0755);
}

void CTVShows::removeThumbnailDir()
{
	fileHelper.removeDir(thumbnail_dir.c_str());
}

void CTVShows::loadMoviesTitle(void)
{
	list.clear();
	db_movies.clear();

	removeThumbnailDir();
	createThumbnailDir();

	CHintBox loadBox("Serien Trailer", g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();

	//
	tmdb = new CTmdb();

	tmdb->clearMovieList();

	tmdb->getMovieTVList("tv", plist, page);

	std::vector<tmdbinfo> &mvlist = tmdb->getMovies();
	
	for (unsigned int count = 0; count < mvlist.size(); count++) 
	{
		MI_MOVIE_INFO Info;
		m_movieInfo.clearMovieInfo(&Info);
		
		Info.epgTitle = mvlist[count].title;
		Info.epgInfo1 = mvlist[count].overview;
		Info.ytdate = mvlist[count].release_date;

		std::string tname = thumbnail_dir;
		tname += "/";
		tname += mvlist[count].poster_path;
		tname += ".jpg";

		if (!mvlist[count].poster_path.empty())
		{

			::downloadUrl("http://image.tmdb.org/t/p/w185" + mvlist[count].poster_path, tname);
		}

		if(!tname.empty())
			Info.tfile = tname;

		//
		tmdbinfo tmp;

		tmp.title = mvlist[count].title;
		tmp.id = mvlist[count].id;
		tmp.seasons = mvlist[count].seasons;

		db_movies.push_back(tmp);
		
		list.push_back(Info);
	}
	
	delete tmdb;
	tmdb = NULL;
}

void CTVShows::loadPlaylist()
{
	m_vMovieInfo.clear();

	//
	tmdb = new CTmdb();
	
	// refill our structure
	for (unsigned int i = 0; i < db_movies.size(); i++)
	{
		MI_MOVIE_INFO movieInfo;
		m_movieInfo.clearMovieInfo(&movieInfo); 

		movieInfo.epgTitle = db_movies[i].title;

		tmdb->clearMovieInfo();
		tmdb->getMovieTVInfo("tv", db_movies[i].id);
		std::vector<tmdbinfo>& movieInfo_list = tmdb->getMovieInfos();

		movieInfo.epgInfo1 = movieInfo_list[0].overview;
		movieInfo.ytdate = movieInfo_list[0].release_date;
		movieInfo.vote_average = movieInfo_list[0].vote_average;
		movieInfo.vote_count = movieInfo_list[0].vote_count;
		movieInfo.original_title = movieInfo_list[0].original_title;
		movieInfo.release_date = movieInfo_list[0].release_date;
		movieInfo.media_type = movieInfo_list[0].media_type;
		movieInfo.length = movieInfo_list[0].runtime;
		movieInfo.runtimes = movieInfo_list[0].runtimes;
		movieInfo.genres = movieInfo_list[0].genres;
		movieInfo.cast = movieInfo_list[0].cast;
		movieInfo.seasons = movieInfo_list[0].seasons;
		movieInfo.episodes = movieInfo_list[0].episodes;
			
		std::string tname = thumbnail_dir;
		tname += "/";
		tname += movieInfo.epgTitle;
		tname += ".jpg";

		tmdb->getSmallCover(movieInfo_list[0].poster_path, tname);

		if(!tname.empty())
			movieInfo.tfile = tname;
					
		// 
		m_vMovieInfo.push_back(movieInfo);
	}

	delete tmdb;
	tmdb = NULL;
}

void CTVShows::showMovieInfo(MI_MOVIE_INFO& movie)
{
	std::string buffer;
	
	// prepare print buffer 
	buffer = movie.epgTitle;
	buffer += "\n"; 
	buffer += movie.epgInfo1;
	buffer += "\n";
	buffer += movie.epgInfo2;

	// thumbnail
	int pich = 246;	//FIXME
	int picw = 162; 	//FIXME

	std::string thumbnail = movie.tfile;
	if(access(thumbnail.c_str(), F_OK))
		thumbnail = "";
	
	CBox position(g_settings.screen_StartX + 50, g_settings.screen_StartY + 50, g_settings.screen_EndX - g_settings.screen_StartX - 100, g_settings.screen_EndY - g_settings.screen_StartY - 100); 
	
	CInfoBox * infoBox = new CInfoBox(g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1], SCROLL, &position, movie.epgTitle.c_str(), g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE], NEUTRINO_ICON_MOVIE);

	infoBox->setText(buffer.c_str(), thumbnail.c_str(), picw, pich);
	infoBox->exec();
	delete infoBox;
}

#define HEAD_BUTTONS_COUNT	2
const struct button_label HeadButtons[HEAD_BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_HELP, NONEXISTANT_LOCALE, NULL },
	{ NEUTRINO_ICON_BUTTON_SETUP, NONEXISTANT_LOCALE, NULL }
};

#define FOOT_BUTTONS_COUNT	4
const struct button_label FootButtons[FOOT_BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_RED, LOCALE_FILEBROWSER_NEXTPAGE, NULL },
	{ NEUTRINO_ICON_BUTTON_GREEN, LOCALE_FILEBROWSER_PREVPAGE, NULL },
	{ NEUTRINO_ICON_BUTTON_YELLOW, NONEXISTANT_LOCALE, NULL },
	{ NEUTRINO_ICON_BUTTON_BLUE, NONEXISTANT_LOCALE, NULL }
};

void CTVShows::showMenu()
{
	dprintf(DEBUG_NORMAL, "CTVShows::showMenu:");

	if(plist == "airing_today")
		caption = "Heute auf Sendung";
	else if(plist == "on_the_air")
		caption = "Auf Sendung";
	else if(plist == "popular")
		caption = "Am populärsten";
	else if(plist == "top_rated")
		caption = "Am besten bewertet";

	mlist = new CMenuWidget(caption.c_str(), NEUTRINO_ICON_TMDB, w_max ( (CFrameBuffer::getInstance()->getScreenWidth() / 20 * 17), (CFrameBuffer::getInstance()->getScreenWidth() / 20 )), h_max ( (CFrameBuffer::getInstance()->getScreenHeight() / 20 * 17), (CFrameBuffer::getInstance()->getScreenHeight() / 20)));
	
	
	// load playlist
	loadPlaylist();

	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		//
		if(db_movies[i].title == m_vMovieInfo[i].epgTitle)
				season_id = db_movies[i].id; 
		//

		std::string tmp = m_vMovieInfo[i].ytdate;
		tmp += " ";
		tmp += m_vMovieInfo[i].epgInfo1;

		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, new CNSeasons(season_id), NULL, RC_nokey, NULL, file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->setHelpText(tmp.c_str());

		mlist->addItem(item);
	}

	mlist->setWidgetMode(MODE_LISTBOX);
	mlist->setWidgetType(WIDGET_TYPE_FRAME);
	mlist->setItemsPerPage(6, 2);
	//mlist->setItemBoxColor(COL_YELLOW);
	mlist->setSelected(selected);
	mlist->enablePaintDate();
	mlist->enablePaintFootInfo();

	mlist->setHeaderButtons(HeadButtons, HEAD_BUTTONS_COUNT);
	mlist->setFooterButtons(FootButtons, FOOT_BUTTONS_COUNT);

	mlist->addKey(RC_info, this, CRCInput::getSpecialKeyName(RC_info));
	mlist->addKey(RC_setup, this, CRCInput::getSpecialKeyName(RC_setup));
	mlist->addKey(RC_red, this, CRCInput::getSpecialKeyName(RC_red));
	mlist->addKey(RC_green, this, CRCInput::getSpecialKeyName(RC_green));

	mlist->exec(NULL, "");
	delete mlist;
	mlist = NULL;
}

int CTVShows::showCategoriesMenu()
{
	dprintf(DEBUG_NORMAL, "showCategoriesMenu:\n");

	int res = -1;

	CMenuWidget * menu = new CMenuWidget("Serien Trailer");

	menu->setWidgetMode(MODE_MENU);
	menu->enableShrinkMenu();

	menu->addItem(new CMenuForwarder("Heute auf Sendung", true, NULL, new CTVShows("airing_today"), "airing_today"));
	menu->addItem(new CMenuForwarder("Auf Sendung", true, NULL, new CTVShows("on_the_air"), "on_the_air"));
	menu->addItem(new CMenuForwarder("Am populärsten", true, NULL, new CTVShows("popular"), "popular"));
	menu->addItem(new CMenuForwarder("Am besten bewertet", true, NULL, new CTVShows("top_rated"), "top_rated"));

	menu->exec(NULL, "");
	menu->hide();
	res = menu->getSelected();
	delete menu;
	menu = NULL;

	return res;
}

int CTVShows::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CTVShows::exec: actionKey: %s\n", actionKey.c_str());

	if(parent)
		hide();

	if(actionKey == "RC_info")
	{
		selected = mlist->getSelected();

		showMovieInfo(m_vMovieInfo[selected]);

		return RETURN_REPAINT;
	}
	else if(actionKey == "RC_setup")
	{
		int res = showCategoriesMenu();

		if(res >= 0 && res <= 3)
		{
			loadMoviesTitle();
			showMenu();

			return RETURN_EXIT_ALL;
		}
		else
			return RETURN_REPAINT;
	}
	else if(actionKey == "RC_red")
	{
		page++;
		selected = 0;
		loadMoviesTitle();
		showMenu();

		return RETURN_EXIT_ALL;
	}
	else if(actionKey == "RC_green")
	{
		page--;

		if(page <= 1)
			page = 1;

		selected = 0;

		loadMoviesTitle();
		showMenu();

		return RETURN_EXIT_ALL;
	}

	loadMoviesTitle();
	showMenu();

	return RETURN_EXIT_ALL;
}

void plugin_init(void)
{
	dprintf(DEBUG_NORMAL, "CTVShows: plugin_init\n");
}

void plugin_del(void)
{
	dprintf(DEBUG_NORMAL, "CTVShows: plugin_del\n");
}

void plugin_exec(void)
{
	CTVShows* nTVShowsHandler = new CTVShows();

	nTVShowsHandler->exec(NULL, "");
	
	delete nTVShowsHandler;
	nTVShowsHandler = NULL;
}


