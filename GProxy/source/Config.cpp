#include "Config.h"
#include "util/Util.h"

#include <QStringList>
#include <QTextStream>
#include <QMetaObject>
#include <QMetaEnum>

using namespace std;

Config::Config (QString config)
{
    configFile = new QFile(config);
}

Config::~Config ()
{
    if(configFile->isOpen())
    {
        configFile->close();
    }
    delete configFile;
}

int Config::loadConfig ()
{
    if (configFile->open(QFile::ReadWrite))
    {
        keys.clear();
        values.clear();
        addKeys();
        addValues(QString(configFile->readAll()));
        configFile->close();

        if (hasRequiredValues())
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 2;
    }
}

void Config::addKeys ()
{
    // BEWARE: Everything that ends with "_foregroundcolor" is considered to be a foregroundcolor.
    keys.append("# Connection");
    keys.append("war3path");
    keys.append("cdkeyroc");
    keys.append("cdkeytft");
    keys.append("server");
    keys.append("username");
    keys.append("password");
    keys.append("channel");
    keys.append("war3version");
    keys.append("port");
    keys.append("cdkeysowner");

    keys.append("# Player versus Player Gaming Network");
    keys.append("exeversion");
    keys.append("exeversionhash");
    keys.append("passwordhashtype");

    keys.append("# Miscellaneous");
    keys.append("ghostgrazUsername");
    keys.append("ghostgrazPassword");
    keys.append("sound");
    keys.append("joinstats");
    keys.append("privategamename");
    keys.append("botprefix");
    keys.append("log");

    keys.append("# Application config values");
    keys.append("width");
    keys.append("height");
    keys.append("botorder");

    keys.append("# Appearance");
    keys.append("backgroundcolor");
    keys.append("outputareaFont");

    keys.append("# Foregroundcolors");
    keys.append("default_foregroundcolor");
    keys.append("whisper_foregroundcolor");
    keys.append("gameinfo_foregroundcolor");
    keys.append("gproxy_foregroundcolor");
    keys.append("bnet_foregroundcolor");
    keys.append("info_foregroundcolor");
    keys.append("emote_foregroundcolor");
    keys.append("error_foregroundcolor");
    keys.append("warning_foregroundcolor");

    keys.append("# Customized user colors");
    keys.append("usercolor");
}

void Config::addValues (QString content)
{
    QList<QString> vTempKey;
    QList<QString> vTempValue;

    QStringList lines = content.split("\n");
    foreach (QString line, lines)
    {
        QString key = line.mid(0, line.indexOf("="));
        key = key.trimmed();

        QString value = line.mid(line.indexOf("=") + 1);
        value = value.trimmed();

        vTempKey.append(key);
        vTempValue.append(value);
    }

    for (int i = 0; i < keys.count(); i++)
    {
        if (!vTempKey.contains(keys.at(i)))
        {
            addDefaultValue(keys.at(i));
            continue;
        }

        // Load config values
        for (int j = 0; j < vTempKey.count(); j++)
        {
            if (keys.at(i) == vTempKey.at(j))
            {
                values.append(vTempValue.at(j));
            }
        }
    }
}

/**
 * Adds default config values if the key is not present at startup.
 *
 * @param key - The non existing key.
 */
void Config::addDefaultValue (const QString &key)
{
    if (key == "war3version")
    {
        values.append("26");
    }
    else if (key == "server")
    {
        values.append("europe.battle.net");
    }
    else if (key == "channel")
    {
        values.append("Clan Graz");
    }
    else if (key == "port")
    {
        values.append("6125");
    }
    else if (key == "cdkeysowner")
    {
        values.append("GProxy");
    }
    else if (key == "sound")
    {
        values.append("Enabled");
    }
    else if (key == "joinstats")
    {
        values.append("Enabled");
    }
    else if (key == "privategamename")
    {
        values.append("inhouse");
    }
    else if (key == "botprefix")
    {
        values.append("GhostGraz");
    }
    else if (key == "log")
    {
        values.append("Enabled");
    }
    else if(key == "backgroundcolor")
    {
        values.append("0,0,0");
    }
    else if(key == "outputareaForegroundcolor")
    {
        values.append("255,255,255");
    }
    else if(key == "outputareaFont")
    {
        values.append(QFont("Arial", 9, QFont::Normal).toString());
    }
    else if(key == "default_foregroundcolor")
    {
        values.append("240,240,240");
    }
    else if(key == "whisper_foregroundcolor")
    {
        values.append("0,255,0");
    }
    else if(key == "gameinfo_foregroundcolor")
    {
        values.append("255,215,0");
    }
    else if(key == "gproxy_foregroundcolor")
    {
        values.append("75,0,130");
    }
    else if(key == "bnet_foregroundcolor")
    {
        values.append("139,0,0");
    }
    else if(key == "info_foregroundcolor")
    {
        values.append("0,255,255");
    }
    else if(key == "emote_foregroundcolor")
    {
        values.append("128,128,128");
    }
    else if(key == "error_foregroundcolor")
    {
        values.append("255,0,0");
    }
    else if(key == "warning_foregroundcolor")
    {
        values.append("255,69,0");
    }
    else if(key == "usercolor")
    {
        values.append("baerli_graz{255,0,0};"
                "klingone_graz{255,0,0};"
                "Noman(1){0,0,255};"
                "Manufactoring{0,255,0};"
                "Phyton{0,255,0}");
    }
    else // Add an empty string (DO NOT DELETE THIS ELSE CLAUSE!)
    {
        values.append("");
    }
}

