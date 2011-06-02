#include "MainGUI.h"
#include "gproxy.h"
#include "bnet.h"
#include "delegate/ChannellistDelegate.h"
#include "delegate/GamelistDelegate.h"
#include "socket.h"
#include "thread/DownloadThread.h"
#include "ConfigGUI.h"
#include "GhostGrazLogininformationDialog.h"

#include <QDesktopWidget>
#include <QScrollBar>
#include <QIcon>
#include <QMessageBox>
#include <QIcon>
#include <QProcess>
#include <QSound>
#include <QTimer>
#include <QFile>
#include <QDateTime>
#include <QClipboard>

MainGUI::MainGUI (CGProxy *p_gproxy)
{
    widget.setupUi(this);
    gproxy = p_gproxy;
    lastLeaver = new Player();
    lastLeaver->setName(gproxy->getUsername());
}

MainGUI::~MainGUI ()
{
    delete statspage;
    vPlayers.clear();
}

CGProxy* MainGUI::getGproxy ()
{
    return gproxy;
}

void MainGUI::onClose ()
{
    Config* config = gproxy->getConfig();
    config->setInt("width", this->width());
    config->setInt("height", this->height());
    config->commit();

    gproxy->cleanup();
}

void MainGUI::init ()
{
    Config* config = gproxy->getConfig();

    QDesktopWidget *desktop = QApplication::desktop();

    int screenWidth = desktop->width();
    int screenHeight = desktop->height();
    int appWidth = config->getInt("width");
    int appHeight = config->getInt("height");

    if (appWidth < 500 || appHeight < 200)
    {
        this->move((screenWidth - this->width()) / 2,
                (screenHeight - this->height()) / 2 - 50);
    }
    else
    {
        this->setGeometry((screenWidth - this->width()) / 2,
                (screenHeight - this->height()) / 2 - 50, appWidth, appHeight);
    }

    widget.channelList->setItemDelegate(new ChannellistDelegate(gproxy));
    widget.gameList->setItemDelegate(new GamelistDelegate());

    QAction *startWarcraftAction = widget.menuBar->addAction("Start Warcraft");
    connect(startWarcraftAction, SIGNAL(activated()),
            this, SLOT(startWarcraft()));

    initStatspage();
    initLayout();
    initSlots();
}

void MainGUI::initConnectionDialog () {
    //    connectionDialog = new QDialog(this);
    //    connectionDialog->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint
    //            | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    //    connectionDialog->setWindowTitle("Connecting...");
    //    connectionDialog->setFixedSize(400, 300);
    //    QLabel *connectionInfo = new QLabel(connectionDialog);
    //    connectionInfo->setGeometry(0, 0, connectionDialog->width(), connectionDialog->height());
}

void MainGUI::initStatspage ()
{
    statspage = new Statspage();

    Config* config = gproxy->getConfig();
    QString ghostgrazUsername = config->getString("ghostgrazUsername");
    QString ghostgrazPassword = config->getString("ghostgrazPassword");

    if (ghostgrazUsername.isEmpty() && ghostgrazPassword.isEmpty()
            && config->hasRequiredValues())
    {
        GhostGrazLogininformationDialog *dialog =
                new GhostGrazLogininformationDialog(gproxy->getConfig(), statspage);
        dialog->setModal(true);
        dialog->show();
    }
    else
    {
        statspage->login(ghostgrazUsername, ghostgrazPassword);
    }
}

void MainGUI::initLayout ()
{
    int inputFieldHeight = 60;
    int channelfieldWidth = 200;
    int channelfieldHeight = 25;
    int gamelistWidth = 220;

    widget.refreshButton->setMinimumSize(gamelistWidth, channelfieldHeight);
    widget.refreshButton->setMaximumSize(gamelistWidth, channelfieldHeight);
    widget.refreshButton->move(width() - channelfieldWidth - gamelistWidth, 0);

    widget.channelField->setMinimumSize(channelfieldWidth, channelfieldHeight);
    widget.channelField->setMaximumSize(channelfieldWidth, channelfieldHeight);
    widget.channelField->move(width() - channelfieldWidth, 0);

    widget.channelList->setMinimumSize(channelfieldWidth,
            (height() - inputFieldHeight - channelfieldHeight) / 2);
    widget.channelList->setMaximumSize(channelfieldWidth,
            (height() - inputFieldHeight - channelfieldHeight) / 2);
    widget.channelList->move(width() - channelfieldWidth, channelfieldHeight);

    widget.friendList->setMinimumSize(channelfieldWidth,
            (height() - inputFieldHeight - channelfieldHeight) / 2);
    widget.friendList->setMaximumSize(channelfieldWidth,
            (height() - inputFieldHeight - channelfieldHeight) / 2);
    widget.friendList->move(width() - channelfieldWidth, channelfieldHeight +
            (height() - inputFieldHeight - channelfieldHeight) / 2);

    widget.gameList->setMinimumSize(gamelistWidth, height() - inputFieldHeight - channelfieldHeight);
    widget.gameList->setMaximumSize(gamelistWidth, height() - inputFieldHeight - channelfieldHeight);
    widget.gameList->move(width() - channelfieldWidth - gamelistWidth, channelfieldHeight);

    widget.inputField->setMinimumSize(width(), inputFieldHeight);
    widget.inputField->setMaximumSize(width(), inputFieldHeight);
    widget.inputField->move(0, height() - inputFieldHeight);

    widget.outputField->setMinimumSize(width() - channelfieldWidth - gamelistWidth, height() - inputFieldHeight);
    widget.outputField->setMaximumSize(width() - channelfieldWidth - gamelistWidth, height() - inputFieldHeight);
    widget.outputField->move(0, 0);
}

