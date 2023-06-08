#pragma once

#include <QString>
#include <QPainter>

class MyFont
{
    MyFont();
public:
    MyFont(const QString& font_name_, int nickname_size_);
    int getWidth(const QString& string_) const;
    int getHeight() const;
    QFont getFont() const;

private:
    QString font_name;
    int size;
    QFont font;
    int mes_font_width;
    int mes_font_height;
    void setWidth(const QString& string_);
};

