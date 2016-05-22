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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QThread>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTreeWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QToolBar>
#include <QTime>
#include <QLabel>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#include <QScrollArea>

#include "Global.h"
#include "ValveInterface.h"
#include "ValveGame.h"
#include "Favorites.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	QThread *interfaceThread;

	ValveInterface *vi;

	QList<ServerInfo*> serverList;

	Favorites favorites;
	Game *currentGame;

	//filters
	QString m_ip;
	QString m_map;
	QString m_name;
	QString m_tags;
	bool m_dedicated;
	bool m_noPassword;
	bool m_secure;
	bool m_notFull;
	bool m_notEmpty;
	bool m_noBots;
	int m_ping;
	int m_maxPlayers;

	//GUI
	QAction *aboutQtAction;
	QAction *serverListAction;
	QAction *lanServerListAction;
	QAction *stopListAction;
	QAction *refreshListAction;
	QAction *minimizeAction;
	QAction *maximizeAction;
	QAction *restoreAction;
	QAction *quitAction;
	QAction *addToFavoritesAction;
	QAction *editSettingsAction;

	QLabel *serverAddressLabel;
	QLabel *serverPingLabel;
	QLabel *serverNameLabel;
	QLabel *serverMapLabel;
	QLabel *serverPlayersLabel;
	QLabel *serverTagsLabel;
	QLabel *serverSecureLabel;
	QLabel *serverTypeLabel;
	QLabel *serverPasswordLabel;
	QLabel *serverOSLabel;
	QLabel *serverGameDescriptionLabel;
	QLabel *serverGameVersionLabel;
	QLabel *serverAppIDLabel;
	QLabel *serverTVNameLabel;
	QLabel *serverTVPortLabel;
	QLabel *serverSteamIDLabel;
	QLabel *flagLabel;

	QGroupBox *playersGroupBox;
	QGroupBox *rulesGroupBox;
	QGroupBox *serversTableGroupBox;

	QTableWidget *serverListTable;
	QTableWidget *playersTable;
	QTableWidget *rulesTable;

	QStatusBar *statusBar;
	QTreeWidget *gameTree;

	QWidget* createServerDetailsWidget();
	QWidget* createFiltersWidget();
	QWidget* createServerListWidget();

	//TODO: test
	QWidget *serverInfoWidget;
	//

	QLineEdit *ipLineEdit;
	QLineEdit *mapLineEdit;
	QLineEdit *nameLineEdit;
	QLineEdit *tagsLineEdit;

	QComboBox *pingComboBox;
	QComboBox *countryComboBox;

	QSpinBox *maxplayerSpinBox;

	QCheckBox *secureCheckBox;
	QCheckBox *nopasswordCheckBox;
	QCheckBox *dedicatedCheckBox;
	QCheckBox *notfullCheckBox;
	QCheckBox *notemptyCheckBox;
	QCheckBox *nobotsCheckBox;

	QSystemTrayIcon *trayIcon;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *helpMenu;
	QMenu *trayIconMenu;
	QMenu *serverListContextMenu;

	void drawGUI();
	void setIcon();
	void createActions();
	void createMenus();
	void createToolBars();
	void createButtons();
	void createGameTree();
	void createServerList();
	void createPlayerList();
	void createRuleList();
	void createTrayIcon();

	void refreshServerInfo();
	bool checkFilters(ServerInfo info);
	void addServerToTable(ServerInfo *info);

	void closeEvent(QCloseEvent *event);

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void getList();
	void getLANList();
	void stopList();
	void refreshList();
	void serverInfo(ServerInfo *info);
	void serverInfoUI(ServerInfo *info);
	void playerInfo(QList<PlayerInfo> playerList);
	void ruleInfo(QList<Rule> ruleList);
	void pingInfo(qint32 msec);
	void finelista(quint32 server_count);
	void aboutQt();
	void serverListClicked(int row, int column);
	void serverListDoubleClicked(int row, int column);
	void trayActivated(QSystemTrayIcon::ActivationReason reason);
	void serverListContextMenuEvent(QPoint pos);
	void addToFavorites();
	void editSettings();
};

#endif // MAINWINDOW_H
