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

#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent)
{
	QIcon icon("images/settings.ico");
	setWindowIcon(icon);

	contentsWidget = new QListWidget;
	contentsWidget->setViewMode(QListView::IconMode);
	contentsWidget->setIconSize(QSize(48, 48));
	contentsWidget->setMovement(QListView::Static);
	contentsWidget->setMaximumWidth(128);
	contentsWidget->setCurrentRow(0);

	gamesPage = new GamesPage;

	pagesWidget = new QStackedWidget;
	pagesWidget->addWidget(gamesPage);
//	pagesWidget->addWidget(new UpdatePage);
//	pagesWidget->addWidget(new QueryPage);

	QPushButton *applyButton = new QPushButton(tr("Apply"));
	QPushButton *closeButton = new QPushButton(tr("Close"));
	QPushButton *restoreButton = new QPushButton(tr("Restore Defaults"));

	createIcons();

	QHBoxLayout *horizontalLayout = new QHBoxLayout;
	horizontalLayout->addWidget(contentsWidget);
	horizontalLayout->addWidget(pagesWidget, 1);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch(1);
	buttonsLayout->addWidget(restoreButton);
	buttonsLayout->addWidget(applyButton);
	buttonsLayout->addWidget(closeButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(horizontalLayout);
	mainLayout->addLayout(buttonsLayout);
	setLayout(mainLayout);

	setWindowTitle(tr("Settings Dialog"));

	connect(restoreButton, SIGNAL(clicked()), this, SLOT(restoreDefaults()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void SettingsDialog::createIcons()
{
	QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
	configButton->setIcon(QIcon("images/game_pad.png"));
	configButton->setText(tr("Games"));
	configButton->setTextAlignment(Qt::AlignHCenter);
	configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	connect(contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void SettingsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
		current = previous;

	pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void SettingsDialog::applyChanges()
{
	//TODO: gestire solo per la pagina selezionata?
	gamesPage->applyChanges();
}

void SettingsDialog::restoreDefaults()
{
	//TODO: gestire solo per la pagina selezionata?
	gamesPage->restoreDefaults();
}
