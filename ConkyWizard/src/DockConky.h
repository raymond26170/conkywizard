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
#ifndef DOCKCONKY_H
#define DOCKCONKY_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <QTextStream>

class WidgetConky;

class DockConky : public QObject
{
	Q_OBJECT
	Q_PROPERTY(Qt::AlignmentFlag alignment READ Alignment WRITE setAlignment)

	Q_PROPERTY(int edge READ Edge WRITE setEdge)
	Q_PROPERTY(int separation READ Separation WRITE setSeparation)
	Q_PROPERTY(int space READ Space WRITE setSpace)

	Q_PROPERTY(int offset READ Offset WRITE setOffset)
	Q_PROPERTY(int displacementX READ DisplacementX WRITE setDisplacementX)
	Q_PROPERTY(int displacementY READ DisplacementY WRITE setDisplacementY)
	Q_PROPERTY(int width READ Width WRITE setWidth)
	Q_PROPERTY(int height READ Height WRITE setHeight)

	Q_PROPERTY(QColor colorTitle READ ColorTitle WRITE setColorTitle)
	Q_PROPERTY(QColor colorText READ ColorText WRITE setColorText)

	Q_PROPERTY(QFont fontTitle READ FontTitle WRITE setFontTitle)
	Q_PROPERTY(QFont fontText READ FontText WRITE setFontText)

	Q_PROPERTY(int delay READ Delay WRITE setDelay)

public:
	DockConky(QObject *parent = 0);
	~DockConky();

	void setAlignment(Qt::AlignmentFlag _alignment) { alignment = _alignment; }
	Qt::AlignmentFlag Alignment() const { return alignment; }

	void setEdge(int _edge) { edge = _edge; }
	int Edge() const { return edge; }
	void setSeparation(int _separation) { separation = _separation; }
	int Separation() const { return separation; }
	void setSpace(int _space) { space = _space; }
	int Space() const { return space; }

	void setOffset(int _offset) { offset = _offset; }
	int Offset() const { return offset; }
	void setDisplacementX(int _displacementX) { displacementX = _displacementX; }
	int DisplacementX() const { return displacementX; }
	void setDisplacementY(int _displacementY) { displacementY = _displacementY; }
	int DisplacementY() const { return displacementY; }
	void setWidth(int _width) { width = _width; }
	int Width() const { return width; }
	void setHeight(int _height) { height = _height; }
	int Height() const { return height; }

	void setColorTitle(const QColor _colorTitle) { colorTitle = _colorTitle; }
	QColor ColorTitle() const { return colorTitle; }
	void setColorText(const QColor _colorText) { colorText = _colorText; }
	QColor ColorText() const { return colorText; }

	void setFontTitle(const QFont _fontTitle) { fontTitle = _fontTitle; }
	QFont FontTitle() const { return fontTitle; }
	void setFontText(const QFont _fontText) { fontText = _fontText; }
	QFont FontText() const { return fontText; }

	void setDelay(int _delay) { delay = _delay; }
	int Delay() const { return delay; }

	void BasicWidgets();
	void DisableWidgetsScripts();

	QList<WidgetConky*>* Widgets() { return &widgets; }

	int count() const { return widgets.count(); }
	WidgetConky* at(int i) const;
	void addWidget(WidgetConky *widgetConky);
	void insert(int i, WidgetConky *widgetConky);
	void move(int from, int to);
	void removeWidget(int i);
	void removeWidget(WidgetConky * widgetConky);

	void NewLaunchFile();
	void NewDockFile();
	void NewDockScripts();

	static QString Font2ConkyString (QFont font);

private:
	Qt::AlignmentFlag alignment;

	int edge, separation, space;
	int offset, displacementX, displacementY, width, height, delay;

	QColor colorTitle;
	QColor colorText;

	QFont fontTitle;
	QFont fontText;

	QList<WidgetConky*> widgets;

	void WidgetsHorizontallyPlaced(QTextStream &out);
	void WidgetsVerticallyPlaced(QTextStream &out);
};

QDataStream &operator<<(QDataStream &out, const DockConky &dockConky);
QDataStream &operator>>(QDataStream &in, DockConky &dockConky);

#endif // DOCKCONKY_H
