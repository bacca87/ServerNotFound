#
#  This file is part of Server Not Found (SNF), an advanced cross-platform
#  game server browser that works with dozens of online games.
#
#  Authors (Alphabetically ordered):
#	Copyright (C) 2010  Marco "SNaKE" Baccarani, <bacca87@gmail.com>
#	Copyright (C) 2010  Riccardo "vortex" Pedrielli, <vortex@ngi.it>
#
#  Server Not Found is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Server Not Found is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Server Not Found.  If not, see <http://www.gnu.org/licenses/>.
#

QT       += core gui network xml

TARGET = ServerNotFound
TEMPLATE = app


SOURCES += src/main.cpp\
	src/mainwindow.cpp \
	src/ValveServerList.cpp \
	src/ValveServerQuery.cpp \
	src/ValveRcon.cpp \
	src/Utility.cpp \
	src/ValveInterface.cpp \
	src/IpToCountry.cpp \
	src/Favorites.cpp \
    src/Settings/SettingsDialog.cpp \
    src/Settings/GamesPage.cpp

HEADERS += src/mainwindow.h \
	src/ValveServerList.h \
	src/ValveServerQuery.h \
	src/Global.h \
	src/ValveRcon.h \
	src/Utility.h \
	src/ValveInterface.h \
	src/Game.h \
	src/ValveGame.h \
	src/IpToCountry.h \
	src/Favorites.h \
    src/Settings/SettingsDialog.h \
    src/Settings/GamesPage.h \
    src/Settings/SettingsPage.h