void MainGUI::initSlots ()
{
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(onClose()), Qt::QueuedConnection);
    connect(widget.outputField->verticalScrollBar(), SIGNAL(rangeChanged(int, int)),
            this, SLOT(onOutputFieldSliderMoved()), Qt::QueuedConnection);
    connect(statspage, SIGNAL(loginFinished()),
            this, SLOT(statspageLoginFinished()), Qt::QueuedConnection);
    connect(statspage, SIGNAL(receivedPlayerInformation(Player *)),
            this, SLOT(receivedPlayerInformation(Player *)), Qt::QueuedConnection);
}

void MainGUI::resizeEvent (QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    initLayout();
}

void MainGUI::onInputFieldTextChanged ()
{
    if (widget.inputField->toPlainText().contains("\n")
            && widget.inputField->toPlainText().size() > 1)
    {
        QString input = widget.inputField->toPlainText();
        input.replace("\n", "");

        widget.inputField->setPlainText("");

        processInput(input);
    }
    else if (widget.inputField->toPlainText().toLower() == "/r "
            && !gproxy->m_BNET->GetReplyTarget().empty())
    {
        widget.inputField->setPlainText("/w "
                + QString::fromStdString(gproxy->m_BNET->GetReplyTarget()) + " ");
        widget.inputField->moveCursor(QTextCursor::End);
    }
}

void MainGUI::onChannelChanged ()
{
    QString text = widget.channelField->text();
    QFont font = widget.channelField->font();
    font.setPointSize(12);
    QFontMetrics *fm = new QFontMetrics(font);
    int width = widget.channelField->width();

    while (fm->boundingRect(text).width() >= width)
    {
        font.setPointSize(font.pointSize() - 1);
        fm = new QFontMetrics(font);
    }

    widget.channelField->setFont(font);
}

void MainGUI::onChannelContextMenu (const QPoint& pos)
{
    QPoint globalPos = widget.channelList->mapToGlobal(pos);
    QListWidgetItem* item = widget.channelList->itemAt(pos);

    if (!item)
    {
        return;
    }

    QString user = item->data(ChannellistDelegate::USER).toString();

    QMenu menu;

    if (gproxy->m_BNET->GetInGame())
    {
        if (user == "The Sentinel" || user == "The Scourge" || user.startsWith("Team "))
        {
            menu.addAction(QIcon(":/images/Play.png"), "Change team");
        }
        else if (item->data(ChannellistDelegate::SLOT_STATUS).toInt() == 2
                && item->data(ChannellistDelegate::SLOT_COMPUTER_STATUS).toInt() == 0)
        {
            menu.addAction(QIcon(":/images/Mail.png"), "Whisper");
            menu.addAction(QIcon(":/images/Add.png"), "Add as friend");
            menu.addAction(QIcon(":/images/Applications.png"), "Copy name");
            menu.addAction(QIcon(":/images/Edit.png"), "!stats");
            menu.addAction(QIcon(":/images/Edit Alt.png"), "!statsdota");
        }
    }
    else
    {
        menu.addAction(QIcon(":/images/Mail.png"), "Whisper");
        menu.addAction(QIcon(":/images/User.png"), "Whois");
        menu.addAction(QIcon(":/images/Applications.png"), "Copy name");
        menu.addAction(QIcon(":/images/Add.png"), "Add as friend");
    }

    QAction* action = menu.exec(globalPos);
    if (action)
    {
        if (action->text() == "Whisper")
        {
            widget.inputField->setPlainText("/w " + user + " ");
            widget.inputField->setFocus();
            widget.inputField->moveCursor(QTextCursor::End);
        }
        else if (action->text() == "Whois")
        {
            gproxy->m_BNET->QueueChatCommand("/whois " + user, false);
        }
        else if (action->text() == "!stats" || action->text() == "!statsdota")
        {
            gproxy->sendGamemessage(action->text() + " " + user);
        }
        else if (action->text() == "Change team")
        {
            int team;

            if (user == "The Sentinel")
            {
                team = 0;
            }
            else if (user == "The Scourge")
            {
                team = 1;
            }
            else
            {
                team = user.mid(5).toInt();
            }

            gproxy->changeTeam(team);
        }
        else if (action->text() == "Copy name")
        {
            QApplication::clipboard()->setText(user);
        }
        else if (action->text() == "Add as friend")
        {
            gproxy->m_BNET->QueueChatCommand("/friends add " + user, false);
            gproxy->m_BNET->UpdateFriendList();
        }
        else
        {
            addMessage("[ERROR] Not yet implemented!", false);
        }
    }
}

