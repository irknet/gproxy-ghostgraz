#ifndef COLOREDMESSAGE_H
#define	COLOREDMESSAGE_H

#include <QObject>
#include <QMetaType>
#include <QString>
#include <QColor>

class ColoredMessage : public QObject
{
    Q_OBJECT
    Q_ENUMS(ColorType)

public:
    enum ColorType
    {
        DEFAULT,
        WHISPER,
        GAMEINFO,
        GPROXY,
        BNET,
        INFO,
        EMOTE,
        WARNING,
        ERROR,
        USERCOLOR,
        CUSTOM
    };

    ColoredMessage();
    ColoredMessage(const QString& message, const ColorType& colorType = ColoredMessage::DEFAULT,
            const QColor& color = QColor::Invalid);
    ColoredMessage(const ColoredMessage& orig);
    ColoredMessage& operator= (const ColoredMessage& orig);
    bool operator== (const ColoredMessage& coloredMessage);
    bool operator!= (const ColoredMessage& coloredMessage);
    virtual ~ColoredMessage();

    // <editor-fold defaultstate="collapsed" desc="Getters and setters">
    QColor getColor() const;
    void setColor(const QColor& color);
    ColorType getColorType() const;
    void setColorType(const ColorType& colorType);
    QString getMessage() const;
    void setMessage(const QString& message);
    // </editor-fold>

private:
    QString message;
    ColorType colorType;
    QColor color;
};

Q_DECLARE_METATYPE(ColoredMessage)

#endif	/* COLOREDMESSAGE_H */

