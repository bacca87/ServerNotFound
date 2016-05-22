//
//  This file is part of Server Not Found (SNF), an advanced cross-platform
//  game server browser that works with dozens of online games.
//
//  Authors (Alphabetically ordered):
//	Copyright (C) 2010  Marco "SNaKE" Baccarani, <bacca87@gmail.com>
//	Copyright (C) 2010  Riccardo "vortex" Pedrielli, <vortex@ngi.it>
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

#include "mainwindow.h"
#include "Settings/SettingsDialog.h"

extern QList<Game*> gameList;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	drawGUI();

	favorites.load("favorites.xml");

	interfaceThread = new QThread();
	interfaceThread->start();

	vi = new ValveInterface(30);
	vi->moveToThread(interfaceThread);

	connect(vi, SIGNAL(serverInfo(ServerInfo*)), this, SLOT(serverInfo(ServerInfo*)));
	connect(vi, SIGNAL(serverInfoUI(ServerInfo*)), this, SLOT(serverInfoUI(ServerInfo*)));
	connect(vi, SIGNAL(serverListCompleted(quint32)), this, SLOT(finelista(quint32)));
	connect(vi, SIGNAL(playerList(QList<PlayerInfo>)), this, SLOT(playerInfo(QList<PlayerInfo>)));
	connect(vi, SIGNAL(ruleList(QList<Rule>)), this, SLOT(ruleInfo(QList<Rule>)));
	connect(vi, SIGNAL(pingInfo(qint32)), this, SLOT(pingInfo(qint32)));
}

MainWindow::~MainWindow()
{
	while (!gameList.isEmpty())
		 delete gameList.takeFirst();

	while (!serverList.isEmpty())
		 delete serverList.takeFirst();
}

void MainWindow::aboutQt()
{
	QMessageBox::aboutQt(this, "nomeprogramma");
}

void MainWindow::getList()
{
	//TODO: fai il metodo per svuotare la lista
	while (!serverList.isEmpty())
		 delete serverList.takeFirst();
	serverList.clear();
	//

	serverListTable->clearContents();
	serverListTable->setRowCount(0);

	if(gameTree->currentItem())
	{
		switch(gameTree->currentItem()->type())
		{
		case 1:
			currentGame = (Game*)gameTree->currentItem()->data(0, Qt::UserRole).value<int>();

			switch(currentGame->getProtocol())
			{
			case STEAM:
				vi->getServerList(((ValveGame*)currentGame)->getEngine(), ANY, ((ValveGame*)currentGame)->getAppid() != LEFT4DEAD ? LEFT4DEAD : 0, ((ValveGame*)currentGame)->getGameDir(), "", false, false, false, false, false, false, false);
				break;
			}
			break;

		case 2:
			//favorites3
			break;

		default:
			break;
		}
	}

	serversTableGroupBox->setTitle("Servers (0)");
}

void MainWindow::getLANList()
{
	serverListTable->clearContents();
	serverListTable->setRowCount(0);
	vi->getLANServerList();
	serversTableGroupBox->setTitle("Servers (0)");
}

void MainWindow::refreshList()
{
	serverListTable->clearContents();
	serverListTable->setRowCount(0);
	vi->refreshServerList();
	serversTableGroupBox->setTitle("Servers (0)");
}

void MainWindow::stopList()
{
	vi->stopList();
}

void MainWindow::serverInfo(ServerInfo *info)
{
	serverList.append(info);
	addServerToTable(info);
}