void MainGUI::onFriendsContextMenu (const QPoint& pos)
{
    QPoint globalPos = widget.friendList->mapToGlobal(pos);
    QListWidgetItem* item = widget.friendList->itemAt(pos);

    if (!item)
    {
        return;
    }

    QMenu menu;
    menu.addAction(QIcon(":/images/Mail.png"), "Whisper");
    menu.addAction(QIcon(":/images/User.png"), "Whois");
    menu.addAction(QIcon(":/images/Chat Bubble.png"), "Whisper all");
    menu.addAction(QIcon(":/images/Remove.png"), "Remove friend");

    if (gproxy->m_BNET->GetInGame())
    {
        menu.addAction(QIcon(":/images/Edit.png"), "!stats");
        menu.addAction(QIcon(":/images/Edit Alt.png"), "!statsdota");
    }

    QAction* action = menu.exec(globalPos);
    if (action)
    {
        if (action->text() == "Whisper")
        {
            widget.inputField->setPlainText("/w " + item->text() + " ");
            widget.inputField->setFocus();
            widget.inputField->moveCursor(QTextCursor::End);
        }
        else if (action->text() == "Whois")
        {
            gproxy->m_BNET->QueueChatCommand("/whois " + item->text(), false);
        }
        else if (action->text() == "Whisper all")
        {
            widget.inputField->setPlainText("/f m ");
            widget.inputField->setFocus();
            widget.inputField->moveCursor(QTextCursor::End);
        }
        else if (action->text() == "!stats" || action->text() == "!statsdota")
        {
            gproxy->sendGamemessage(action->text() + " " + item->text());
        }
        else if (action->text() == "Remove friend")
        {
            gproxy->m_BNET->QueueChatCommand("/friends remove "
                    + item->text(), false);
            gproxy->m_BNET->UpdateFriendList();
        }
        else
        {
            addMessage("[ERROR] Not yet implemented!", false);
        }
    }
}

void MainGUI::onGameListItemClicked (QMouseEvent *mouseEvent)
{
    widget.inputField->setFocus();

    QListWidgetItem *item = widget.gameList->itemAt(mouseEvent->pos());
    if (!item)
    {
        return;
    }

    QString gamename = item->data(GamelistDelegate::GAMENAME).toString();

    if (mouseEvent->button() == Qt::LeftButton && !gamename.isEmpty()
            && gamename != "-" && gamename != "in channel ghostgraz")
    {
        gproxy->m_BNET->SetSearchGameName(gamename.toStdString());
        autosearch(false);
        addMessage("[BNET] looking for a game named \"" + gamename
                + "\" for up to two minutes");
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        QApplication::clipboard()->setText(gamename);
    }
}

void MainGUI::onOutputFieldSliderMoved ()
{
    QScrollBar* vsb = widget.outputField->verticalScrollBar();
    if (vsb->value() >= (vsb->maximum() - vsb->singleStep() * 2))
    {
        vsb->setValue(vsb->maximum());
    }
}

void MainGUI::onRefreshButtonClicked ()
{
    widget.refreshButton->setEnabled(false);

    DownloadThread *dt = new DownloadThread(this);
    dt->refresh();

    widget.refreshButton->setText("Refresh (10)");
    QTimer::singleShot(1000, this, SLOT(updateRefreshButton()));
}

void MainGUI::updateRefreshButton ()
{
    QString text = widget.refreshButton->text();
    int secondsLeft = text.mid(9, text.length() - 10).toInt();

    if (secondsLeft != 1)
    {
        widget.refreshButton->setText("Refresh ("
                + QString::number(secondsLeft - 1) + ")");
        QTimer::singleShot(1000, this, SLOT(updateRefreshButton()));
    }
    else
    {
        widget.refreshButton->setText("Refresh");
        widget.refreshButton->setEnabled(true);
    }
}

