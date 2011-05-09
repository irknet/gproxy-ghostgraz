#ifndef CONFIG2_H
#define	CONFIG2_H

#include <QFile>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QPair>

class Config : public QObject
{
    Q_OBJECT

public:
    Config(QString config);
    virtual ~Config();
    int loadConfig();
    bool hasRequiredValues();

    QVector<QString> getKeys();
    QVector<QString> getValues();
    QString getString(QString key);
    int getInt(QString key);
    bool getBoolean(QString key);

    bool setString(QString key, QString value);
    bool setInt(QString key, int value);
    bool setBoolean(QString key, bool value);

    void commit();

private:
    QFile *configFile;
    QVector<QString> vKey;
    QVector<QString> vValue;

    void addKeys();
    void addValues(QString content);

private slots:

};

#endif	/* CONFIG_H */