void MainWindow::addServerToTable(ServerInfo *info)
{
	if(!checkFilters(*info))
		return;

	const int currentRow = serverListTable->rowCount();
	serverListTable->setRowCount(currentRow + 1);

	QIcon flagIcon("images/flags/16/" + info->country.ctry.toLower() + ".png");
	QIcon passwordIcon("images/password.png");
	QIcon vacIcon("images/vacsimil.png");

	QTableWidgetItem *flagItem = new QTableWidgetItem(flagIcon, "", QTableWidgetItem::Type);
	flagItem->setData(Qt::UserRole, QVariant(info->country.ctry.toLower()));
	flagItem->setToolTip(info->country.country);

	QTableWidgetItem *passwordItem = new QTableWidgetItem("");
	if(info->password) passwordItem->setIcon(passwordIcon);

	QTableWidgetItem *secureItem = new QTableWidgetItem("");
	if(info->secure) secureItem->setIcon(vacIcon);

	QTableWidgetItem *infoPointer = new QTableWidgetItem();
	infoPointer->setData(Qt::UserRole, QVariant((int)info));

	//serverListTable->setSortingEnabled(false); //TODO: Workaround QTBUG-7483

	serverListTable->setItem(currentRow, 0, infoPointer);
	serverListTable->setItem(currentRow, 1, passwordItem);
	serverListTable->setItem(currentRow, 2, secureItem);
	serverListTable->setItem(currentRow, 3, flagItem);
	serverListTable->setItem(currentRow, 4, new QTableWidgetItem(QHostAddress(info->ipv4addr).toString() + ":" + QString::number(info->port)));
	serverListTable->setItem(currentRow, 5, new QTableWidgetItem(QString::number(info->ping)));
	serverListTable->setItem(currentRow, 6, new QTableWidgetItem(info->name));
	serverListTable->setItem(currentRow, 7, new QTableWidgetItem(QString::number(info->players) + QString(" / ") + QString::number(info->maxPlayers) + (info->bots > 0 ? " (" + QString::number(info->bots) + ")" : "")));
	serverListTable->setItem(currentRow, 8, new QTableWidgetItem(info->map));
	serverListTable->setItem(currentRow, 9, new QTableWidgetItem(info->tags));
	serverListTable->setItem(currentRow, 10, new QTableWidgetItem(info->tvPort ? QString::number(info->tvPort) : ""));
	serverListTable->setItem(currentRow, 11, new QTableWidgetItem(info->tvName));

	//serverListTable->setSortingEnabled(true); //TODO: Workaround QTBUG-7483

	serversTableGroupBox->setTitle("Servers (" + QString::number(serverListTable->rowCount()) + ")");
}

void MainWindow::serverInfoUI(ServerInfo *info)
{
	QTableWidgetItem *item = serverListTable->item(serverListTable->currentRow(), 3);
	QPixmap flagPixmap("images/flags/64/" + item->data(Qt::UserRole).value<QString>() + ".png", "png", Qt::AutoColor);
	flagLabel->setPixmap(flagPixmap);
	flagLabel->setToolTip(item->toolTip());

	serverAddressLabel->setText(QHostAddress(info->ipv4addr).toString() + ":" + QString::number(info->port));
	serverPingLabel->setText(QString::number(info->ping) + "ms");
	serverNameLabel->setText(info->name);
	serverMapLabel->setText(info->map);
	serverPlayersLabel->setText(QString::number(info->players) + QString("/") + QString::number(info->maxPlayers) + (info->bots ? " (" + QString::number(info->bots) + ")" : ""));
	serverTagsLabel->setText(info->tags);
	serverSecureLabel->setText((info->secure ? "Yes" : "No"));

	switch(info->dedicated)
	{
	case 'l':
		serverTypeLabel->setText("Listen");
		break;
	case 'd':
		serverTypeLabel->setText("Dedicated");
		break;
	case 'p':
		serverTypeLabel->setText("Source TV");
		break;
	default:
		serverTypeLabel->setText("Unknown");
	}

	serverPasswordLabel->setText((info->password ? "Yes" : "No"));
	serverOSLabel->setText((info->os == 'l'? "GNU/Linux" : "Microsoft Windows"));
	serverGameDescriptionLabel->setText(info->gameDescription + " (" + info->gameDirectory + ")");
	serverGameVersionLabel->setText("v" + info->gameVersion + " (Protocol " + QString::number(info->version) + ")");
	serverAppIDLabel->setText(QString::number(info->appID));
	serverTVNameLabel->setText(info->tvName);
	serverTVPortLabel->setText(info->tvPort ? QString::number(info->tvPort) : "");
	serverSteamIDLabel->setText(QString::number(info->steamID));

	//TODO: test (la funzione va chiamata 2 volte altrimenti non funziona. non chiedetemi il perche)
	serverInfoWidget->adjustSize();
	serverInfoWidget->adjustSize();
	//
}

