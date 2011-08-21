#include "MTextEdit.h"

MTextEdit::MTextEdit (QWidget* parent) : QTextEdit (parent)
{
    doLineBreak = false;
}

MTextEdit::~MTextEdit () { }

void MTextEdit::setConfig(Config* config)
{
    this->config = config;
}

/**
 * Appends the colored Message with the message and color to the end of the TextEdit.
 *
 * @param coloredMessage The message including the color information.
 */
void MTextEdit::appendColoredMessage(const ColoredMessage& coloredMessage)
{
    coloredMessages.append(coloredMessage);

    bool scrollbarOnBottom = false;
    int scrollbarPosition = verticalScrollBar()->value();
    if (scrollbarPosition == verticalScrollBar()->maximum())
    {
        scrollbarOnBottom = true;
    }

    moveCursor(QTextCursor::End);
    setTextColor(getColor(coloredMessage));

    if (doLineBreak)
    {
        insertPlainText("\n");
    }

    QString message = coloredMessage.getMessage();
    if (message.endsWith('\n'))
    {
        doLineBreak = true;
        insertPlainText(message.left(message.length() - 1));
    }
    else
    {
        doLineBreak = false;
        insertPlainText(message);
    }

    if (scrollbarOnBottom)
    {
        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    }
    else
    {
        verticalScrollBar()->setValue(scrollbarPosition);
    }
}

/**
 * Repaints the text edit with the changed color of the colorType.
 *
 * @param colorType ColoredMessage::ColorType.
 * @param color New color for the colorType.
 * @param username The username for comparison if the colorType is usercolor.
 */
void MTextEdit::repaint(const QString& colorType, const QColor& color, const QString& username)
{
    QList<ColoredMessage> currentColoredMessages = coloredMessages;
    resetVariables();
    clear();

    foreach(ColoredMessage coloredMessage, currentColoredMessages)
    {
        if (ColorUtil::toString(coloredMessage.getColorType()).toLower() == colorType.toLower())
        {
            if (coloredMessage.getColorType() == ColoredMessage::USERCOLOR)
            {
                if (username.isEmpty() || coloredMessage.getMessage() == username)
                {
                    coloredMessage.setColor(color);
                }
            }
            else
            {
                coloredMessage.setColor(color);
            }
        }
        else if (colorType.isEmpty())
        {
            coloredMessage.setColor(QColor::Invalid);
        }
        else if (colorType.toLower() == "default"
                && coloredMessage.getColorType() == ColoredMessage::USERCOLOR)
        {
            coloredMessage.setColor(config->getUserColor(coloredMessage.getMessage(), color));
        }

        appendColoredMessage(coloredMessage);
    }
}

QColor MTextEdit::getColor(const ColoredMessage& coloredMessage)
{
    // If the color is valid use the color value, else use the config value.
    if (coloredMessage.getColorType() == ColoredMessage::CUSTOM
            || coloredMessage.getColor().isValid())
    {
        return coloredMessage.getColor();
    }
    else if (coloredMessage.getColorType() == ColoredMessage::USERCOLOR)
    {
        return config->getUserColor(coloredMessage.getMessage(), coloredMessage.getColor());
    }
    else
    {
        return config->getColor(coloredMessage.getColorType());
    }
}

void MTextEdit::resetVariables()
{
    // Do not reset the config variable!
    coloredMessages.clear();
    doLineBreak = false;
}