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
#include "WidgetModel.h"
#include <QMimeData>
#include <QApplication>
#include <QPalette>

// _____________________________________________________________________________
// CONSTRUCTORES Y DESTRUCTOR.

WidgetModel::WidgetModel(QObject *parent) : QAbstractListModel(parent)
{
	iconSpace = new QIcon(":/graphics/IconSpace.png");
	iconJump = new QIcon(":/graphics/IconJump.png");
	iconTitle = new QIcon(":/graphics/IconTitle.png");
	iconNormal = new QIcon(":/graphics/IconNormal.png");
	iconOther = new QIcon(":/graphics/IconOther.png");
}

WidgetModel::~WidgetModel()
{
	delete iconSpace;
	delete iconJump;
	delete iconTitle;
	delete iconNormal;
	delete iconOther;
}

// _____________________________________________________________________________
// MÉTODOS.

void WidgetModel::setWidgets(QList<WidgetConky*>* _widgets)
{
	widgets = _widgets;
	reset();
}

// _____________________________________________________________________________
// MÉTODOS SOBRESCRITOS.

int WidgetModel::rowCount(const QModelIndex &workIndex) const
{
	Q_UNUSED(workIndex)
	return widgets->count();
}

QVariant WidgetModel::data(const QModelIndex &workIndex, int role) const
{
	if (!workIndex.isValid()) return QVariant();

	switch(role){
	case Qt::DecorationRole:
		switch(widgets->at(workIndex.row())->TypeWidget()){
		case WidgetConky::JumpType:
			return *iconJump;
			break;
		case WidgetConky::SpaceType:
			return *iconSpace;
			break;
		case WidgetConky::TitleType:
			return *iconTitle;
			break;
		case WidgetConky::NormalType:
			return *iconNormal;
			break;
		default: // case WidgetConky::OtherType:
			return *iconOther;
			break;
		}
		break;
	case Qt::TextAlignmentRole:
		if(widgets->at(workIndex.row())->TypeWidget() == WidgetConky::JumpType ||
		   widgets->at(workIndex.row())->TypeWidget() == WidgetConky::SpaceType)
			return int(Qt::AlignCenter);
		else return int(Qt::AlignLeft | Qt::AlignVCenter);
		break;
	case Qt::CheckStateRole:
		if(widgets->at(workIndex.row())->Enabled()) return Qt::Checked;
		else return Qt::Unchecked;
		break;
	case Qt::DisplayRole:
		return widgets->at(workIndex.row())->Name();
		break;
	}

	return QVariant();
}

bool WidgetModel::setData(const QModelIndex &workIndex, const QVariant &value, int role)
{
	if (!workIndex.isValid()) return false;

	QModelIndex topLeft = workIndex;
	QModelIndex bottomRight = workIndex;

	switch(role){
	case Qt::CheckStateRole:
		// Actualizamos el nombre internamente.
		widgets->at(workIndex.row())->setEnabled(static_cast<Qt::CheckState>(value.toInt()));
		printf("Doy candela %d\n", value.toInt());

		// Actualizamos la vista.
		topLeft = workIndex;
		bottomRight = workIndex;
		emit dataChanged(topLeft, bottomRight);
		return true;
		break;
	case Qt::EditRole:
		// Actualizamos el nombre internamente.
		widgets->at(workIndex.row())->setName(value.toString());

		// Actualizamos la vista.
		topLeft = workIndex;
		bottomRight = workIndex;
		emit dataChanged(topLeft, bottomRight);

		return true;
		break;
	}

	return false;
}

Qt::ItemFlags WidgetModel::flags(const QModelIndex &workIndex) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(workIndex);
	if (!workIndex.isValid()) return flags;

	// Solo es editable la widgets título y normal.
	if(widgets->at(workIndex.row())->TypeWidget() == WidgetConky::TitleType ||
	   widgets->at(workIndex.row())->TypeWidget() == WidgetConky::NormalType)
		flags |= Qt::ItemIsEditable;

	// Todos los widgets se pueden activar y desactivar.
	flags |= Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;

	return flags;
}

bool WidgetModel::insertRows(int position, int rows, const QModelIndex &workIndex)
{
	Q_UNUSED(workIndex);

	beginInsertRows(QModelIndex(), position, position+rows-1);
	endInsertRows();
	return true;
}

bool WidgetModel::removeRows(int position, int rows, const QModelIndex &workIndex)
{
	Q_UNUSED(workIndex);

	beginRemoveRows(QModelIndex(), position, position+rows-1);
	endRemoveRows();
	return true;
}

// _____________________________________________________________________________
// MÉTODOS.

bool WidgetModel::insertWidget(int position, const QModelIndex &workIndex, WidgetConky *widgetConky)
{
	if(!workIndex.isValid()) return false;

	bool response = insertRows(position, 1);
	if(response) widgets->insert(position, widgetConky);
	return response;
}

bool WidgetModel::removeWidget(int position, const QModelIndex &workIndex)
{
	if(!workIndex.isValid()) return false;

	bool response = removeRows(position, 1);
	if(response) widgets->removeAt(position);
	return response;
}

bool WidgetModel::moveUpWidget(int position, const QModelIndex &workIndex)
{
	if(!workIndex.isValid() || position == 0) return false;

	bool response = beginMoveRows(QModelIndex(), position, position, QModelIndex(), position - 1);
	if(response) widgets->move(position, position - 1);
	endMoveRows();

	return response;
}

bool WidgetModel::moveDownWidget(int position, const QModelIndex &workIndex)
{
	if (!workIndex.isValid() || position == widgets->count() - 1) return false;

	bool response = beginMoveRows(QModelIndex(), position, position, QModelIndex(), position + 2);
	if(response) widgets->move(position, position + 1);
	endMoveRows();

	return response;
}
