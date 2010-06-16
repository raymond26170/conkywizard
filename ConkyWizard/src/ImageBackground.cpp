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
#include "ImageBackground.h"

#include <QDir>

// _____________________________________________________________________________
// CONSTRUCTORES Y DESTRUCTOR.

ImageBackground::ImageBackground(QObject *parent) : QObject(parent)
{
	offset = 20;

	colorLine.setRgb(255, 255, 255);
	colorDots.setRgb(224, 223, 222);
	colorBackground.setRgb(57, 56, 52);

	transparency = false;
	alpha = 255;

	pixmapLogoUbuntu.load(":/graphics/LogoUbuntu.png");
	pixmapLogoCircle.load(":/graphics/LogoCircle.png");
}

ImageBackground::~ImageBackground()
{

}

// _____________________________________________________________________________
// MÉTODOS.

QPixmap* ImageBackground::NewImage(Qt::AlignmentFlag alignment, int width, int height)
{
	QPixmap* image = new QPixmap(width, height);
	image->fill(Qt::transparent);

	QPainter painter(image);
	QColor color = colorBackground;
	if(transparency == true) color.setAlpha(alpha);

	switch (alignment){
	case Qt::AlignTop:{
			QRect rect(0, 0 , width, height - offset);

			// Fondo de la franja.
			painter.fillRect(rect, QBrush(color));

			// Linea blanca.
			painter.fillRect(rect.left(), rect.bottom() - 4, rect.width(), 4, QBrush(colorLine));

			// Puntitos.
			QLinearGradient gradient(rect.topLeft(), rect.topRight());
			gradient.setColorAt(0.5, QColor(colorDots.red(), colorDots.green(), colorDots.blue(), 0));
			gradient.setColorAt(1.0, QColor(colorDots.red(), colorDots.green(), colorDots.blue(), 100));
			painter.fillPath(drawDots(rect), gradient);

			// Logotipo ubuntu.
			painter.drawPixmap(15, rect.top() + 10, pixmapLogoUbuntu);

			// Logotipo círculo.
			painter.drawPixmap(width - 20 - pixmapLogoCircle.width(), rect.bottom() - pixmapLogoCircle.height() + offset, pixmapLogoCircle);
			break;
		}
	case Qt::AlignLeft:{
			QRect rect(0, 0 , width - offset, height);

			// Fondo de la franja.
			painter.fillRect(rect, QBrush(color));

			// Linea blanca.
			painter.fillRect(rect.right() - 4, rect.top(), 4, rect.height(), QBrush(colorLine));

			// Puntitos.
			QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
			gradient.setColorAt(0.5, QColor(colorDots.red(), colorDots.green(), colorDots.blue(), 0));
			gradient.setColorAt(1.0, QColor(colorDots.red(), colorDots.green(), colorDots.blue(), 100));
			painter.fillPath(drawDots(rect), gradient);

			// Logotipo ubuntu.
			painter.drawPixmap(15, rect.top() + 10, pixmapLogoUbuntu);

			// Logotipo círculo.
			painter.drawPixmap(rect.right() - pixmapLogoCircle.width() + offset, rect.bottom() - 20 - pixmapLogoCircle.height(), pixmapLogoCircle);
			break;
		}
	case Qt::AlignRight:{
			QRect rect(offset, 0 , width - offset, height);

			// Fondo de la franja.
			painter.fillRect(rect, QBrush(color));

			// Linea blanca.
			painter.fillRect(rect.left(), rect.top(), 4, rect.height(), QBrush(colorLine));

			// Puntitos.
			QLinearGradient gradient(rect.topRight(), rect.bottomRight());
			gradient.setColorAt(0.5, QColor(colorDots.red(), colorDots.green(), colorDots.blue(), 0));
			gradient.setColorAt(1.0, QColor(colorDots.red(), colorDots.green(), colorDots.blue(), 100));
			painter.fillPath(drawDots(rect), gradient);

			// Logotipo ubuntu.
			painter.drawPixmap(15 + offset, rect.top() + 10, pixmapLogoUbuntu);

			// Logotipo círculo.
			painter.drawPixmap(rect.left() - offset, rect.bottom() - 20 - pixmapLogoCircle.height(), pixmapLogoCircle);
			break;
		}
	default:{ // Qt::AlignBottom
			QRect rect(0, offset, width, height - offset);

			// Fondo de la franja.
			painter.fillRect(rect, QBrush(color));

			// Linea blanca.
			painter.fillRect(rect.left(), rect.top(), rect.width(), 4, QBrush(colorLine));

			// Puntitos.
			QLinearGradient gradient(rect.bottomLeft(), rect.bottomRight());
			gradient.setColorAt(0.5, QColor(colorDots.red(), colorDots.green(), colorDots.blue(), 0));
			gradient.setColorAt(1.0, QColor(colorDots.red(), colorDots.green(), colorDots.blue(), 100));
			painter.fillPath(drawDots(rect), gradient);

			// Logotipo ubuntu.
			painter.drawPixmap(15, 10 + offset, pixmapLogoUbuntu);

			// Logotipo círculo.
			painter.drawPixmap(rect.width() - 20 - pixmapLogoCircle.width(), 0, pixmapLogoCircle);
			break;
		}
	}

	// Creamos la ruta.
	QDir dir;
	dir.mkpath(QDir::homePath() + "/.ConkyWizardTheme/pix/");

	// Guardamos la imagen.
	image->save(QDir::homePath() + "/.ConkyWizardTheme/pix/background.png", "PNG");
	return image;
}

