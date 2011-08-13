#ifndef _MAINGUI_H
#define	_MAINGUI_H

#include "ui_MainGUI.h"
#include "gproxy.h"
#include "thread/DownloadThread.h"
#include "thread/GproxyUpdateThread.h"
#include "statspage/Statspage.h"
#include "Player.h"
#include "Slot.h"

#include <QString>
#include <QDialog>
#include <QVector>
#include <QMouseEvent>
#include <QList>
#include <QPalette>

using namespace std;

class MainGUI : public QMainWindow
{
    Q_OBJECT

public:
    MainGUI(CGProxy *p_gproxy);
    virtual ~MainGUI();

    void init();
    CGProxy* getGproxy();
    Statspage* getStatspage();

    bool isAdmin(const QString &name);

public slots:
    void startUpdateThread();
    void addMessage(QString message, bool log = true);
    void changeChannel(QString channel);
    void addChannelUser(QString username, QString clanTag);
    void removeChannelUser(QString username);
    void clearFriendlist();
    void addFriend(QString username, bool online, QString location);
    void clearGamelist();
    void addGame(QString botname, QString gamename, QString openSlots);
    void setGameslots(QList<Slot*> slotList);
    void showErrorMessage(QString errorMessage);
    void showConfigDialog(bool exitOnReject = false);
    void playerJoined(const QString &playerName);
    void initConfigurations();
    void applyConfig();
    void setColor(const QString& area, const QPalette::ColorRole& colorRole, const QColor& color);
    void setFont(const QString& area, const QFont& font);

private:
    Ui::MainGUI widget;
    CGProxy* gproxy;
    DownloadThread* refreshButtonDownloadThread;
    DownloadThread* downloadThread;
    GproxyUpdateThread* gproxyUpdateThread;
    Statspage* statspage;
    QList<QString> admins;

    void initStatspage();
    void initLayout();
    void initSlots();
    void initAdminlist();

    void resizeEvent(QResizeEvent* event);
    void processInput(const QString& input);
    void addColor(QString &message);
    void addColor(QListWidgetItem* item);
    void sortFriendList();

private slots:
    void onClose();
    void onRestart();
    void onInputTextAreaTextChanged();
    void onTitleLabelTextChanged();
    void onChannelContextMenu(const QPoint&);
    void onFriendsContextMenu(const QPoint&);
    void onGameListItemClicked(QMouseEvent*);
    void onOutputFieldSliderMoved();
    void onRefreshButtonClicked();
    void updateRefreshButton();
    void startWarcraft();
    void onChannellistItemClicked(QMouseEvent*);
    void onFriendlistItemClicked(QMouseEvent*);
    void statspageLoginFinished();
    void receivedPlayerInformation(Player*);
    void onAdminlistReceived(QList<QString>);
};

#endif	/* _MAINGUI_H */