void MainGUI::processInput (const QString& input)
{
    QString command = input.toLower();

    if (command == "/commands")
    {
        addMessage(">>> /commands", false);
        addMessage("", false);
        addMessage("  In the GProxy++ console:", false);
        addMessage("   /commands           : show command list", false);
        addMessage("   /exit or /quit      : close GProxy++", false);
        addMessage("   /filter <f>         : start filtering public game names for <f>", false);
        addMessage("   /filteroff          : stop filtering public game names", false);
        addMessage("   /game <gamename>    : look for a specific game named <gamename>", false);
        addMessage("   /help               : show help text", false);
        addMessage("   /public             : enable listing of public games", false);
        addMessage("   /publicoff          : disable listing of public games", false);
        addMessage("   /r <message>        : reply to the last received whisper", false);
        addMessage("   /pgn <privgamename> : sets privategamename temporary to <privgamename> (Alias: /privategamename)", false);
        addMessage("   /waitgame <option>  : waits for a bot joining channel with the name-prefix set in gproxy.cfg and hosts", false);
        addMessage("                         a private game named <value of privategamename> (Alias: /wg <option>", false);
        addMessage("   Optons:   quiet     : doesn't display the message to other users in the channel (Alias: q)", false);
        addMessage("             off       : stops waiting for a bot to host a game", false);
        addMessage("", false);
        addMessage("   /botprefix <prefix> : sets <value of botprefix> temporary to <prefix>", false);
        addMessage("   /parrot <plname>    : repeats anything that Player <plname> sais in Chat with [PARROT] <Player's message>", false);
        addMessage("   /parrotall          : repeats anything that Players say in Chat with [PARROT] <Player's message>", false);
        addMessage("   /parrotoff          : stops /parrot and /parrotall", false);
#ifdef WIN32
        addMessage("   /start              : start warcraft 3", false);
#endif
        addMessage("   /version            : show version text", false);
        addMessage("", false);
        addMessage("  In game:", false);
        addMessage("   /re <message>       : reply to the last received whisper", false);
        addMessage("   /sc                 : whispers \"spoofcheck\" to the game host", false);
        addMessage("   /status             : show status information", false);
        addMessage("   /w <user> <message> : whispers <message> to <user>", false);
        addMessage("", false);
    }
    else if (command == "/exit" || command == "/quit")
    {
        close();
        return;
    }
    else if (command.startsWith("/friends ") || command.startsWith("/f "))
    {
        gproxy->m_BNET->QueueChatCommand(input);
        gproxy->m_BNET->UpdateFriendList();
    }
    else if (command == "/filteroff" || command == "/filter off")
    {
        gproxy->m_BNET->SetPublicGameFilter(string());
        addMessage("[BNET] stopped filtering public game names");
    }
    else if (command.length() >= 9 && command.startsWith("/filter "))
    {
        QString filter = input.mid(8);

        if (!filter.isEmpty())
        {
            filter = filter.left(31);
            gproxy->m_BNET->SetPublicGameFilter(filter.toStdString());
            addMessage("[BNET] started filtering public game names for \"" + filter + "\"");
        }
    }
    else if (command.startsWith("/priv ") && command.length() >= 7
            && !gproxy->getPrivategamename().isEmpty())
    {
        QString botname = input.mid(6);
        if (!botname.isEmpty() && botname.length() <= 15)
        {
            QString pgn = gproxy->getPrivategamename();
            if (botname.length() > 2)
            {
                gproxy->m_BNET->QueueChatCommand("/w " + botname + " !priv " + pgn);
            }
            else
            {
                gproxy->m_BNET->QueueChatCommand("/w " + gproxy->getBotprefix()
                        + botname + " !priv " + pgn);
            }
            gproxy->m_BNET->SetSearchGameName(pgn.toStdString());
            autosearch(true);
            addMessage("[BNET] try to create a private game named ["
                    + pgn + "] at Bot ["
                    + botname + "].");
        }
    }
    else if (command.startsWith("/autosearch") || command.startsWith("/as"))
    {
        if (command == "/autosearch" || command == "/as")
        {
            if (gproxy->autosearch)
            {
                addMessage("[Pr0gm4n] 'autosearch' is on.");
            }
            else
            {
                addMessage("[Pr0gm4n] 'autosearch' is off.");
            }
        }
        else if (command == "/autosearch on" || command == "/as on")
        {
            autosearch(true);
            addMessage("[Pr0gm4n] Autosearch enabled.");
        }
        else if (command == "/autosearch off" || command == "/as off")
        {
            autosearch(false);
            addMessage("[Pr0gm4n] Autosearch disabled.");
        }
        else
        {
            addMessage("[Pr0gm4n] Invalid input.");
        }
    }
    else if (command == "/privategamename" || command == "/pgn")
    {
        addMessage("[Phyton] The value of 'privategamename' is: ["
                + gproxy->getPrivategamename() + "]. Change it with the input "
                + "'/privategamename <value>' alias '/pgn <value>'.");
    }
    else if (command.startsWith("/privategamename ") || command.startsWith("/pgn "))
    {
        if (command.startsWith("/privategamename "))
        {
            gproxy->setPrivategamename(input.mid(17, (input.length() - 17)));
        }
        else
        {
            gproxy->setPrivategamename(input.mid(5, (input.size() - 5)));
        }

        addMessage("[Phyton] Change value of 'privategamename' to ["
                + gproxy->getPrivategamename() + "].");

    }
    else if (command == "/botprefix")
    {
        addMessage("[Phyton] Value of 'botprefix' is ["
                + gproxy->getBotprefix()
                + "]. You can change it with '/botprefix <value>'.");
    }
    else if (command.startsWith("/botprefix "))
    {
        gproxy->setBotprefix(input.mid(11, input.size() - 11));
        addMessage("[Phyton] Change value of 'botprefix' to ["
                + gproxy->getBotprefix() + "].");
    }
    else if (command.startsWith("/wg"))
    {
        if (command == "/wg")
        {
            gproxy->setVShallCreateQuiet(false);
            gproxy->setVShallCreate(true);
            addMessage("[Phyton] waiting for a bot and then trying to create a game "
                    "and say [gn: " + gproxy->getPrivategamename() + "].");
            gproxy->m_BNET->QueueChatCommand("[GProxy++][Phyton] Waiting for a bot "
                    "and create a game with the name ["
                    + gproxy->getPrivategamename() + "].");
        }
        else if (command == "/wg off")
        {
            addMessage("[Phyton] stopped waiting for a bot to create a game.");
            gproxy->setVShallCreate(false);
        }
        else if (command == "/wg quiet" || command == "/wg q")
        {
            gproxy->setVShallCreate(true);
            gproxy->setVShallCreateQuiet(true);
            addMessage("[Phyton] waiting for a bot and then trying to create a game [quiet].");
        }
    }
    else if (command.startsWith("/waitgame") && gproxy->getBotprefix() != "")
    {
        if (command == "/waitgame")
        {
            gproxy->setVShallCreateQuiet(false);
            gproxy->setVShallCreate(true);
            addMessage("[Phyton] waiting for a bot and then trying to create a game "
                    "and say [gn: " + gproxy->getPrivategamename() + "].");
            gproxy->m_BNET->QueueChatCommand("[GProxy++][Phyton] Waiting for a bot "
                    "and create a game with the name ["
                    + gproxy->getPrivategamename() + "].");
        }
        else if (command == "/waitgame off")
        {
            addMessage("[Phyton] stopped waiting for a bot to create a game.");
            gproxy->setVShallCreate(false);
        }
        else if (command == "/waitgame quiet")
        {
            gproxy->setVShallCreate(true);
            gproxy->setVShallCreateQuiet(true);
            addMessage("[Phyton] waiting for a bot and then trying to create a game [quiet].");
        }
    }
    else if (command.startsWith("/parrot "))
    {
        gproxy->setParrot(input.mid(8, input.length() - 8));
        addMessage("[Phyton] Parrot [" + gproxy->getParrot() + "]!!!111");
    }
    else if (command == "/parrotoff")
    {
        gproxy->setParrot("");
        addMessage("[Phyton] Parrot off");
    }
    else if (input == "/parrotall")
    {
        gproxy->setParrot("Parrot parrot");
    }
    else if (input == "/parrotignore")
    {
        gproxy->setParrot("Ignore ignore");
    }
    else if (command.length() >= 7 && command.startsWith("/game "))
    {
        string GameName = input.mid(6).toStdString();

        if (!GameName.empty() && GameName.size() <= 31)
        {
            gproxy->m_BNET->SetSearchGameName(GameName);
            autosearch(false);
            addMessage("[BNET] looking for a game named \"" + QString::fromStdString(GameName)
                    + "\" for up to two minutes");
        }
    }
    else if (command == "/help")
    {
        addMessage(">>> /help", false);
        addMessage("", false);
        addMessage("  GProxy++ connects to battle.net and looks for games for you to join.", false);
        addMessage("  If GProxy++ finds any they will be listed on the Warcraft III LAN screen.", false);
        addMessage("  To join a game, simply open Warcraft III and wait at the LAN screen.", false);
        addMessage("  Standard games will be white and GProxy++ enabled games will be blue.", false);
        addMessage("  Note: You must type \"/public\" to enable listing of public games.", false);
        addMessage("", false);
        addMessage("  If you want to join a specific game, type \"/game <gamename>\".", false);
        addMessage("  GProxy++ will look for that game for up to two minutes before giving up.", false);
        addMessage("  This is useful for private games.", false);
        addMessage("", false);
        addMessage("  Please note:", false);
        addMessage("  GProxy++ will join the game using your battle.net name, not your LAN name.", false);
        addMessage("  Other players will see your battle.net name even if you choose another name.", false);
        addMessage("  This is done so that you can be automatically spoof checked.", false);
        addMessage("", false);
        addMessage("  Type \"/commands\" for a full command list.", false);
        addMessage("", false);
    }
    else if (command == "/public" || command == "/publicon" || command == "/public on"
            || command == "/list" || command == "/liston" || command == "/list on")
    {
        gproxy->m_BNET->SetListPublicGames(true);
        addMessage("[BNET] listing of public games enabled");
    }
    else if (command == "/publicoff" || command == "/public off"
            || command == "/listoff" || command == "/list off")
    {
        gproxy->m_BNET->SetListPublicGames(false);
        addMessage("[BNET] listing of public games disabled");
    }
    else if (command.length() >= 4 && command.startsWith("/r "))
    {
        if (!gproxy->m_BNET->GetReplyTarget().empty())
        {
            gproxy->m_BNET->QueueChatCommand(input.mid(3),
                    gproxy->m_BNET->GetReplyTarget(), true);
        }
        else
            addMessage("[BNET] nobody has whispered you yet");
    }
#ifdef WIN32
    else if (command == "/start")
    {
        startWarcraft();
    }
#endif
    else if (command == "/version")
    {
        addMessage("[GPROXY] Customized GProxy++ Version " + QString::fromStdString(gproxy->m_Version));
        addMessage("[GPROXY] <font color=\"#e6e6e6\">"
                "This mod is by <font color=\"darkgreen\">Phyton</font>, "
                "<font color=\"deeppink\">Pr0gm4n</font> and "
                "<font color=\"gold\">Manufactoring</font>.</font>");
    }
    else if (command == "/test")
    {
        // Isn't it obvious?
    }
    else if (command.startsWith("/p ") || command.startsWith("/phrase "))
    {
        QString filePath = "phrase/";

        if (command.startsWith("/p "))
        {
            filePath.append(command.mid(3));
        }
        else if (command.startsWith("/phrase "))
        {
            filePath.append(command.mid(8));
        }

        if (!filePath.endsWith(".txt"))
        {
            filePath.append(".txt");
        }

        QFile* phraseFile = new QFile(filePath);

        if (phraseFile->open(QFile::ReadOnly))
        {
            QStringList lines = QString(phraseFile->readAll()).split("\n");

            foreach(QString line, lines)
            {
                if (gproxy->m_BNET->GetInGame() && !line.startsWith("#"))
                {
                    if (!line.startsWith("%"))
                    {
                        if (!line.startsWith("~"))
                        {
                            gproxy->sendGamemessage(line);
                        }
                        else
                        {
                            // Sleep
                        }
                    }
                    else
                    {
                        gproxy->m_BNET->QueueChatCommand(line.mid(1));
                    }
                }
                else if (!line.startsWith("#"))
                {
                    gproxy->m_BNET->QueueChatCommand(line);
                }
            }
        }
        else
        {
            CONSOLE_Print("[ERROR] File \"" + filePath + "\" does not exist!");
            gproxy->SendLocalChat(string() + "File \"" + filePath.toStdString() + "\" does not exist!");
        }
    }
    else if (gproxy->m_BNET->GetInGame())
    {
        if (command == "/statslast" || command == "/sl"
                || command == "!statslast" || command == "!sl")
        {
            gproxy->sendGamemessage("!stats " + lastLeaver->getName());
        }
        else if (command.startsWith("/allies "))
        {
            gproxy->sendGamemessage(input.mid(8), true);
        }
        else
        {
            gproxy->sendGamemessage(input);
        }
    }
    else
    {
        gproxy->m_BNET->QueueChatCommand(input);
    }
}

