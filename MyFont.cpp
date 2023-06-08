#include "MyFont.h"

MyFont::MyFont()
{
}

MyFont::MyFont(const QString& font_name_, int size_)
{
	this->font_name = font_name_;
	this->size = size_;
	this->font = QFont(font_name_, size);
	QFontMetrics font_metrics(this->font);
	this->mes_font_height = font_metrics.height();
	this->mes_font_width = font_metrics.horizontalAdvance("");
}

int MyFont::getWidth(const QString& string_) const
{
	QFontMetrics font_metrics(this->font);
	return font_metrics.horizontalAdvance(string_);;
}

int MyFont::getHeight() const
{
	return this->mes_font_height;
}

QFont MyFont::getFont() const
{
	return this->font;
}

void MyFont::setWidth(const QString& string_) 
{
	QFontMetrics font_metrics(this->font);
	this->mes_font_width = font_metrics.horizontalAdvance(string_);
}
