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
#ifndef WIDGETCONKY_H
#define WIDGETCONKY_H

#include <QObject>
#include <QTextStream>

class DockConky;

class WidgetConky : public QObject
{
	Q_OBJECT
	Q_ENUMS(ETypeWidget)
	Q_PROPERTY(bool enabled READ Enabled WRITE setEnabled)
	Q_PROPERTY(QString name READ Name WRITE setName)
	Q_PROPERTY(ETypeWidget typeWidget READ TypeWidget)

public:
	enum ETypeWidget{ OtherType, JumpType, SpaceType, TitleType, NormalType };

	WidgetConky(QString _name = NULL, QObject *parent = 0);

	void setEnabled(bool _enabled);
	bool Enabled() const;
	void setName(const QString _name);
	QString Name() const;
	ETypeWidget TypeWidget() const;

	virtual void WriteTheLine(DockConky* dockConky, int position, int space, QTextStream &out) = 0;

private:
	bool enabled;
	QString name;

protected:
	ETypeWidget typeWidget;
};

#endif // WIDGETCONKY_H