void MainGUI::addMessage (QString message, bool log)
{
    if (log)
    {
        LOG_Print(message);
    }

    addColor(message);

    QString dateTime = QLocale().toString(QDateTime::currentDateTime(), "[hh:mm:ss] ");
    message.insert(message.indexOf(">") + 1, dateTime);

    widget.outputField->append(message);
}

void MainGUI::addColor (QString &message)
{
    if (message.startsWith("[WHISPER]")
            || message.startsWith("[QUEUED] /w ")
            || message.startsWith("[WHISPER TO]"))
    {
        message.prepend("<font color=\"Lime\">");
        message.append("</font>");
    }
    else if (message.startsWith("[Phyton]")
            || message.startsWith("[Pr0gm4n]")
            || message.startsWith("[CONFIG]")
            || message.startsWith("Bot ["))
    {
        message.prepend("<font color=\"DarkGreen\">");
        message.append("</font>");
    }
    else if (message.startsWith("[Manufactoring]"))
    {
        message.prepend("<font color=\"Gold\">");
        message.append("</font>");
    }
    else if (message.startsWith("[LOCAL]"))
    {
        // removing "[LOCAL] "
        message.remove(0, 8);

        // special coloring only for the chiefs
        if (message.startsWith("[baerli_graz]")
                || message.startsWith("[klingone_graz]"))
        {
            message.insert(1, "<font color=\"red\">");
            message.insert(message.indexOf("]"), "</font>");
        }

        message.prepend("<font color=\"#e6e6e6\">");
        message.append("</font>");
    }
    else if (message.startsWith("[QUEUED]"))
    {
        // replacing "QUEUED" with the username
        message = "[" + QString::fromStdString(gproxy->m_BNET->m_UserName) + "]"
                + message.mid(8, message.size() - 8);

        // special coloring only for the chiefs
        if (message.startsWith("[baerli_graz]")
                || message.startsWith("[klingone_graz]"))
        {
            message.insert(1, "<font color=\"red\">");
            message.insert(message.indexOf("]"), "</font>");
        }

        message.prepend("<font color=\"#e6e6e6\">");
        message.append("</font>");
    }
    else if (message.startsWith("[INFO]"))
    {
        message.remove(0, 7);
        message.prepend("<font color=\"Cyan\">");
        message.append("</font>");
    }
    else if (message.startsWith("[TCPSOCKET]"))
    {
        message.prepend("<font color=\"Indigo\">");
        message.append("</font>");
    }
    else if (message.startsWith("[GPROXY]")
            || message.startsWith("[UDPSOCKET]"))
    {
        message.prepend("<font color=\"Indigo\">");
        message.append("</font>");
    }
    else if (message.startsWith("[BNET]"))
    {
        message.prepend("<font color=\"DarkRed\">");
        message.append("</font>");
    }
    else if (message.startsWith("[EMOTE]"))
    {
        message.remove(0, 8);
        message.prepend("<font color=\"Gray\">");
        message.append("</font>");
    }
    else if (message.endsWith(" has joined the game.")
            || message.endsWith(" has left the game."))
    {
        message.prepend("<font color=\"Gold\">");
        message.append("</font>");
    }
    else if (message.startsWith("[ERROR]")
            || message.startsWith("[Manufactoring][ERROR]"))
    {
        message.prepend("<font color=\"Red\">");
        message.append("</font>");
    }
    else if (message.startsWith("[WARNING]"))
    {
        message.prepend("<font color=\"OrangeRed\">");
        message.append("</font>");
    }
    else
    {
        message.prepend("<font color=\"#e6e6e6\">");
        message.append("</font>");
    }
}