void MainWindow::playerInfo(QList<PlayerInfo> playerList)
{
	playersTable->clearContents();
	playersTable->setRowCount(0);

	playersGroupBox->setTitle("Players (" + QString::number(playerList.length()) + ")");

	playersTable->setSortingEnabled(false); //TODO: Workaround QTBUG-7483

	for(int i = 0; i < playerList.count(); i++)
	{
		int currentRow = playersTable->rowCount();
		PlayerInfo info = playerList.at(i);

		QTime sessionTime(0,0,0,0);

		playersTable->setRowCount(currentRow + 1);
		playersTable->setItem(currentRow, 0, new QTableWidgetItem(info.name));

		QTableWidgetItem *killsItem = new QTableWidgetItem();
		killsItem->setData(Qt::DisplayRole, info.kills);
		playersTable->setItem(currentRow, 1, killsItem);
		playersTable->setItem(currentRow, 2, new QTableWidgetItem(sessionTime.addSecs(info.time_s).toString("hh:mm:ss")));
	}

	playersTable->setSortingEnabled(true); //TODO: Workaround QTBUG-7483
}

void MainWindow::ruleInfo(QList<Rule> ruleList)
{
	rulesTable->clearContents();
	rulesTable->setRowCount(0);

	rulesGroupBox->setTitle("Rules (" + QString::number(ruleList.length()) + ")");

	for(int i = 0; i < ruleList.count(); i++)
	{
		int currentRow = rulesTable->rowCount();
		Rule info = ruleList.at(i);
		rulesTable->setRowCount(currentRow + 1);
		rulesTable->setItem(currentRow, 0, new QTableWidgetItem(info.name));
		rulesTable->setItem(currentRow, 1, new QTableWidgetItem(info.value));
	}
}

void MainWindow::pingInfo(qint32 msec)
{
	//int a = 0;
	//textArea->clear();
	//textArea->append(QString::number(msec));
}

void MainWindow::finelista(quint32 server_count)
{
	qDebug()<<"FINE LISTA!!!!";
	qDebug()<<server_count;
}

void MainWindow::serverListClicked(int row, int column)
{
	playersTable->clearContents();
	playersTable->setRowCount(0);
	rulesTable->clearContents();
	rulesTable->setRowCount(0);
	rulesGroupBox->setTitle("Rules (0)");
	playersGroupBox->setTitle("Players (0)");

	refreshServerInfo();
}

void MainWindow::serverListDoubleClicked(int row, int column)
{
	ServerInfo info = *(ServerInfo*)serverListTable->item(serverListTable->currentRow(), 0)->data(Qt::UserRole).value<int>();

	QString program = "C:/Programmi/Steam/Steam.exe";
	QStringList arguments;

	arguments << "-applaunch" << QString::number(info.appID) << "+connect" << QHostAddress(info.ipv4addr).toString() + ":" + QString::number(info.port);

	QProcess myProcess;
	myProcess.startDetached(program, arguments);
}

void MainWindow::refreshServerInfo()
{
	ServerInfo info = *(ServerInfo*)serverListTable->item(serverListTable->currentRow(), 0)->data(Qt::UserRole).value<int>();

	vi->getServerInfo(((ValveGame*)currentGame)->getEngine(), info.ipv4addr, info.port);
	vi->getPlayers(((ValveGame*)currentGame)->getEngine(), info.ipv4addr, info.port);
	vi->getRules(((ValveGame*)currentGame)->getEngine(), info.ipv4addr, info.port);
	//vi->getPing(((ValveGame*)currentGame)->getEngine(), info.ipv4addr, info.port);
}

