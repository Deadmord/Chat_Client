//#include "MyFont.h"
#include "MessageDelegate.h"


//MessageDelegate::MessageDelegate(QObject* parent)
//    : QItemDelegate(parent)
//{
//}
//
//MessageDelegate::~MessageDelegate()
//{}

void MessageDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    
    if (!index.isValid())
        return;

    createEditor(const_cast<QWidget*>(option.widget), option, index);


    const auto& draw_message = [] {

    };
    const MessageStruct& message = index.data().value<MessageStruct>();

    QList<int> list_width = { text_font.getWidth(message.mes_text) , nickname_font.getWidth(message.mes_nickname) , time_font.getWidth(message.mes_time.toString("dd:MM:yy hh:mm")) };
    int rect_width = *std::max_element(list_width.begin(), list_width.end());

    rect_width = rect_width < option.rect.width() / 3 * 2 ? rect_width : option.rect.width() / 3 * 2;
    QRect bubbleRect(option.rect.topLeft(), QSize(rect_width + 20, (option.rect.height() - 20)));

    //TODO change to current usernickname
    QColor bubbleColor = (message.mes_nickname == "Vasya") ? QColor(224, 255, 255) : QColor(255,182,193); // Example background color for message

    int borderRadius = 10;
    painter->setBrush(bubbleColor);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(bubbleRect, borderRadius, borderRadius);

    //TODO if loop if(mes_is_del == false){}
    //if(message.mes_nickname == USER_NICKNAME){Print left}
    //else{Print right}

    int time_height = time_font.getHeight();
    int nickname_height = nickname_font.getHeight();;
    int text_height = text_font.getHeight();;

    // Draw nickname
    painter->setPen(Qt::black);
    painter->setFont(nickname_font.getFont());

    QRect nicknameRect = bubbleRect.adjusted(10, 5, -10, ( - 5 - time_height - text_height));
    painter->drawText(nicknameRect, Qt::AlignLeft | Qt::AlignTop, message.mes_nickname);

    // Draw message
    QRect textRect = bubbleRect.adjusted(10, (5 + nickname_height + 5), -10, (-5 - time_height)); // Example positioning
    painter->setPen(Qt::black);
    painter->setFont(text_font.getFont()); // Example font and size
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignTop, message.mes_text);

    // Draw time
    painter->setFont(time_font.getFont());
    painter->setPen(Qt::gray);
    QRect timeRect = bubbleRect.adjusted(10, (5 + nickname_height + 5 + text_height + 5), -10, -5);
    painter->drawText(timeRect, Qt::AlignRight | Qt::AlignBottom, message.mes_time.toString("dd:MM:yy hh:mm"));

    const auto whole_rect = option.rect.adjusted(option.rect.width() / 4, 0, -option.rect.width() / 4, 0);
    auto const& body_rect = whole_rect.adjusted(-5, 0, 20, 0);

    auto const actions_rect = QRect{
            QPoint {
                body_rect.center() - QPoint{ body_rect.width() / 2 + gl_action_box_size.width() - gl_action_box_offset,
                    gl_action_box_size.height() / 2 }
            },
        gl_action_box_size
    };

    const auto& draw_actions = [&]()
        {

        	// draw shadow
        	QPainterPath path;
        	path.addRoundedRect(QRectF(actions_rect.adjusted(gl_shadow_offset, gl_shadow_offset, gl_shadow_offset, gl_shadow_offset)), 3, 3);
        	painter->fillPath(path, gl_shadow_color);
        	path.clear();

        	// draw body
        	path.addRoundedRect(QRectF(actions_rect), 3, 3);
        	painter->fillPath(path, gl_action_box_color);

        	auto const action_rc = QRect{
        		QPoint{
        			actions_rect.left() + gl_action_icon_margins.x(),
        			actions_rect.top() + gl_action_icon_margins.y() 	},
        		gl_action_icon_size };
        	painter->drawPixmap(action_rc, _like_pixmap.scaled(gl_action_icon_size , Qt::KeepAspectRatio ));
        	auto const dislike_rc = action_rc.translated({ gl_action_icon_width + 10, 0 });
        	painter->drawPixmap(dislike_rc, _dislike_pixmap.scaled(gl_action_icon_size, Qt::KeepAspectRatio));

        };

    draw_actions();
    
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{

    const MessageStruct& message = index.data().value<MessageStruct>();

    int bubble_height = text_font.getHeight() + nickname_font.getHeight() + time_font.getHeight() + 80;  // Add padding for top and bottom margins

    return QSize(option.rect.width(), bubble_height);
}
