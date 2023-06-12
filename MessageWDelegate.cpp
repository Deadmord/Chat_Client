#include "MessageWDelegate.h"

//QT_BEGIN_NAMESPACE
//extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter* p, QImage& blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
//QT_END_NAMESPACE

MessageWDelegate::MessageWDelegate(QObject* parent)
	: QItemDelegate(parent)
{
}

MessageWDelegate::~MessageWDelegate()
{}

QSize MessageWDelegate::sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const
{
	const auto& msg = index.data(MessageWModel::MessageRole).value< messageItemPtr>();
	const auto own_msg = msg->isOwn();

	const int item_own_width = option.rect.width() * 3 / 4 - 20;
	const int added_height = CONTENT_MARGINS.y() * 2 + AVATAR_SIZE.height() + TEXTBOX_TOP_MARGIN + SHADOW_OFFSET + msg->getImageCount() * (IMAGE_PREVIEW_SIZE_MAX.height() + IMAGE_PREVIEW_SPACING);
	const QRect available_text_box_rect = { option.rect.topLeft(), QSize{ item_own_width - CONTENT_MARGINS.x() * 2 - 10, option.rect.height() } };

	const QFontMetrics fm{ MESSAGE_FONT };
	const auto textbox_rect = fm.
		boundingRect(available_text_box_rect, msg->isRtl() ?
			Qt::AlignRight | Qt::AlignTop | Qt::TextWordWrap :
			Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, msg->getMesText());

	msg->setCurrentTextBoxSize(textbox_rect.size());
	return { option.rect.width(), textbox_rect.height() + added_height };
}

void MessageWDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const auto& msg = index.data(MessageWModel::MessageRole).value<messageItemPtr>();

	const auto own_msg = msg->isOwn();
	const auto is_hoverd_msg = msg->isHovered();
	auto const is_rtl = msg->isRtl();

	const int own_left_shift = own_msg ? option.rect.width() / 4 : 0;
	const int own_right_shift = own_msg ? 0 : option.rect.width() / 4;

	const auto whole_rect = option.rect.adjusted(own_left_shift, 0, -own_right_shift, 0);

	auto const& body_rect = whole_rect.adjusted(-SHADOW_OFFSET, 0, 20, 0);

	auto const& shadow_rect = body_rect.translated(SHADOW_OFFSET, SHADOW_OFFSET);

	auto const& current_text_box_size = msg->getCurrentTextBoxSize();

	const QPoint text_start_point = !is_rtl ?
		body_rect.topLeft() + QPoint(CONTENT_MARGINS.x(), CONTENT_MARGINS.y() + AVATAR_SIZE.height() + TEXTBOX_TOP_MARGIN)
		: body_rect.topRight() + QPoint(-CONTENT_MARGINS.x() - current_text_box_size.width(), CONTENT_MARGINS.y() + AVATAR_SIZE.height() + TEXTBOX_TOP_MARGIN);

	const QRect text_rect = { text_start_point, current_text_box_size };

	// avatar rect
	auto avatar_rc = body_rect.adjusted(CONTENT_MARGINS.x(), CONTENT_MARGINS.y(), 0, 0);
	avatar_rc.setSize(AVATAR_SIZE);

	// name rect
	auto name_rc = avatar_rc.adjusted(avatar_rc.width() + AVATAR_MARGIN_RIGHT, 0, 0, 0);
	name_rc.setRight(body_rect.right());
	name_rc.setHeight(NAME_TIME_LINE_HEIGHT);

	// time rect
	auto const time_rc = name_rc.adjusted(0, 0, -10, 0);

	// likes rect
	auto const likes_rect = QRect{
		body_rect.left(),
		body_rect.bottom() - 2,
		LIKES_ICON_WIDTH + LIKES_MARGIN.x() * 2 + LIKES_TEXT_WIDTH,
		LIKES_ICON_WIDTH + LIKES_MARGIN.y() * 2 
	};

	// actions rect
	auto const actions_rect = own_msg
		? QRect {
		QPoint {
			body_rect.center() - QPoint{ body_rect.width() / 2 + ACTION_BOX_SIZE.width() - ACTION_BOX_OFFSET,
				ACTION_BOX_SIZE.height() / 2 }
		},
			ACTION_BOX_SIZE
	}
	: QRect{
		QPoint {
			body_rect.center() + QPoint{ body_rect.width() / 2 - ACTION_BOX_OFFSET,
					-ACTION_BOX_SIZE.height() / 2 }
			},
		ACTION_BOX_SIZE
	};

	/// drawing lambdas
	auto const draw_message_box = [&]()
	{
		auto const border_color = own_msg ? MY_BORDER_COLOR : THERE_BORDER_COLOR;
		auto const& get_callout = [&](const QRect& box)
		{
			const QPointF first_point = own_msg
				? QPointF{ static_cast<qreal>(box.right() - BORDER_RADIUS), static_cast<qreal>(box.top()) }
			: QPointF{ static_cast<qreal>(box.left() + BORDER_RADIUS), static_cast<qreal>(box.top()) };

			QPainterPath triangle_path(first_point);

			triangle_path.lineTo(own_msg
				? QPoint{ box.right() + 10, box.top() }.toPointF()
				: QPoint{ box.left() - 10, box.top() }.toPointF());

			triangle_path.lineTo(own_msg
				? QPoint{ box.right(), box.top() + 12 }.toPointF()
				: QPoint{ box.left(), box.top() + 12 }.toPointF());
			triangle_path.lineTo(first_point);
			triangle_path.closeSubpath();
			return triangle_path;
		};

		_msg_box_gradient->setStart(body_rect.left(), body_rect.top());
		_msg_box_gradient->setFinalStop(body_rect.left(), body_rect.bottom());
		_msg_box_gradient->setColorAt(0.0, own_msg ? MY_COLOR_1 : THERE_COLOR_1);
		_msg_box_gradient->setColorAt(1.0, own_msg ? MY_COLOR_2 : THERE_COLOR_2);

		// draw shadow
		QPainterPath path;
		path.addRoundedRect(QRectF(shadow_rect), BORDER_RADIUS, BORDER_RADIUS);
		path = path.united(get_callout(shadow_rect));
		painter->fillPath(path, SHADOW_COLOR);
		path.clear();

		// draw body
		auto const& border_pen = QPen{ border_color, 1 };
		painter->setPen(border_pen);
		path.addRoundedRect(QRectF(body_rect), BORDER_RADIUS, BORDER_RADIUS);
		path = path.united(get_callout(body_rect));
		painter->fillPath(path, *_msg_box_gradient);
		painter->drawPath(path);

		msg->setContentBox(body_rect);
	};

	const auto& draw_avatar = [&]()
	{
		msg->getMesAvatar().paint(painter, avatar_rc);
	};

	const auto& draw_name = [&]()
	{
		painter->setPen(NAME_COLOR);
		painter->setFont(NAME_FONT);
		painter->drawText(name_rc, Qt::AlignLeft | Qt::AlignVCenter, "~" + msg->getMesNickname());
	};

	const auto& draw_timestamp = [&]()
	{
		painter->setPen(TIME_COLOR);
		painter->setFont(TIME_FONT);
		painter->drawText(time_rc, Qt::AlignRight | Qt::AlignVCenter, msg->getMesTimeStamp().toString());
	};

	const auto& draw_actions = [&]()
	{
		if (!is_hoverd_msg)
			return;

		// draw shadow
		QPainterPath path;
		path.addRoundedRect(QRectF(actions_rect.adjusted(SHADOW_OFFSET, SHADOW_OFFSET, SHADOW_OFFSET, SHADOW_OFFSET)), 3, 3);
		painter->fillPath(path, SHADOW_COLOR);
		path.clear();

		// draw body
		path.addRoundedRect(QRectF(actions_rect), 3, 3);
		painter->fillPath(path, ACTION_BOX_COLOR);

		auto const action_rc = QRect{
			QPoint{
				actions_rect.left() + ACTION_ICON_MARGINS.x(),
				actions_rect.top() + ACTION_ICON_MARGINS.y() 	},
			ACTION_ICON_SIZE };
		painter->drawPixmap(action_rc, _like_pixmap.scaled(ACTION_ICON_SIZE, Qt::KeepAspectRatio));
		auto const dislike_rc = action_rc.translated({ ACTION_ICON_WIDTH + 10, 0 });
		painter->drawPixmap(dislike_rc, _dislike_pixmap.scaled(ACTION_ICON_SIZE, Qt::KeepAspectRatio));

		msg->setLikeButtRect(action_rc);
		msg->setDislikeButtRect(dislike_rc);
	};

	const auto& draw_likes = [&]()
	{
		auto const& likes = msg->getMesLikes();
		if (!likes)
			return;

		// draw shadow
		QPainterPath path;
		path.addRoundedRect(QRectF(likes_rect.adjusted(SHADOW_OFFSET, SHADOW_OFFSET, SHADOW_OFFSET, SHADOW_OFFSET)), 2, 2);
		painter->fillPath(path, SHADOW_COLOR);
		path.clear();

		// draw body
		path.addRoundedRect(QRectF(likes_rect), 3, 3);
		painter->fillPath(path, likes > 0 ? LIKES_BOX_COLOR : DISLIKES_BOX_COLOR);

		auto const& icon_rc = QRect{
			likes_rect.left() + LIKES_MARGIN.x(),
			likes_rect.top() + LIKES_MARGIN.y(),
			LIKES_IQON_WIDTH,
			LIKES_IQON_WIDTH };


		painter->drawPixmap(icon_rc, likes > 0 ? _like_pixmap : _dislike_pixmap);

		painter->setPen(LIKES_TEXT_COLOR);
		painter->setFont(LIKES_FONT);
		painter->drawText(QRect{
			icon_rc.right() + 6,
			icon_rc.top() - 2,
			LIKES_TEXT_WIDTH,
			icon_rc.height() }, Qt::AlignLeft | Qt::AlignVCenter, QString::number(likes));
	};

	const auto& draw_text = [&]()
	{
		painter->setFont(MESSAGE_FONT);
		painter->setPen(TEXT_COLOR);

		painter->drawText(text_rect, msg->isRtl() ?
			Qt::AlignRight | Qt::AlignTop | Qt::TextWordWrap :
			Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, msg->getMesText());

		msg->setCurrentTextBoxRect(text_rect);
	};

	const auto& draw_attached_images = [&]()
	{
		if (!msg->getImageCount())
			return;

		auto  start_point = text_rect.bottomLeft() + QPoint{ 0, IMAGE_PREVIEW_SPACING };
		QPainterPath path;

		msg->getImageBoxRects().clear();
		for (auto i = 0; i < msg->getImageCount(); ++i)
		{
			auto px = msg->getMesImagelist().at(i);
			auto const px_rc = QRect{ start_point, px.size() };

			auto const& border_pen = QPen{ MY_BORDER_COLOR, 1 };
			painter->setPen(border_pen);

			path.addRoundedRect(px_rc.adjusted(-1, -1, 1, 1).toRectF(), 3, 3);
			painter->drawPath(path);
			painter->drawPixmap(start_point, px);
			start_point += QPoint{ 0, px.height() + IMAGE_PREVIEW_SPACING };

			msg->getImageBoxRects().push_back(px_rc);
		}
	};

	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	painter->save();

	//QImage tmp;
	//qt_blurImage(painter, tmp, 3, false, _shadow_offset);

	draw_message_box();
	draw_avatar();
	draw_name();
	draw_text();
	draw_timestamp();
	draw_attached_images();
	draw_likes();
	draw_actions();
	painter->restore();
};