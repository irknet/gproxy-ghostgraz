
#include "Config.h"
#include <QStringList>
#include <QTextStream>

using namespace std;

Config::Config (QString config)
{
    configFile = new QFile(config);
}

Config::~Config () { }

int Config::loadConfig()
{
    if (configFile->open(QFile::ReadWrite))
    {
        vKey.clear();
        vValue.clear();
        addKeys();
        addValues(QString(configFile->readAll()));
        configFile->close();

        if(hasRequiredValues())
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

void Config::addKeys()
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
    vKey.append("sound");
    vKey.append("privategamename");
    vKey.append("botprefix");
    vKey.append("autosearch");
    vKey.append("log");

    // TODO
    vKey.append("# Application config values");
    vKey.append("width");
    vKey.append("height");
}

void Config::addValues(QString content)
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

    for(int i = 0; i < vKey.count(); i++)
    {
        if(!vTempKey.contains(vKey.at(i)))
        {
            if(vKey.at(i) == "war3version")
            {
                vValue.append("26");
            }
            else if(vKey.at(i) == "server")
            {
                vValue.append("europe.battle.net");
            }
            else if(vKey.at(i) == "channel")
            {
                vValue.append("Clan Graz");
            }
            else if(vKey.at(i) == "port")
            {
                vValue.append("6125");
            }
            else if(vKey.at(i) == "sound")
            {
                vValue.append("on");
            }
            else if(vKey.at(i) == "privategamename")
            {
                vValue.append("inhouse");
            }
            else if(vKey.at(i) == "botprefix")
            {
                vValue.append("GhostGraz");
            }
            else if(vKey.at(i) == "autosearch")
            {
                vValue.append("on");
            }
            else if(vKey.at(i) == "log")
            {
                vValue.append("on");
            }
            else
            {
                vValue.append("");
            }
            continue;
        }

        for(int j = 0; j < vTempKey.count(); j++)
        {
            if(vKey.at(i) == vTempKey.at(j))
            {
                vValue.append(vTempValue.at(j));
            }
        }
    }
}

bool Config::hasRequiredValues()
{
    if(getString("war3path").isEmpty() || getString("cdkeyroc").isEmpty()
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

QVector<QString> Config::getKeys()
{
    return vKey;
}

QVector<QString> Config::getValues()
{
    return vValue;
}

QString Config::getString (QString key)
{
    for(int i = 0; i < vKey.count(); i++)
    {
        if(vKey.at(i).toLower() == key.toLower())
        {
            return vValue.at(i);
        }
    }

    return "";
}

int Config::getInt (QString key)
{
    for(int i = 0; i < vKey.count(); i++)
    {
        if(vKey.at(i).toLower() == key.toLower())
        {
            return vValue.at(i).toInt();
        }
    }

    return 0;
}

bool Config::getBoolean(QString key)
{
    for(int i = 0; i < vKey.count(); i++)
    {
        if(vKey.at(i).toLower() == key.toLower())
        {
            QString value = vValue.at(i).toLower();
            if(value == "false" || value == "off"
                    || value == "no" || value == "n")
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

bool Config::setString (QString key, QString value)
{
    for(int i = 0; i < vKey.count(); i++)
    {
        if(vKey.at(i).toLower() == key.toLower())
        {
            vValue[i] = value;
            return true;
        }
    }

    return false;
}

bool Config::setInt (QString key, int value)
{
    for(int i = 0; i < vKey.count(); i++)
    {
        if(vKey.at(i).toLower() == key.toLower())
        {
            vValue[i] = QString::number(value);
            return true;
        }
    }

    return false;
}

bool Config::setBoolean(QString key, bool value)
{
    for(int i = 0; i < vKey.count(); i++)
    {
        if(vKey.at(i).toLower() == key.toLower())
        {
            if(value == false)
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

void Config::commit()
{
    if (configFile->open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(configFile);

        for (int i = 0; i < vKey.count(); i++)
        {
            if(vKey.at(i).startsWith("#"))
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