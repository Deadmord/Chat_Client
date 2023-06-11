#include "LogInWDelegate.h"

void LogInWDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{

    if (!index.isValid())
        return;

    const LogInWStruct& message = index.data().value<LogInWStruct>();

    QRect bubbleRect(option.rect);

    //TODO change to current usernickname
    int borderRadius = 10;
    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(bubbleRect, borderRadius, borderRadius);

    //TODO if loop if(logIn_is_exist == false){}
    //print error
    //else{Print right}

    // Draw nickname
    painter->setPen(Qt::black);
    painter->setFont(data_font.getFont());

    QRect nicknameRect = bubbleRect.adjusted(10, 5, -10, -5);
    painter->drawText(nicknameRect, Qt::AlignLeft | Qt::AlignTop, message.log_in_nickname);

    // Draw message
    QRect textRect = bubbleRect.adjusted(10, 30, -10, -5); // Example positioning
    painter->setPen(Qt::black);
    painter->setFont(data_font.getFont()); // Example font and size
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignTop, message.log_in_password);
    
}

QSize LogInWDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{

    const MessageStruct& message = index.data().value<MessageStruct>();
    int bubble_height = data_font.getHeight() * 2 + 80;
    return QSize(option.rect.width(), bubble_height);
}
