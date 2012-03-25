#ifndef _MAINGUI_H
#define	_MAINGUI_H

#include "ui_MainGUI.h"
#include "gproxy.h"
#include "thread/GproxyUpdateThread.h"
#include "Statspage.h"
#include "data/Player.h"
#include "data/Slot.h"
#include "data/GameListEntry.h"
#include "widget/MColorDialog.h"
#include "GameListDownloader.h"

#include <QString>
#include <QDialog>
#include <QVector>
#include <QMouseEvent>
#include <QList>
#include <QStringList>
#include <QPalette>
#include <QKeyEvent>

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
    void addMessage(const ColoredMessage& coloredMessage, bool log = true, bool printTimestamp = true, bool lineBreak = true);
    void changeChannel(QString channel);
    void addChannelUser(QString username, QString clanTag);
    void removeChannelUser(QString username);
    void updateFriendlist(QList<Friend*> friends);
    void updateFriend(Friend* f);
    void addFriend(Friend* f);
    void removeFriend(unsigned char entryNumber);
    void setGameslots(QList<Slot*> slotList);
    void showErrorMessage(QString errorMessage);
    void showConfigDialog(bool exitOnReject = false);
    void playerJoined(const ColoredMessage& playername);
    void initConfigurations();
    void applyConfig();
    void setColor(const QString& area, const QColor& color);
    void setFont(const QString& area, const QFont& font);

protected:
    bool eventFilter(QObject* watchedObject, QEvent* event);
    
private:
    Ui::MainGUI widget;
    CGProxy* gproxy;
    GameListDownloader* gameListDownloader;
    GproxyUpdateThread* gproxyUpdateThread;
    Statspage* statspage;
    QStringList admins;
    static const int ROLE_FRIEND = 35;

    void initStatspage();
    void initLayout();
    void initSlots();
    void initAdminlist();

    void resizeEvent(QResizeEvent* event);
    void processInput(const QString& input);
    void addColor(QListWidgetItem* item);
    void addTooltip(QListWidgetItem* item);
    void addFriendInformation(Friend* f, QListWidgetItem* item);
    bool onInputTextAreaKeyPressed(QKeyEvent* event);

private slots:
    void onClose();
    void onRestart();
    void onTitleLabelTextChanged();
    void onChannelContextMenu(const QPoint&);
    void onFriendsContextMenu(const QPoint&);
    void onGameListItemClicked(QMouseEvent*);
    void onRefreshButtonClicked();
    void updateRefreshButton();
    void startWarcraft();
    void onChannellistItemClicked(QMouseEvent*);
    void onFriendlistItemClicked(QMouseEvent*);
    void statspageLoginFinished();
    void receivedPlayerInformation(Player*);
    void onAdminlistReceived(QStringList);
    void userColorChanged(MColorDialog* colorDialog);
    void onGameListDownloadFinished(QList<GameListEntry> gameList);
};

#endif	/* _MAINGUI_H */
