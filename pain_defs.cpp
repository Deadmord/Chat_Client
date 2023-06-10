#pragma once
#include <QColor>
#include <QFont>
#include <QSize>

static const QString  gl_view_style_sheet = R"(
QMenu::item{ background-color: rgb(0, 170, 0); color: rgb(255, 255, 255); }
QMenu::item:selected { background-color: rgb(0, 85, 127); color: rgb(255, 255, 255);}
QListView { background-color: white; background-image: url(:/mainWindow/resources/tile.png) repeat;} 
)";

static const QColor		gl_shadow_color{ 0, 0, 0, 80 };
static constexpr int	gl_shadow_offset{ 5 };

static constexpr QColor gl_my_color_1{ 216, 235, 255 };
static const QColor		gl_my_color_2{ 240, 240, 255 };

static const QColor		gl_their_color_1{ 240, 240, 240 };
static const QColor		gl_their_color_2{ 255, 255, 255 };

static const QColor		gl_their_color{ 255, 255, 255 };
static const QColor		gl_my_color_selected{ 201, 220, 250 };
static const QColor		gl_their_color_selected{ 250, 250, 250 };

static const QColor		gl_my_border_color{ 180, 180, 180 };
static const QColor		gl_their_border_color{ 180, 180, 180 };
static const QColor		gl_border_color_selected{ 80, 80, 80 };

static constexpr QColor gl_text_color{ 20, 20, 20 };
static constexpr QColor gl_name_color{ 25, 109, 153 };
static constexpr QColor gl_time_color{ 160, 160, 160 };

static const QFont		gl_msg_font{ "Titilium Web", 12, QFont::Normal };
static const QFont		gl_name_font{ "Segoe UI", 12, QFont::DemiBold };
static const QFont		gl_time_font{ "Segoe UI", 8, QFont::Light };
static const QFont		gl_likes_font{ "Segoe UI", 10, QFont::DemiBold };

static const QPoint		gl_content_margins{ 10, 10 };

static const QSize		gl_avatar_size = { 32, 32 };
static constexpr int	gl_name_time_line_height{ 30 };
static constexpr int	gl_avatar_margin_right{ 10 };
static constexpr int	gl_textbox_top_margin{ 10 };

static constexpr int	gl_border_radius{ 5 };

static const QSize		gl_likes_box_size { 28, 50 };
static constexpr int	gl_likes_icon_width = 16;
static const QPoint		gl_likes_margins { 6, 6 };

static constexpr int	gl_likes_text_width = 20;
static const QColor		gl_likes_text_color = { 255, 255, 255 };
static const QColor		gl_likes_box_color{ 0, 0, 255, 150  };
static const QColor		gl_dislikes_box_color{ 255, 0, 0, 150  };

static const QSize		gl_action_box_size{ 80, 24  };
static constexpr QColor gl_action_box_color{ 200, 200, 200  };
static int				gl_action_box_offset{ gl_content_margins.x() };
static constexpr int	gl_action_icon_width = 16;
static const QPoint		gl_action_icon_margins = { 6, 4 };
static const QSize		gl_action_icon_size = { 16, 16 };


static const QSize gl_image_preview_size_max = { 250, 250 };
static constexpr int gl_image_preview_spacing = { 15 };
static const QColor gl_image_preview_bg_color = { 220, 220, 220 };
