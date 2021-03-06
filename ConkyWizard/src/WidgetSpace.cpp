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
#include "WidgetSpace.h"

// _____________________________________________________________________________
// CONSTRUCTORES Y DESTRUCTOR.

WidgetSpace::WidgetSpace(QObject *parent) : WidgetConky(QString::null, parent)
{
	typeWidget = SpaceType;
	setName(tr("(Newline)"));
}

WidgetSpace::~WidgetSpace() {}

// _____________________________________________________________________________
// MÉTODOS.

void WidgetSpace::WriteTheLine(DockConky* dockConky, int position, int space, QTextStream &out)
{
	Q_UNUSED(dockConky)
	Q_UNUSED(position)
	Q_UNUSED(space)
	Q_UNUSED(out)
}
