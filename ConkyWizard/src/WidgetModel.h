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
#ifndef WIDGETMODEL_H
#define WIDGETMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include <QBrush>
#include <QStringList>
#include <QList>
#include "WidgetConky.h"

class WidgetModel : public QAbstractListModel
{
public:
	WidgetModel(QObject *parent = 0);
	~WidgetModel();

	void setWidgets(QList<WidgetConky*>* _widgets);

	int rowCount(const QModelIndex &parent) const;

	QVariant data(const QModelIndex &workIndex, int role) const;
	bool setData(const QModelIndex &workIndex, const QVariant &value, int role);

	Qt::ItemFlags flags(const QModelIndex &workIndex) const;

	bool insertRows(int position, int rows, const QModelIndex &workIndex = QModelIndex());
	bool removeRows(int position, int rows, const QModelIndex &workIndex = QModelIndex());

	bool insertWidget(int position, const QModelIndex &workIndex,  WidgetConky *widgetConky);
	bool removeWidget(int position, const QModelIndex &workIndex);
	bool moveUpWidget(int position, const QModelIndex &workIndex);
	bool moveDownWidget(int position, const QModelIndex &workIndex);

private:
	QIcon *iconSpace;
	QIcon *iconJump;
	QIcon *iconTitle;
	QIcon *iconNormal;
	QIcon *iconOther;

	QList<WidgetConky*>* widgets;
};

#endif // WIDGETMODEL_H
