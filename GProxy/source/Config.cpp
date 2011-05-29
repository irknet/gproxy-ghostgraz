#include "Config.h"
#include <QStringList>
#include <QTextStream>

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
        vKey.clear();
        vValue.clear();
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
    vKey.append("# Required config values");
    vKey.append("war3path");
    vKey.append("cdkeyroc");
    vKey.append("cdkeytft");
    vKey.append("war3version");
    vKey.append("server");
    vKey.append("username");
    vKey.append("password");
    vKey.append("channel");
    vKey.append("port");

    vKey.append("# PvPGN config values");
    vKey.append("exeversion");
    vKey.append("exeversionhash");
    vKey.append("passwordhashtype");

    vKey.append("# Optional config values");
    vKey.append("ghostgrazUsername");
    vKey.append("ghostgrazPassword");
    vKey.append("sound");
    vKey.append("privategamename");
    vKey.append("botprefix");
    vKey.append("log");

    vKey.append("# Application config values");
    vKey.append("width");
    vKey.append("height");
    vKey.append("botorder");
    vKey.append("outputareaForegroundcolor");
    vKey.append("outputareaBackgroundcolor");
    vKey.append("outputareaFont");
    vKey.append("inputareaForegroundcolor");
    vKey.append("inputareaBackgroundcolor");
    vKey.append("inputareaFont");
}

void Config::addValues (QString content)
{
    QVector<QString> vTempKey;
    QVector<QString> vTempValue;

    QStringList lines = content.split("\n");
    for (QStringList::Iterator it = lines.begin(); it != lines.end(); ++it)
    {
        QString line = QString(*it);

        QString key = line.mid(0, line.indexOf("="));
        key = key.trimmed();

        QString value = line.mid(line.indexOf("=") + 1);
        value = value.trimmed();

        vTempKey.append(key);
        vTempValue.append(value);
    }

    for (int i = 0; i < vKey.count(); i++)
    {
        if (!vTempKey.contains(vKey.at(i)))
        {
            addDefaultValue(vKey.at(i));
            continue;
        }

        // Load config values
        for (int j = 0; j < vTempKey.count(); j++)
        {
            if (vKey.at(i) == vTempKey.at(j))
            {
                vValue.append(vTempValue.at(j));
            }
        }
    }
}

void Config::addDefaultValue (const QString &key)
{
    if (key == "war3version")
    {
        vValue.append("26");
    }
    else if (key == "server")
    {
        vValue.append("europe.battle.net");
    }
    else if (key == "channel")
    {
        vValue.append("Clan Graz");
    }
    else if (key == "port")
    {
        vValue.append("6125");
    }
    else if (key == "sound")
    {
        vValue.append("Enabled");
    }
    else if (key == "privategamename")
    {
        vValue.append("inhouse");
    }
    else if (key == "botprefix")
    {
        vValue.append("GhostGraz");
    }
    else if (key == "log")
    {
        vValue.append("Enabled");
    }
    else if(key == "outputareaForegroundcolor")
    {
        vValue.append("255,255,255");
    }
    else if(key == "outputareaBackgroundcolor")
    {
        vValue.append("0,0,0");
    }
    else if(key == "outputareaFont")
    {
        vValue.append("Arial,9,50,false");
    }
    else // add an empty string (THIS IS NEEDED!)
    {
        vValue.append("");
    }
}

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

QVector<QString> Config::getKeys ()
{
    return vKey;
}

QVector<QString> Config::getValues ()
{
    return vValue;
}

QString Config::getString (const QString &key)
{
    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            return vValue.at(i);
        }
    }

    return "";
}

int Config::getInt (const QString &key)
{
    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            return vValue.at(i).toInt();
        }
    }

    return 0;
}

bool Config::getBoolean (const QString &key)
{
    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            QString value = vValue.at(i).toLower();
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
    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            QString color = vValue.at(i);
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

QFont Config::getFont (const QString &key)
{
    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            QString font = vValue.at(i);
            QStringList fontSettings = font.split(",");

            if (fontSettings.count() != 4)
            {
                return QFont();
            }

            if (fontSettings.at(3) == "true")
            {
                return QFont(fontSettings.at(0), fontSettings.at(1).toInt(),
                        QString(fontSettings.at(2)).toInt(), true);
            }
            else
            {
                return QFont(fontSettings.at(0), fontSettings.at(1).toInt(),
                        QString(fontSettings.at(2)).toInt(), false);
            }
        }
    }

    return QFont();
}

bool Config::setString (const QString &key, const QString &value)
{
    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            vValue[i] = value;
            return true;
        }
    }

    return false;
}

bool Config::setInt (const QString &key, const int &value)
{
    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            vValue[i] = QString::number(value);
            return true;
        }
    }

    return false;
}

bool Config::setBoolean (const QString &key, const bool &value)
{
    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            if (value == false)
            {
                vValue[i] = "false";
            }
            else
            {
                vValue[i] = "true";
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

    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            vValue[i] = QString::number(color.red()) + ","
                    + QString::number(color.green()) + ","
                    + QString::number(color.blue());
            return true;
        }
    }

    return false;
}

bool Config::setFont (const QString &key, const QFont &font)
{
    for (int i = 0; i < vKey.count(); i++)
    {
        if (vKey.at(i).toLower() == key.toLower())
        {
            if (font.italic())
            {
                vValue[i] = font.family() + "," + font.pointSize() + ","
                        + font.weight() + ",true";
            }
            else
            {
                vValue[i] = font.family() + "," + font.pointSize() + ","
                        + font.weight() + ",false";
            }

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

        for (int i = 0; i < vKey.count(); i++)
        {
            if (vKey.at(i).startsWith("#"))
            {
                out << "\n" << vKey.at(i) << "\n\n";
            }
            else
            {
                out << vKey.at(i) << " = " << vValue.at(i) << "\n";
            }
        }

        out.flush();
        configFile->close();
    }
}