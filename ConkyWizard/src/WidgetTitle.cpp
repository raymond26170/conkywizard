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
#include "WidgetTitle.h"
#include "DockConky.h"

// _____________________________________________________________________________
// CONSTRUCTORES Y DESTRUCTOR.

WidgetTitle::WidgetTitle(QString _name, QObject *parent) :
		WidgetConky(_name, parent), title(_name)
{
	typeWidget = TitleType;
}

WidgetTitle::WidgetTitle(QObject *parent) : WidgetConky(NULL, parent)
{
	typeWidget = TitleType;
}

WidgetTitle::~WidgetTitle() {}

// _____________________________________________________________________________
// MÉTODOS.

void WidgetTitle::WriteTheLine(DockConky* dockConky, int position, int space, QTextStream &out)
{
	Q_UNUSED(space)

	out << QString("${GOTO %1}").arg(position);
	out << QString("${font %1").arg(DockConky::Font2ConkyString(dockConky->FontTitle())) << "}${color0}";
	out << title ;
	out << "${font}${color}";
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
	\see WidgetTitle
*/
QDataStream &operator<<(QDataStream &out, const WidgetTitle &widgetTitle)
{
	out << widgetTitle.Title ();

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
	\see WidgetTitle
*/
QDataStream &operator>>(QDataStream &in, WidgetTitle &widgetTitle)
{
	QString title;
	in >> title;

	widgetTitle.setTitle (title);

	return in;
}