void MainGUI::addColor (QListWidgetItem *item)
{
    QString username = item->data(ChannellistDelegate::USER).toString();

    if (username == "baerli_graz" || username == "klingone_graz")
    {
        item->setData(ChannellistDelegate::COLOR_USER, QColor(255, 0, 0));
        item->setToolTip("Creator of GhostGraz");
    }
    else if (username.startsWith("GhostGraz"))
    {
        item->setData(ChannellistDelegate::COLOR_USER, QColor(255, 255, 0));
        item->setToolTip("GhostGraz bot");
    }
    else if (username == "Phyton" || username == "Manufactoring")
    {

        item->setData(ChannellistDelegate::COLOR_USER, QColor(0, 255, 0));
        item->setToolTip("Creator of GProxy GhostGraz");
    }
}

void MainGUI::sortChannelList ()
{
    for (int i = widget.channelList->count() - 1; i >= 0; i--)
    {
        QListWidgetItem *item = widget.channelList->item(i);
        if (item->data(ChannellistDelegate::USER).toString() == "baerli_graz"
                || item->data(ChannellistDelegate::USER).toString() == "klingone_graz"
                || item->data(ChannellistDelegate::USER).toString().startsWith("GhostGraz")
                || item->data(ChannellistDelegate::USER).toString() == "Phyton"
                || item->data(ChannellistDelegate::USER).toString() == "Manufactoring")
        {

            QListWidgetItem *newItem = new QListWidgetItem();
            newItem->setData(ChannellistDelegate::USER,
                    item->data(ChannellistDelegate::USER));
            newItem->setData(ChannellistDelegate::CLAN_TAG,
                    item->data(ChannellistDelegate::CLAN_TAG));
            addColor(newItem);
            delete widget.channelList->takeItem(i);
            widget.channelList->insertItem(0, newItem);
        }
    }
}