bool MainWindow::checkFilters(ServerInfo info)
{
	if(nameLineEdit->text() != "" && !info.name.contains(nameLineEdit->text(), Qt::CaseInsensitive))
		return false;

	if(ipLineEdit->text() != "" && info.ipv4addr != QHostAddress(ipLineEdit->text()).toIPv4Address())
		return false;

	if(mapLineEdit->text() != "" && !info.map.contains(mapLineEdit->text(), Qt::CaseInsensitive))
		return false;

	if(tagsLineEdit->text() != "")
	{
		QStringList filterTags = tagsLineEdit->text().split(' ');

		for(int i = 0; i < filterTags.length(); i++)
			if(!info.tags.contains(filterTags.at(i), Qt::CaseInsensitive))
				return false;
	}

	if(pingComboBox->itemData(pingComboBox->currentIndex(), Qt::UserRole) != 0)
		if(info.ping > pingComboBox->itemData(pingComboBox->currentIndex(), Qt::UserRole).toInt())
			return false;

	if(secureCheckBox->isChecked() && !info.secure)
		return false;

	if(nopasswordCheckBox->isChecked() && info.password)
		return false;

	if(dedicatedCheckBox->isChecked() && !info.dedicated)
		return false;

	if(notfullCheckBox->isChecked() && (info.players == info.maxPlayers))
		return false;

	if(notemptyCheckBox->isChecked() && info.players == 0)
		return false;

	if(nobotsCheckBox->isChecked() && info.bots > 0)
		return false;

	if(maxplayerSpinBox->value() > 0 && info.maxPlayers > maxplayerSpinBox->value())
		return false;

	return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
	event->ignore();
	hide();
 }

void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger)
	{
		if(isVisible())
		{
			hide();
		}
		else
		{
			if(isMaximized())
				showMaximized();
			else
				showNormal();
		}
	}
}

void MainWindow::serverListContextMenuEvent(QPoint pos)
{
	if(serverListTable->selectedItems().count() > 0)
		QAction *a = serverListContextMenu->exec(serverListTable->viewport()->mapToGlobal(pos));
}

void MainWindow::addToFavorites()
{
	ServerInfo info = *(ServerInfo*)serverListTable->item(serverListTable->currentRow(), 0)->data(Qt::UserRole).value<int>();

	favorites.add(*currentGame, info.ipv4addr, info.port);
}

void MainWindow::editSettings()
{
	SettingsDialog settings;
	settings.exec();
}

/*
	###################################################################################################################################
	#																GUI																  #
	###################################################################################################################################
*/

void MainWindow::drawGUI()
{
	createActions();
	createMenus();
	createTrayIcon();
	setIcon();
	createToolBars();
	createButtons();
	createGameTree();
	createServerList();
	createPlayerList();
	createRuleList();

	trayIcon->show();

	QList<int> verticalSplitterSizes;
	verticalSplitterSizes.append(10);
	verticalSplitterSizes.append(120);

	QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, this);
	verticalSplitter->addWidget(createServerListWidget());
	verticalSplitter->addWidget(createServerDetailsWidget());
	verticalSplitter->setSizes(verticalSplitterSizes);

	QList<int> horizzontalSplitterSizes;
	horizzontalSplitterSizes.append(100);
	horizzontalSplitterSizes.append(500);

	QSplitter *horizzontalSplitter = new QSplitter(this);
	horizzontalSplitter->addWidget(gameTree);
	horizzontalSplitter->addWidget(verticalSplitter);
	horizzontalSplitter->setSizes(horizzontalSplitterSizes);

	statusBar = new QStatusBar(this);
	setStatusBar(statusBar);
	setCentralWidget(horizzontalSplitter);
	setWindowState(Qt::WindowMaximized);
	setWindowTitle("Server Not Found");

	connect(serverListTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(serverListContextMenuEvent(QPoint)));
	connect(serverListTable, SIGNAL(cellClicked(int,int)), this, SLOT(serverListClicked(int,int)));
	connect(serverListTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(serverListDoubleClicked(int,int)));

	connect(aboutQtAction,		SIGNAL(triggered()), this, SLOT(aboutQt()));
	connect(serverListAction,	SIGNAL(triggered()), this, SLOT(getList()));
	connect(lanServerListAction,	SIGNAL(triggered()), this, SLOT(getLANList()));
	connect(stopListAction,		SIGNAL(triggered()), this, SLOT(stopList()));
	connect(refreshListAction,	SIGNAL(triggered()), this, SLOT(refreshList()));

	connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(addToFavoritesAction, SIGNAL(triggered()), this, SLOT(addToFavorites()));
	connect(editSettingsAction, SIGNAL(triggered()), this, SLOT(editSettings()));

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::createActions()
{
	aboutQtAction		 = new QAction("About Qt", this);
	serverListAction	 = new QAction(QIcon("images/global-search.png"), "Get Global List", this);
	lanServerListAction	 = new QAction(QIcon("images/lan-search.png"), "Get LAN List", this);
	stopListAction		 = new QAction(QIcon("images/stop.png"), "Stop List", this);
	refreshListAction	 = new QAction(QIcon("images/refresh-list.png"), "Refresh List", this);
	minimizeAction		 = new QAction(tr("Mi&nimize"), this);
	maximizeAction		 = new QAction(tr("Ma&ximize"), this);
	restoreAction		 = new QAction(tr("&Restore"), this);
	quitAction			 = new QAction(tr("&Quit"), this);
	addToFavoritesAction = new QAction("Add to Favorites", this);
	editSettingsAction	 = new QAction("Settings", this);
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(quitAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(editSettingsAction);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutQtAction);

	serverListContextMenu = new QMenu(this);
	serverListContextMenu->addAction(addToFavoritesAction);
}

