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

#include "GamesPage.h"

extern QList<Game*> gameList;

GamesPage::GamesPage(QWidget *parent) :
	SettingsPage(parent)
{
	gamesSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "ServerNotFound", "games");

	QLabel *gameLabel = new QLabel(tr("Game:"));

	gamesCombo = new QComboBox;
	gamesCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	gamesCombo->addItem(" -- " + tr("Select") + " --");

	for(int i = 0; i < gameList.length(); i++)
		gamesCombo->addItem(gameList.at(i)->getIcon(), gameList.at(i)->getName(), gameList.at(i)->getUID());

	QHBoxLayout *gameSelectionLayout = new QHBoxLayout;
	gameSelectionLayout->addWidget(gameLabel);
	gameSelectionLayout->addWidget(gamesCombo);

	QGroupBox *configGroup = new QGroupBox(tr("Select Game"));
	configGroup->setLayout(gameSelectionLayout);

	QLabel *binPathLabel = new QLabel(tr("Bin Path:"));
	binPathEdit = new QLineEdit;
	openFileButton = new QPushButton("...");
	openFileButton->setMaximumWidth(40);

	QHBoxLayout *binPathLayout = new QHBoxLayout;
	binPathLayout->addWidget(binPathLabel);
	binPathLayout->addWidget(binPathEdit);
	binPathLayout->addWidget(openFileButton);

	QLabel *launchOptionsLabel = new QLabel(tr("Launch Options:"));
	launchOptionsEdit = new QLineEdit;

	QHBoxLayout *launchOptionsLayout = new QHBoxLayout;
	launchOptionsLayout->addWidget(launchOptionsLabel);
	launchOptionsLayout->addWidget(launchOptionsEdit);

	QVBoxLayout *propertiesLayout = new QVBoxLayout;

#ifdef Q_OS_WIN32
	enableProtocolCheck = new QCheckBox(tr("Use the dedicated protocol"));
	enableProtocolCheck->setChecked(false);
	enableProtocolCheck->setEnabled(false);

	binPathEdit->setEnabled(false);
	launchOptionsEdit->setEnabled(false);
	openFileButton->setEnabled(false);

	propertiesLayout->addWidget(enableProtocolCheck);

	connect(enableProtocolCheck, SIGNAL(stateChanged(int)), this, SLOT(enableProtocol(int)));
#endif

	propertiesLayout->addLayout(binPathLayout);
	propertiesLayout->addLayout(launchOptionsLayout);

	QGroupBox *propertiesGroup = new QGroupBox(tr("Properties"));
	propertiesGroup->setLayout(propertiesLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(configGroup);
	mainLayout->addWidget(propertiesGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	connect(openFileButton, SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(gamesCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(gameChanged(int)));
}

void GamesPage::gameChanged(int index)
{
	if(index > 0)
	{
		Game *game = gameList.at(index -1);

#ifdef Q_OS_WIN32
		if(gamesSettings->value(game->getAlias() + "/UseDedicatedProtocol", true).toBool())
			enableProtocolCheck->setChecked(true);
		else
			enableProtocolCheck->setChecked(false);
#else
		binPathEdit->setEnabled(true);
		launchOptionsEdit->setEnabled(true);
#endif
		enableProtocolCheck->setEnabled(true);
		binPathEdit->setText(gamesSettings->value(game->getAlias() + "/BinPath", "").toString());
		launchOptionsEdit->setText(gamesSettings->value(game->getAlias() + "/LaunchOptions", "").toString());
	}
	else
	{
#ifdef Q_OS_WIN32
		enableProtocolCheck->setChecked(false);
		enableProtocolCheck->setEnabled(false);
#endif

		binPathEdit->setText("");
		launchOptionsEdit->setText("");
		binPathEdit->setEnabled(false);
		launchOptionsEdit->setEnabled(false);
	}
}

void GamesPage::openFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select the game path"), "/", tr("Executable (*.exe)"));

	if(fileName != "")
		binPathEdit->setText(fileName);
}

#ifdef Q_OS_WIN32
void GamesPage::enableProtocol(int state)
{
	binPathEdit->setEnabled(state == Qt::Unchecked);
	launchOptionsEdit->setEnabled(state == Qt::Unchecked);
	openFileButton->setEnabled(state == Qt::Unchecked);
}
#endif

void GamesPage::applyChanges()
{
	if(gamesCombo->currentIndex() > 0)
	{
		Game *game = gameList.at(gamesCombo->currentIndex() - 1);
#ifdef Q_OS_WIN32
		gamesSettings->setValue(game->getAlias() + "/UseDedicatedProtocol", (bool)enableProtocolCheck->checkState());
#endif
		gamesSettings->setValue(game->getAlias() + "/LaunchOptions", launchOptionsEdit->text());
		gamesSettings->setValue(game->getAlias() + "/BinPath", binPathEdit->text());
		gamesSettings->sync();
	}
}

void GamesPage::restoreDefaults()
{
#ifdef Q_OS_WIN32
	enableProtocolCheck->setChecked(true);
#endif

	binPathEdit->setText("");
	launchOptionsEdit->setText("");
}
