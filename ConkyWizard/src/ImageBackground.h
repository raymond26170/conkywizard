/* *****************************************************************************
 * C·O·N·K·Y  W·I·Z·A·R·D
 *
 * Copyright (C) 2010 José David Abad García
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Boston, MA 02111.
 * ************************************************************************** */
#ifndef IMAGEBACKGROUND_H
#define IMAGEBACKGROUND_H

#include <QPixmap>
#include <QPainter>

class ImageBackground : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int offset READ Offset WRITE setOffset)

	Q_PROPERTY(QColor colorLine READ ColorLine WRITE setColorLine)
	Q_PROPERTY(QColor colorDots READ ColorDots WRITE setColorDots)
	Q_PROPERTY(QColor colorBackground READ ColorBackground WRITE setColorBackground)

	Q_PROPERTY(bool transparency READ Transparency WRITE setTransparency)
	Q_PROPERTY(int alpha READ Alpha WRITE setAlpha)

public:
	ImageBackground(QObject *parent = 0);
	~ImageBackground();

	void setOffset(int _offset) { offset = _offset; }
	int Offset() const { return offset; }

	void setColorLine(const QColor _colorLine) { colorLine = _colorLine; }
	QColor ColorLine() const { return colorLine; }
	void setColorDots(const QColor _colorDots) { colorDots = _colorDots; }
	QColor ColorDots() const { return colorDots; }
	void setColorBackground(const QColor _colorBackground) { colorBackground = _colorBackground; }
	QColor ColorBackground() const { return colorBackground; }

	void setTransparency(bool _transparency) { transparency = _transparency; }
	bool Transparency() const { return transparency; }
	void setAlpha(int _alpha) { alpha = _alpha; }
	int Alpha() const { return alpha; }

	QPixmap* NewImage(Qt::AlignmentFlag alignment, int width, int height);

private:
	int offset;

	QColor colorLine;
	QColor colorDots;
	QColor colorBackground;

	bool transparency;
	int alpha;

	QPixmap pixmapLogoUbuntu;
	QPixmap pixmapLogoCircle;

	QPainterPath drawDots(QRect rect);
};

QDataStream &operator<<(QDataStream &out, const ImageBackground &imageBackground);
QDataStream &operator>>(QDataStream &in, ImageBackground &imageBackground);

#endif // IMAGEBACKGROUND_H
