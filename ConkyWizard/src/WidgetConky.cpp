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
#include "WidgetConky.h"
#include "DockConky.h"

/*!
	\class WidgetConky
	\brief Clase abstracta base para todo widget Conky

	La clase WidgetConky es una clase abstracta base para crear encima de ella
	todos los demás widgets.

	Esta posee la funcionalidad más básica y la enumeración con los tipos de wigets.
*/

// _____________________________________________________________________________
// CONSTRUCTORES Y DESTRUCTOR.

/*!
	Constructor por defecto.
*/
WidgetConky::WidgetConky(QString _name, QObject *parent) :
		QObject(parent), enabled(true), name(_name), typeWidget(WidgetConky::OtherType) {}

// _____________________________________________________________________________
// PROPIEDADES.

/*!
	\property WidgetConky::Enabled
	\brief Activar el widget

	Esta propiedad establece si el widget será añadido en el resultado final
	Dock de Conky.
*/
void WidgetConky::setEnabled(bool _enabled)
{
	enabled = _enabled;
}

bool WidgetConky::Enabled() const
{
	return enabled;
}

/*!
	\property WidgetConky::Name
	\brief Nombre del widget

	Esta propiedad establece un nombre al widget. Solo se usa este nombre cuando
	se crea la lista de widgets. Es simplemente para poder ordenarlos.
*/
void WidgetConky::setName(const QString _name)
{
	name = _name;
}

QString WidgetConky::Name() const
{
	return name;
}

/*!
	\enum WidgetConky::ETypeWidget

	Tipo de widget Conky a usar.

	\value JumpType		Widget que añade un salto de columna si el dock es horizontal y un salto de linea si el dock es vertical.
	\value SpaceType	Widget que añade un salto de linea.
	\value TitleType	Widget específico para mostrar títulos.
	\value NormalType	Widget compuesto por dos partes ("pregunta" y "respuesta").
	\value OtherType	Cualquier tipo de widget no definido aún, para futuras implementaciones.
*/

/*!
	\property WidgetConky::TypeWidget()
	\brief Tipo de widget

	Esta propiedad devuelve el tipo de widget.
*/
WidgetConky::ETypeWidget WidgetConky::TypeWidget() const
{
	return typeWidget;
}
