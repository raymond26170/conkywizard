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
#ifndef WIDGETTITLE_H
#define WIDGETTITLE_H

#include "WidgetConky.h"

class WidgetTitle : public WidgetConky
{
	Q_PROPERTY(QString title READ Title WRITE setTitle)

public:
	WidgetTitle(QString _name, QObject *parent = 0);
	WidgetTitle(QObject *parent = 0);
	~WidgetTitle();

	void setTitle(const QString _title) { setName(_title); title = _title; }
	QString Title() const { return title; }

	 void WriteTheLine(DockConky* dockConky, int position, int space, QTextStream &out);

private:
	QString title;
};

QDataStream &operator<<(QDataStream &out, const WidgetTitle &widgetTitle);
QDataStream &operator>>(QDataStream &in, WidgetTitle &widgetTitle);

#endif // WIDGETTITLE_H
