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
    keys.append("# Required config values");
    keys.append("war3path");
    keys.append("cdkeyroc");
    keys.append("cdkeytft");
    keys.append("war3version");
    keys.append("server");
    keys.append("username");
    keys.append("password");
    keys.append("channel");
    keys.append("port");

    keys.append("# PvPGN config values");
    keys.append("exeversion");
    keys.append("exeversionhash");
    keys.append("passwordhashtype");

    keys.append("# Optional config values");
    keys.append("ghostgrazUsername");
    keys.append("ghostgrazPassword");
    keys.append("sound");
    keys.append("privategamename");
    keys.append("botprefix");
    keys.append("log");

    keys.append("# Application config values");
    keys.append("width");
    keys.append("height");
    keys.append("botorder");

    keys.append("# Appearance");
    keys.append("backgroundcolor");
    keys.append("outputareaForegroundcolor");
    keys.append("outputareaFont");
    keys.append("inputareaForegroundcolor");
    keys.append("inputareaFont");
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
    else if (key == "sound")
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
    else // add an empty string (THIS IS NEEDED!)
    {
        values.append("");
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

QList<QString> Config::getKeys ()
{
    return keys;
}

QList<QString> Config::getValues ()
{
    return values;
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