void MainWindow::createGameTree()
{
	gameTree = new QTreeWidget(this);
	gameTree->setHeaderLabel("Games");

	QSize gameTreeItemSize = QSize(0, 18);

	for(int i = 0; i < gameList.length(); i++)
	{
		Game *game = gameList.at(i);

		QTreeWidgetItem *gameItem = new QTreeWidgetItem(1); //TODO: define dei tipi
		gameItem->setData(0, Qt::UserRole, QVariant((int)game));
		gameItem->setText(0, game->getName());
		gameItem->setIcon(0, game->getIcon());

		QTreeWidgetItem *favoritesItem = new QTreeWidgetItem(gameItem, 2); //TODO: define dei tipi
		favoritesItem->setText(0, "Favorites");
		favoritesItem->setIcon(0, QIcon("images/favorites.png"));

		gameTree->insertTopLevelItem(0, gameItem);
		gameItem->setSizeHint(0, gameTreeItemSize);
		favoritesItem->setSizeHint(0, gameTreeItemSize);
	}

	gameTree->sortItems(0, Qt::AscendingOrder);
	//gameTree->expandAll();
}

void MainWindow::createServerList()
{
	serverListTable = new QTableWidget();
	//serverListTable->sortByColumn(4, Qt::AscendingOrder);
	//serverListTable->setSortingEnabled(true);
	serverListTable->verticalHeader()->setDefaultSectionSize(20);
	serverListTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	serverListTable->setSelectionMode(QAbstractItemView::SingleSelection);
	serverListTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	serverListTable->verticalHeader()->hide();
	//serverListTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	serverListTable->setColumnCount(12);

	serverListTable->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
	serverListTable->setHorizontalHeaderItem(1, new QTableWidgetItem(""));
	serverListTable->setHorizontalHeaderItem(2, new QTableWidgetItem(""));
	serverListTable->setHorizontalHeaderItem(3, new QTableWidgetItem(""));
	serverListTable->setHorizontalHeaderItem(4, new QTableWidgetItem("Address"));
	serverListTable->setHorizontalHeaderItem(5, new QTableWidgetItem("Ping"));
	serverListTable->setHorizontalHeaderItem(6, new QTableWidgetItem("Name"));
	serverListTable->setHorizontalHeaderItem(7, new QTableWidgetItem("Players"));
	serverListTable->setHorizontalHeaderItem(8, new QTableWidgetItem("Map"));
	serverListTable->setHorizontalHeaderItem(9, new QTableWidgetItem("Tags"));
	serverListTable->setHorizontalHeaderItem(10, new QTableWidgetItem("TV Port"));
	serverListTable->setHorizontalHeaderItem(11, new QTableWidgetItem("TV Name"));

	serverListTable->hideColumn(0); //pointer to ServerInfo

	serverListTable->horizontalHeader()->setResizeMode(1,QHeaderView::Custom);
	serverListTable->horizontalHeader()->setResizeMode(2,QHeaderView::Custom);
	serverListTable->horizontalHeader()->setResizeMode(3,QHeaderView::Custom);
	serverListTable->setColumnWidth(1, 25);
	serverListTable->setColumnWidth(2, 25);
	serverListTable->setColumnWidth(3, 25);
	serverListTable->setColumnWidth(4, 130);
	serverListTable->setColumnWidth(5, 35);
	serverListTable->setColumnWidth(6, 500);
	serverListTable->setColumnWidth(7, 60);
	serverListTable->setColumnWidth(8, 130);
	serverListTable->setColumnWidth(9, 400);
	serverListTable->setColumnWidth(10, 50);
	serverListTable->setColumnWidth(11, 200);

	serverListTable->setContextMenuPolicy(Qt::CustomContextMenu);

	//table->sortByColumn();
	//table->setSortingEnabled(true); //genera un bordello se si usano i filtri durante la ricezione server
}