/**
 * Checks if the GProxy configuration file has values to connect to battle.net.
 *
 * @return <code>true</code> if the required values are configurated, <code>false</code> otherwise.
 */
bool Config::hasRequiredValues ()
{
    if (getString("war3path").isEmpty() || getString("cdkeyroc").isEmpty()
            || getString("cdkeytft").isEmpty() || getInt("war3version") == 0
            || getString("server").isEmpty() || getString("username").isEmpty()
            || getString("password").isEmpty() || getString("channel").isEmpty()
            || getInt("port") == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

QList<QString> Config::getKeys ()
{
    return keys;
}

QList<QString> Config::getValues ()
{
    return values;
}

void Config::setAdmins(const QStringList& admins)
{
    this->admins = admins;
}

QString Config::getString (const QString &key)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            return values.at(i);
        }
    }

    return "";
}

int Config::getInt (const QString &key)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            return values.at(i).toInt();
        }
    }

    return 0;
}

bool Config::getBoolean (const QString &key)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            QString value = values.at(i).toLower();
            if (value == "false" || value == "off"
                    || value == "no" || value == "n" || value == "disabled")
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }

    return true;
}

QColor Config::getColor (const QString &key)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            QString color = values.at(i);
            QStringList rgbList = color.split(",");

            if (rgbList.count() != 3)
            {
                return QColor::Invalid;
            }

            return QColor(rgbList.at(0).toInt(), rgbList.at(1).toInt(), rgbList.at(2).toInt());
        }
    }

    return QColor::Invalid;
}

QColor Config::getColor(const ColoredMessage::ColorType& colorType)
{
    QString colorTypeString = ColorUtil::toString(colorType);
    return getColor(colorTypeString.toLower() + "_foregroundcolor");
}

QColor Config::getDefaultColor()
{
    return getColor(ColoredMessage::DEFAULT);
}

QFont Config::getFont (const QString &key)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            QFont font;
            font.fromString(values.at(i));
            return font;
        }
    }

    return QFont();
}

QColor Config::getUserColor(const QString& username, const QColor& defaultColor)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == "usercolor")
        {
            QStringList userColors = values.at(i).split(';');
            foreach (QString userColor, userColors)
            {
                QString name = userColor.left(userColor.indexOf('{'));
                if (name.toLower() == username.toLower())
                {
                    QString color = userColor.mid(name.length() + 1, userColor.length() - name.length() - 2);
                    QStringList rgbList = color.split(",");

                    if (rgbList.count() != 3)
                    {
                        return QColor::Invalid;
                    }

                    return QColor(rgbList.at(0).toInt(), rgbList.at(1).toInt(), rgbList.at(2).toInt());
                }
            }

            return getDefaultUserColor(username, defaultColor);
        }
    }

    return QColor::Invalid;
}



bool Config::setString (const QString &key, const QString &value)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            values[i] = value;
            return true;
        }
    }

    return false;
}

bool Config::setInt (const QString &key, const int &value)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            values[i] = QString::number(value);
            return true;
        }
    }

    return false;
}

bool Config::setBoolean (const QString &key, const bool &value)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            if (value == false)
            {
                values[i] = "false";
            }
            else
            {
                values[i] = "true";
            }

            return true;
        }
    }

    return false;
}

bool Config::setColor (const QString &key, const QColor &color)
{
    if(color.isValid() == false)
    {
        return false;
    }

    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            values[i] = QString::number(color.red()) + ","
                    + QString::number(color.green()) + ","
                    + QString::number(color.blue());
            return true;
        }
    }

    return false;
}

bool Config::setFont (const QString &key, const QFont &font)
{
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == key.toLower())
        {
            values[i] = font.toString();

            return true;
        }
    }

    return false;
}

bool Config::setUserColor(const QString& username, const QColor& usercolor)
{
    if(usercolor.isValid() == false)
    {
        return false;
    }

    QString newUserColor = username + '{'
            + QString::number(usercolor.red()) + ','
            + QString::number(usercolor.green()) + ','
            + QString::number(usercolor.blue())
            + '}';

    for (int i = 0; i < keys.count(); i++)
    {
        if (keys.at(i).toLower() == "usercolor")
        {
            QStringList userColors = values.at(i).split(';');
            bool userColorReplaced = false;
            for (int j = 0; j < userColors.count(); ++j)
            {
                QString userColor = userColors.at(j);
                QString name = userColor.left(userColor.indexOf('{'));
                if (name.toLower() == username.toLower())
                {
                    userColors[j] = newUserColor;
                    userColorReplaced = true;
                    break;
                }
            }

            if (!userColorReplaced)
            {
                userColors.append(newUserColor);
            }

            values[i] = userColors.join(";");
            return true;
        }
    }

    return false;
}

void Config::commit ()
{
    if (configFile->open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(configFile);

        for (int i = 0; i < keys.count(); i++)
        {
            if (keys.at(i).startsWith("#"))
            {
                out << "\n" << keys.at(i) << "\n\n";
            }
            else
            {
                out << keys.at(i) << " = " << values.at(i) << "\n";
            }
        }

        out.flush();
        configFile->close();

        emit configSaved();
    }
}

QColor Config::getDefaultUserColor(const QString& username, const QColor& defaultColor)
{
    if (username.startsWith("GhostGraz"))
    {
        // Return bot color.
        return QColor(255, 255, 0);
    }
    else if (admins.contains(username, Qt::CaseInsensitive))
    {
        // Return admin color.
        return QColor(0, 255, 200);
    }
    else
    {
        // Return default color.
        if (defaultColor.isValid())
        {
            return defaultColor;
        }
        else
        {
            return getDefaultColor();
        }
    }
}