/*
	* $Id: zapit_setup.h 2013/08/18 11:23:30 mohousch Exp $

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

#ifndef __ZAPIT_SETUP__
#define __ZAPIT_SETUP__

#include <gui/widget/menue.h>

#include <string>

/*zapit includes*/
#include <client/zapitclient.h>


class CZapitSetup : public CMenuTarget, CChangeObserver
{
	private:
		CMenuOptionChooser * zapit1;
		CMenuForwarder * zapit2, * zapit3;

		void showMenu();

	public:
		CZapitSetup();
		~CZapitSetup();
		int exec(CMenuTarget* parent, const std::string & actionKey);
		virtual bool changeNotify(const neutrino_locale_t , void *);
};

class CSelectChannelWidget : public CMenuWidget
{
	private:
		void InitZapitChannelHelper(CZapitClient::channelsMode mode);

	public:
		CSelectChannelWidget();
		~CSelectChannelWidget();
		int exec(CMenuTarget *parent, const std::string & actionKey);
};

#endif
