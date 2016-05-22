//
//  This file is part of Server Not Found (SNF), an advanced cross-platform
//  game server browser that works with dozens of online games.
//
//  Authors (Alphabetically ordered):
//	Copyright (C) 2010  Marco "SNaKE" Baccarani, <bacca87@gmail.com>
//	Copyright (C) 2010  Riccardo "vort3x" Pedrielli, <vort3x@ngi.it>
//
//  Server Not Found is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Server Not Found is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Server Not Found.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef GAMESPAGE_H
#define GAMESPAGE_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QSettings>
#include <QCheckBox>

#include "SettingsPage.h"
#include "../Game.h"

class GamesPage : public SettingsPage
{
	Q_OBJECT

#ifdef Q_OS_WIN32
	QCheckBox *enableProtocolCheck;
#endif

	QLineEdit *binPathEdit;
	QLineEdit *launchOptionsEdit;
	QSettings *gamesSettings;
	QPushButton *openFileButton;
	QComboBox *gamesCombo;

public:
	explicit GamesPage(QWidget *parent = 0);
	void applyChanges();
	void restoreDefaults();

private slots:
	void openFileDialog();
	void gameChanged(int index);

#ifdef Q_OS_WIN32
	void enableProtocol(int state);
#endif
};

#endif // GAMESPAGE_H