QPainterPath ImageBackground::drawDots(QRect rect)
{
	quint32 point = 2;
	quint32 separation = 10;

	QPainterPath path;

	for(int x = rect.left() + separation; x < rect.right(); x += separation){
		for(int y = rect.top() + separation; y < rect.bottom(); y += separation){
			path.addRect(x, y, point, point);
		}
	}

	return path;
}

// _____________________________________________________________________________
// MÉTODOS DE DEFINICIÓN DEL OPERADOR "<<" Y ">>" EN LOS QDataStream.

//     ____________           __  __
//    /           /  __|\    /  \/__\___
//   / DATOS DEL /  |    \  |           |
//  /  PROGRAMA /   |__  /  |  ARCHIVO  |
// /___________/       |/   |___________|
/*!
	Implementación del operador "<<" para escribir datos al archivo
	mediante QDataSream, para lograr la serialización de la clase.
	\see ImageBackground
*/
QDataStream &operator<<(QDataStream &out, const ImageBackground &imageBackground)
{
	out << imageBackground.Offset();
	out << imageBackground.ColorLine()
		<< imageBackground.ColorDots()
		<< imageBackground.ColorBackground();
	out << imageBackground.Transparency()
		<< imageBackground.Alpha();

	return out;
}

//     ____________           __  __
//    /           /  /|__    /  \/__\___
//   / DATOS DEL /  /    |  |           |
//  /  PROGRAMA /   \  __|  |  ARCHIVO  |
// /___________/     \|     |___________|
/*!
	Implementación del operador ">>" de leer de datos del archivo
	mediante QDataSream, para lograr la serialización de la clase.
	\see ImageBackground
*/
QDataStream &operator>>(QDataStream &in, ImageBackground &imageBackground)
{
	int offset;
	QColor colorLine;
	QColor colorDots;
	QColor colorBackground;
	bool transparency;
	int alpha;

	in >> offset;
	in >> colorLine
	   >> colorDots
	   >> colorBackground;
	in >> transparency
	   >> alpha;

	//imageBackground = new ImageBackground;
	imageBackground.setOffset(offset);
	imageBackground.setColorLine(colorLine);
	imageBackground.setColorDots(colorDots);
	imageBackground.setColorBackground(colorBackground);
	imageBackground.setTransparency(transparency);
	imageBackground.setAlpha(alpha);

	return in;
}