void MainGUI::sortFriendList ()
{
    for (int i = widget.friendList->count() - 1; i >= 0; i--)
    {
        QListWidgetItem *item = widget.friendList->item(i);
        if (item->foreground() == Qt::green)
        {

            QListWidgetItem *newItem = new QListWidgetItem();
            newItem->setText(item->text());
            newItem->setForeground(Qt::green);
            delete widget.friendList->takeItem(i);
            widget.friendList->insertItem(0, newItem);
        }
    }
}

void MainGUI::addChannelUser (QString username, QString clanTag)
{
    for (int i = 0; i < widget.channelList->count(); i++)
    {
        QListWidgetItem *item = widget.channelList->item(i);
        if (item->data(ChannellistDelegate::USER).toString() == username)
        {
            if (item->data(ChannellistDelegate::CLAN_TAG).toString() == clanTag)
            {
                return;
            }
            else
            {
                // Update
                QListWidgetItem *newItem = new QListWidgetItem();
                newItem->setData(ChannellistDelegate::USER, username);
                newItem->setData(ChannellistDelegate::CLAN_TAG, clanTag);
                addColor(newItem);
                delete widget.channelList->takeItem(i);
                widget.channelList->addItem(newItem);
                //                sortChannelList();
                return;
            }
        }
    }

    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setData(ChannellistDelegate::USER, username);
    if (!clanTag.isEmpty())
    {

        newItem->setData(ChannellistDelegate::CLAN_TAG, clanTag);
    }
    addColor(newItem);
    widget.channelList->addItem(newItem);
    //    sortChannelList();
}

void MainGUI::removeChannelUser (QString username)
{
    for (int i = 0; i < widget.channelList->count(); i++)
    {
        QListWidgetItem *item = widget.channelList->item(i);
        if (item->data(ChannellistDelegate::USER).toString() == username)
        {
            item = widget.channelList->takeItem(i);
            delete item;

            return;
        }
    }
}

void MainGUI::changeChannel (QString channel)
{

    widget.channelList->clear();
    widget.channelField->setText(channel);
}

void MainGUI::clearFriendlist ()
{

    widget.friendList->clear();
}

void MainGUI::addFriend (QString username, bool online, QString location)
{
    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setText(username);
    newItem->setToolTip(location);

    if (online)
    {
        newItem->setForeground(Qt::green);
    }
    else
    {

        newItem->setForeground(Qt::red);
    }

    widget.friendList->addItem(newItem);
    sortFriendList();
}

void MainGUI::addGame (QString botname, QString gamename, QString openSlots)
{

    QListWidgetItem* newItem = new QListWidgetItem();
    newItem->setData(GamelistDelegate::BOTNAME, botname);
    newItem->setData(GamelistDelegate::GAMENAME, gamename);
    newItem->setData(GamelistDelegate::OPEN_SLOTS, openSlots);
    widget.gameList->addItem(newItem);
}

void MainGUI::clearGamelist ()
{

    widget.gameList->clear();
}

void MainGUI::setGameslots (vector<CIncomingSlots *> slotList)
{
    widget.channelList->clear();

    QVector<int> vTeamNumbers;

    for (vector<CIncomingSlots *>::iterator it = slotList.begin(); it != slotList.end(); it++)
    {
        if (!gproxy->isDotaMap())
        {
            if (!vTeamNumbers.contains((*it)->GetTeam()))
            {
                vTeamNumbers.push_back((*it)->GetTeam());
            }
        }

        QListWidgetItem *itemSlot = new QListWidgetItem();
        itemSlot->setData(ChannellistDelegate::SLOT_PID, (*it)->GetPID());
        itemSlot->setData(ChannellistDelegate::SLOT_DOWNLOAD_STATUS, (*it)->GetDownloadStatus());
        itemSlot->setData(ChannellistDelegate::SLOT_STATUS, (*it)->GetSlotStatus());
        itemSlot->setData(ChannellistDelegate::SLOT_COMPUTER_STATUS, (*it)->GetComputerStatus());
        itemSlot->setData(ChannellistDelegate::SLOT_TEAM, (*it)->GetTeam());
        itemSlot->setData(ChannellistDelegate::SLOT_COLOR, (*it)->GetColor());
        itemSlot->setData(ChannellistDelegate::SLOT_RACE, (*it)->GetRace());
        itemSlot->setData(ChannellistDelegate::SLOT_COMPUTER_TYPE, (*it)->GetComputerType());
        itemSlot->setData(ChannellistDelegate::SLOT_HANDICAP, (*it)->GetHandicap());
        itemSlot->setData(ChannellistDelegate::USER, QString::fromStdString((*it)->GetName()));
        widget.channelList->addItem(itemSlot);
    }

    if (!gproxy->isDotaMap())
    {
        for (int i = 0; i < vTeamNumbers.count(); i++)
        {
            QListWidgetItem *itemTeam = new QListWidgetItem();
            QString teamString = QString("Team ").append(QString::number(i + 1));
            itemTeam->setData(ChannellistDelegate::USER, teamString);
            widget.channelList->insertItem(i, itemTeam);
        }

        sortSlots(vTeamNumbers.count());
    }
    else
    {

        QListWidgetItem *itemSentinel = new QListWidgetItem();
        itemSentinel->setData(ChannellistDelegate::USER, "The Sentinel");
        itemSentinel->setData(ChannellistDelegate::COLOR_USER, QColor(255, 0, 0));
        widget.channelList->insertItem(0, itemSentinel);

        QListWidgetItem *itemScourge = new QListWidgetItem();
        itemScourge->setData(ChannellistDelegate::USER, "The Scourge");
        itemScourge->setData(ChannellistDelegate::COLOR_USER, QColor(0, 255, 0));
        widget.channelList->insertItem(1, itemScourge);

        sortSlots(2);
    }
}