void MainWindow::createPlayerList()
{
	playersTable = new QTableWidget();
	playersTable->setSortingEnabled(true);
	playersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	playersTable->setSelectionMode(QAbstractItemView::SingleSelection);
	playersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	playersTable->setGridStyle(Qt::NoPen);
	playersTable->verticalHeader()->setDefaultSectionSize(20);
	playersTable->setColumnCount(3);
	playersTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
	playersTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Kills"));
	playersTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Time"));
	playersTable->setColumnWidth(0, 200);
	playersTable->setColumnWidth(1, 50);
	playersTable->setColumnWidth(2, 55);
}

void MainWindow::createRuleList()
{
	rulesTable = new QTableWidget();
	rulesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	rulesTable->setSelectionMode(QAbstractItemView::SingleSelection);
	rulesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	rulesTable->setGridStyle(Qt::NoPen);
	rulesTable->verticalHeader()->setDefaultSectionSize(20);
	rulesTable->verticalHeader()->hide();
	rulesTable->setColumnCount(2);
	rulesTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Variable"));
	rulesTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));
	rulesTable->setColumnWidth(0, 150);
	rulesTable->setColumnWidth(1, 300);
}

void MainWindow::createButtons()
{
	//btnServerList = new QPushButton();
	//btnServerList->setText("Get Global List");
}

void MainWindow::createToolBars()
{
	QToolBar *commandToolBar = addToolBar(tr("File"));

	commandToolBar->addAction(serverListAction);
	commandToolBar->addAction(lanServerListAction);
	commandToolBar->addAction(stopListAction);
	commandToolBar->addAction(refreshListAction);
}

