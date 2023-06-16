#pragma once

#include <QSize>
#include <QFont>

//For MessageWView
static const QString  VIEW_STYLE_SHEET = R"(
QMenu::item{ background-color: rgb(0, 170, 0); color: rgb(255, 255, 255); }
QMenu::item:selected { background-color: rgb(0, 85, 127); color: rgb(255, 255, 255);}
QListView { background-color: white; background-image: url(:/mainWindow/resources/tile.png) repeat;} 
)";


//For ChatWView
static const QString  VIEW_CHAT_STYLE_SHEET = R"(
QMenu::item{ background-color: rgb(0, 170, 0); color: rgb(255, 255, 255); }
QMenu::item:selected { background-color: rgb(0, 85, 127); color: rgb(0, 255, 255);}
QListView { background-color: white;} 
)";


//For MessageWDelegate
static constexpr int	SHADOW_OFFSET{ 5 };
static constexpr int	TEXTBOX_TOP_MARGIN{ 10 };
static constexpr int	AVATAR_MARGIN_RIGHT{ 10 };
static constexpr int	NAME_TIME_LINE_HEIGHT{ 30 };
static constexpr int	LIKES_ICON_WIDTH = 16;
static constexpr int	LIKES_TEXT_WIDTH = 20;
static constexpr int	IMAGE_PREVIEW_SPACING = { 15 };
static constexpr int	BORDER_RADIUS{ 5 };
static constexpr int	LIKES_IQON_WIDTH = 16;
static constexpr int	ACTION_ICON_WIDTH = 16;



static const QColor		MY_COLOR_1{ 216, 235, 255 };
static const QColor		MY_COLOR_2{ 240, 240, 255 };
static const QColor		THERE_COLOR_1{ 240, 240, 240 };
static const QColor		THERE_COLOR_2{ 255, 255, 255 };
static const QColor		SHADOW_COLOR{ 0, 0, 0, 80 };
static const QColor		NAME_COLOR{ 25, 109, 153 };
static const QColor		TEXT_COLOR{ 20, 20, 20 };
static const QColor		TIME_COLOR{ 160, 160, 160 }; 
static const QColor		ACTION_BOX_COLOR{ 200, 200, 200  };
static const QColor		LIKES_BOX_COLOR{ 0, 0, 255, 150  };
static const QColor		LIKES_TEXT_COLOR = { 255, 255, 255 };
static const QColor		DISLIKES_BOX_COLOR{ 255, 0, 0, 150  };
static const QColor		MY_BORDER_COLOR{ 100, 180, 180 };
static const QColor		THERE_BORDER_COLOR{ 180, 180, 100 };

static const QPoint		ACTION_ICON_MARGINS = { 6, 4 };
static const QPoint		CONTENT_MARGINS{ 10, 10 };
static const QPoint		LIKES_MARGIN { 6, 6 };


static const QSize		AVATAR_SIZE = { 32, 32 };
static const QSize		ACTION_BOX_SIZE{ 80, 24  };
static const QSize		ACTION_ICON_SIZE = { 16, 16 };
// Also for MessageItem
static const QSize		IMAGE_PREVIEW_SIZE_MAX = { 250, 250 };


static int				ACTION_BOX_OFFSET{ CONTENT_MARGINS.x() };

static const QFont		NAME_FONT{ "Segoe UI", 12, QFont::DemiBold };
static const QFont		TIME_FONT{ "Segoe UI", 8, QFont::Light };
static const QFont		LIKES_FONT{ "Segoe UI", 10, QFont::DemiBold };
//Also for MessageWView
static const QFont		MESSAGE_FONT{ "Titilium Web", 12, QFont::Normal };

//ChatWView
static const QFont		CHAT_FONT{"Titilium Web", 14, QFont::Normal};
static const QFont		CHAT_HOVER_FONT{"Titilium Web", 14, QFont::Bold};
static const QFont		CHAT_DESCR_FONT {"Titilium Web", 10, QFont::Normal};

static QColor			HOVERED_CHAT_COLOR{224, 255, 255};
static QColor			NORMAL_CHAT_COLOR{255, 182, 193};
static QColor			DESCRIPTION_COLOR {255, 255, 224};