void MainGUI::sortSlots (int teams)
{
    for (int i = widget.channelList->count() - 1; i >= teams; i--)
    {

        QListWidgetItem *sortedItem =
                widget.channelList->item(widget.channelList->count() - 1)->clone();
        delete widget.channelList->takeItem(widget.channelList->count() - 1);
        widget.channelList->insertItem(sortedItem->data(
                ChannellistDelegate::SLOT_TEAM).toInt() + 1, sortedItem);
    }
}

void MainGUI::startWarcraft ()
{
    QString exePath;

    if (!gproxy->getCDKeyTFT().isEmpty())
    {
        exePath = gproxy->getWar3Path() + "Frozen Throne.exe";
    }
    else
    {
        exePath = gproxy->getWar3Path() + "Warcraft III.exe";
    }

    QProcess *wc3Process = new QProcess(this);
    wc3Process->start(exePath, QStringList());

    if (!wc3Process->waitForStarted(1000))
    {
        showErrorMessage(wc3Process->errorString());
    }
}

void MainGUI::showErrorMessage (QString errorMessage)
{
    addMessage("[ERROR] " + errorMessage);

    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/images/Error.png"));
    msgBox.setWindowTitle("Error");
    msgBox.setText(errorMessage);
    msgBox.exec();
}

void MainGUI::showConfigDialog ()
{
    ConfigGUI *config = new ConfigGUI(gproxy->getConfig());
    config->exec();
}

void MainGUI::statspageLoginFinished ()
{
    if (statspage->isLoggedIn())
    {
        addMessage("[GPROXY] Statspage available.");
    }
    else
    {
        addMessage("[ERROR] Statspage unavailable. You might have entered wrong username or password.");
    }
}

void MainGUI::playerJoined (const QString& playerName)
{
    addMessage("[LOBBY] " + playerName + " has joined the game.");
    vPlayers.append(new Player(playerName));
    statspage->getPlayerInformation(playerName);

    for (int i = 0; i < widget.friendList->count(); i++)
    {
        if (widget.friendList->item(i)->text() == playerName)
        {
            QSound::play("sounds/vip_joins.wav");
            break;
        }
    }
}

void MainGUI::playerLeft (const QString& playerName)
{
    for (int i = 0; i < vPlayers.count(); i++)
    {
        if (vPlayers.at(i)->getName() == playerName)
        {
            lastLeaver = vPlayers.at(i);
            vPlayers.remove(i);
            break;
        }
    }
}

void MainGUI::receivedPlayerInformation (Player *player)
{

    foreach(Player *p, vPlayers)
    {
        if (p->getName() == player->getName())
        {
            p = player;
            if (player->getGamesPlayed() < 20)
            {
                addMessage("[WARNING] " + player->getName()
                        + " has only played " + QString::number(player->getGamesPlayed())
                        + " games until now. (Stay ratio: "
                        + QString::number(player->getStayPercent(), 'f', 2) + "%)");
                gproxy->SendLocalChat("WARNING: " + player->getName().toStdString()
                        + " has only played " + QString::number(player->getGamesPlayed()).toStdString()
                        + " games until now. (Stay ratio: "
                        + QString::number(player->getStayPercent(), 'f', 2).toStdString() + "%)");
            }
            else if (player->getStayPercent() < 80)
            {
                addMessage("[WARNING] " + player->getName()
                        + " has a stay ratio below 80%. ("
                        + QString::number(player->getStayPercent(), 'f', 2) + "%)");
                gproxy->SendLocalChat("WARNING: " + player->getName().toStdString()
                        + " has a stay ratio below 80%. ("
                        + QString::number(player->getStayPercent(), 'f', 2).toStdString() + "%)");
            }
        }
    }
}

Statspage* MainGUI::getStatspage ()
{
    return statspage;
}

Player* MainGUI::getLastLeaver ()
{
    return lastLeaver;
}

void MainGUI::onChannellistItemClicked (QMouseEvent *mouseEvent)
{
    widget.inputField->setFocus();

    QListWidgetItem *item = widget.channelList->itemAt(mouseEvent->pos());
    if (!item)
    {
        return;
    }

    if (mouseEvent->button() == Qt::LeftButton)
    {
        widget.inputField->setPlainText("/w "
                + item->data(ChannellistDelegate::USER).toString() + " ");
        widget.inputField->moveCursor(QTextCursor::End);
    }
}

void MainGUI::onFriendlistItemClicked (QMouseEvent *mouseEvent)
{
    widget.inputField->setFocus();

    QListWidgetItem *item = widget.friendList->itemAt(mouseEvent->pos());
    if (!item)
    {
        return;
    }

    if (mouseEvent->button() == Qt::LeftButton)
    {
        widget.inputField->setPlainText("/w " + item->text() + " ");
        widget.inputField->moveCursor(QTextCursor::End);
    }
}