QWidget* MainWindow::createServerDetailsWidget()
{
	QPixmap flagPixmap("images/flags/64/it.png", "png", Qt::AutoColor);

	flagLabel = new QLabel();
	flagLabel->setContentsMargins(0,0,0,0);
	flagLabel->setPixmap(flagPixmap);

	QFormLayout *serverLayout = new QFormLayout();
	serverLayout->setContentsMargins(0,5,0,0);
	serverLayout->setLabelAlignment(Qt::AlignRight);
	serverNameLabel = new QLabel();
	serverLayout->addRow("Name: ", serverNameLabel);
	serverAddressLabel = new QLabel();
	serverLayout->addRow("Address: ", serverAddressLabel);
	serverPingLabel = new QLabel();
	serverLayout->addRow("Ping: ", serverPingLabel);

	QFormLayout *serverInfoLayout = new QFormLayout();
	serverInfoLayout->setLabelAlignment(Qt::AlignRight);
	serverInfoLayout->addRow(flagLabel, serverLayout);
	serverMapLabel = new QLabel();
	serverInfoLayout->addRow("Map: ", serverMapLabel);
	serverPlayersLabel = new QLabel();
	serverInfoLayout->addRow("Players: ", serverPlayersLabel);
	serverTagsLabel = new QLabel();
	serverInfoLayout->addRow("Tags: ", serverTagsLabel);
	serverSecureLabel = new QLabel();
	serverInfoLayout->addRow("Secure: ", serverSecureLabel);
	serverTypeLabel = new QLabel();
	serverInfoLayout->addRow("Type: ", serverTypeLabel);
	serverPasswordLabel = new QLabel();
	serverInfoLayout->addRow("Password: ", serverPasswordLabel);
	serverOSLabel = new QLabel();
	serverInfoLayout->addRow("Operating System: ", serverOSLabel);
	serverGameDescriptionLabel = new QLabel();
	serverInfoLayout->addRow("Game Description: ", serverGameDescriptionLabel);
	serverGameVersionLabel = new QLabel();
	serverInfoLayout->addRow("Game Version: ", serverGameVersionLabel);
	serverAppIDLabel = new QLabel();
	serverInfoLayout->addRow("App ID: ", serverAppIDLabel);
	serverTVNameLabel = new QLabel();
	serverInfoLayout->addRow("TV Name: ", serverTVNameLabel);
	serverTVPortLabel = new QLabel();
	serverInfoLayout->addRow("TV Port: ", serverTVPortLabel);
	serverSteamIDLabel = new QLabel();
	serverInfoLayout->addRow("Steam ID: ", serverSteamIDLabel);

	serverInfoWidget = new QWidget();
	serverInfoWidget->setLayout(serverInfoLayout);

	QScrollArea *scrollArea = new QScrollArea();
	scrollArea->setFrameShape(QFrame::NoFrame);
	scrollArea->setWidget(serverInfoWidget);

	QHBoxLayout *scrollAreaLayout = new QHBoxLayout();
	scrollAreaLayout->setContentsMargins(0,0,0,0);
	scrollAreaLayout->addWidget(scrollArea);

	QGroupBox *serverInfoGroupBox = new QGroupBox();
	serverInfoGroupBox->setTitle("Server Details");
	serverInfoGroupBox->setLayout(scrollAreaLayout);

	QHBoxLayout *playersLayout = new QHBoxLayout();
	playersLayout->setContentsMargins(0,0,0,0);
	playersLayout->addWidget(playersTable);

	playersGroupBox = new QGroupBox();
	playersGroupBox->setTitle("Players (0)");
	playersGroupBox->setLayout(playersLayout);

	QHBoxLayout *rulesLayout = new QHBoxLayout();
	rulesLayout->setContentsMargins(0,0,0,0);
	rulesLayout->addWidget(rulesTable);

	rulesGroupBox = new QGroupBox();
	rulesGroupBox->setTitle("Rules (0)");
	rulesGroupBox->setLayout(rulesLayout);

	QHBoxLayout *serverDetailsLayout = new QHBoxLayout();
	serverDetailsLayout->setContentsMargins(0,0,0,0);
	serverDetailsLayout->addWidget(serverInfoGroupBox);
	serverDetailsLayout->addWidget(playersGroupBox);
	serverDetailsLayout->addWidget(rulesGroupBox);
	serverDetailsLayout->setStretchFactor(serverInfoGroupBox, 20);
	serverDetailsLayout->setStretchFactor(playersGroupBox, 17);
	serverDetailsLayout->setStretchFactor(rulesGroupBox, 17);

	QWidget *serverDetailsWidget = new QWidget();
	serverDetailsWidget->setLayout(serverDetailsLayout);

	return serverDetailsWidget;
}

