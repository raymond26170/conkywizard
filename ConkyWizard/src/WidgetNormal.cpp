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
#include "WidgetNormal.h"

//QUITALO LUEGO
#include <QDir>
#include <QFile>

// _____________________________________________________________________________
// CONSTRUCTORES Y DESTRUCTOR.

WidgetNormal::WidgetNormal(QString _name, QString _question, QString _response, int _ratio,
						   QObject *parent) :
		WidgetConky(_name, parent), question(_question), response(_response), ratio(_ratio)
{
	typeWidget = NormalType;

	isScript = false;
	typeScript = ScriptBash;
}

WidgetNormal::WidgetNormal(QObject *parent) : WidgetConky(NULL, parent), ratio(50)
{
	typeWidget = NormalType;

	isScript = false;
	typeScript = ScriptBash;
}

WidgetNormal::~WidgetNormal() {}

// _____________________________________________________________________________
// MÉTODOS.

void WidgetNormal::WriteTheLine(DockConky* dockConky, int position, int space, QTextStream &out)
{
	Q_UNUSED(dockConky)

	if(ratio > 0)
		out << QString("${GOTO %1}").arg(position) << question;
	if(ratio < 100)
		out << QString("${GOTO %1}").arg(position+((int)(ratio*space/100))) << response;
}

void WidgetNormal::WriteTheScript(QTextStream &out)
{
	out << script << endl;
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
	\see WidgetNormal
*/
QDataStream &operator<<(QDataStream &out, const WidgetNormal &widgetNormal)
{
	out << widgetNormal.Name ()
		<< widgetNormal.Question ()
		<< widgetNormal.Response ()
		<< widgetNormal.Ratio ()
		<< widgetNormal.IsScript ()
		<< widgetNormal.NameScript ()
		<< widgetNormal.TypeScript ()
		<< widgetNormal.Script ();

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
	\see WidgetNormal
*/
QDataStream &operator>>(QDataStream &in, WidgetNormal &widgetNormal)
{
	QString name;
	QString question;
	QString response;
	int ratio;
	bool isScript;
	int typeScript;
	QString nameScript;
	QString script;

	in >> name
	   >> question
	   >> response
	   >> ratio
	   >> isScript
	   >> nameScript
	   >> typeScript
	   >> script;

	widgetNormal.setName (name);
	widgetNormal.setQuestion (question);
	widgetNormal.setResponse (response);
	widgetNormal.setRatio (ratio);
	widgetNormal.setIsScript (isScript);
	widgetNormal.setNameScript (nameScript);
	widgetNormal.setTypeScript (WidgetNormal::ETypeScript(typeScript));
	widgetNormal.setScript(script);

	return in;
}