QWidget* MainWindow::createFiltersWidget()
{
	QLabel *ipLabel = new QLabel();
	ipLabel->setText("IP:");
	QLabel *mapLabel = new QLabel();
	mapLabel->setText("Map:");
	QLabel *nameLabel = new QLabel();
	nameLabel->setText("Name:");
	QLabel *TagsLabel = new QLabel();
	TagsLabel->setText("Tags:");
	QLabel *countryLabel = new QLabel();
	countryLabel->setText("Country:");
	QLabel *maxplayersLabel = new QLabel();
	maxplayersLabel->setText("Max Players:");
	QLabel *pingLabel = new QLabel();
	pingLabel->setText("Ping:");

	ipLineEdit = new QLineEdit();
	ipLineEdit->setMaximumWidth(95);
	ipLineEdit->setMaxLength(15);
	mapLineEdit = new QLineEdit();
	nameLineEdit = new QLineEdit();
	tagsLineEdit = new QLineEdit();

	secureCheckBox = new QCheckBox();
	secureCheckBox->setText("Secure");
	nopasswordCheckBox = new QCheckBox();
	nopasswordCheckBox->setText("No Password");
	dedicatedCheckBox = new QCheckBox();
	dedicatedCheckBox->setText("Dedicated");
	notfullCheckBox = new QCheckBox();
	notfullCheckBox->setText("Not Full");
	notemptyCheckBox = new QCheckBox();
	notemptyCheckBox->setText("Not Empty");
	nobotsCheckBox = new QCheckBox();
	nobotsCheckBox->setText("No Bots");

	countryComboBox = new QComboBox();

	pingComboBox = new QComboBox();
	pingComboBox->addItem("All", QVariant(0));
	pingComboBox->addItem("<50", QVariant(50));
	pingComboBox->addItem("<100", QVariant(100));
	pingComboBox->addItem("<150", QVariant(150));

	maxplayerSpinBox = new QSpinBox();

	QGridLayout *filtersLayout = new QGridLayout();
	filtersLayout->setContentsMargins(0, 0, 0, 0);
	filtersLayout->addWidget(ipLabel, 0, 0);
	filtersLayout->addWidget(mapLabel, 0, 2);
	filtersLayout->addWidget(nameLabel, 1, 0);
	filtersLayout->addWidget(TagsLabel, 2, 0);
	filtersLayout->addWidget(countryLabel, 0, 6);
	filtersLayout->addWidget(maxplayersLabel, 1, 6);
	filtersLayout->addWidget(pingLabel, 2, 6);

	filtersLayout->addWidget(ipLineEdit, 0, 1);
	filtersLayout->addWidget(mapLineEdit, 0, 3);
	filtersLayout->addWidget(nameLineEdit, 1, 1, 1, 3);
	filtersLayout->addWidget(tagsLineEdit, 2, 1, 1, 3);

	filtersLayout->addWidget(secureCheckBox, 2, 4);
	filtersLayout->addWidget(nopasswordCheckBox, 1, 4);
	filtersLayout->addWidget(dedicatedCheckBox, 0, 4);
	filtersLayout->addWidget(notfullCheckBox, 0, 5);
	filtersLayout->addWidget(notemptyCheckBox, 1, 5);
	filtersLayout->addWidget(nobotsCheckBox, 2, 5);

	filtersLayout->addWidget(countryComboBox, 0, 7);
	filtersLayout->addWidget(pingComboBox, 2, 7);

	filtersLayout->addWidget(maxplayerSpinBox, 1, 7);

	//filtersLayout->setColumnStretch(8, 1);

	QWidget *filtersWidget = new QWidget();
	filtersWidget->setLayout(filtersLayout);

	return filtersWidget;
}

QWidget* MainWindow::createServerListWidget()
{
	QHBoxLayout *filtersLayout = new QHBoxLayout();
	filtersLayout->addWidget(createFiltersWidget());

	QGroupBox *filtersGroupBox = new QGroupBox();
	filtersGroupBox->setTitle("Filters");
	filtersGroupBox->setLayout(filtersLayout);

	QHBoxLayout *serversTableLayout = new QHBoxLayout();
	serversTableLayout->setContentsMargins(0,0,0,0);
	serversTableLayout->addWidget(serverListTable);

	serversTableGroupBox = new QGroupBox();
	serversTableGroupBox->setTitle("Servers (0)");
	serversTableGroupBox->setLayout(serversTableLayout);

	QVBoxLayout *serverListLayout = new QVBoxLayout();
	serverListLayout->setContentsMargins(0,0,0,0);
	serverListLayout->addWidget(filtersGroupBox);
	serverListLayout->addWidget(serversTableGroupBox);

	QWidget *serverListWidget = new QWidget();
	serverListWidget->setLayout(serverListLayout);

	return serverListWidget;
}

void MainWindow::setIcon()
 {
	 QIcon SNFicon("images/snf.ico");
	 trayIcon->setIcon(SNFicon);
	 setWindowIcon(SNFicon);

	 trayIcon->setToolTip("Server Not Found");
 }

void MainWindow::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
}
