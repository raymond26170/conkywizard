/* *****************************************************************************
 * C·O·N·K·Y  W·I·Z·A·R·D
 *
 * Copyright (C) 2010 José David Abad García
 * GPL Version 3
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
#include "DockConky.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

#include "WidgetConky.h"
#include "WidgetJump.h"
#include "WidgetSpace.h"
#include "WidgetTitle.h"
#include "WidgetNormal.h"

// _____________________________________________________________________________
// CONSTRUCTORES Y DESTRUCTOR.

DockConky::DockConky(QObject *parent) : QObject(parent)
{
	alignment = Qt::AlignBottom;

	offset = 20;
	edge = 16;
	separation = 16;
	space = 240;

	displacementX = displacementY = 0;
	width = height = 256;

	colorTitle.setRgb(221, 58, 33);
	colorText.setRgb(224, 223, 222);

	fontTitle.setBold(true);
	fontTitle.setPointSize(12);
	fontText.setPointSize(10);

	delay = 8;
}

DockConky::~DockConky()
{

}

// _____________________________________________________________________________
// MÉTODOS DE INICIALIZACIÓN.

void DockConky::BasicWidgets()
{
	// Widgets de sistema.
	widgets.append(new WidgetSpace);
	widgets.append(new WidgetSpace);
	widgets.append(new WidgetNormal(tr("Kernell"), tr("Kernell:"), "${kernel}"));
	widgets.append(new WidgetNormal(tr("CPU"), tr("CPU:"), "${cpubar cpu1 10,75} ${cpu cpu1} %"));
	widgets.append(new WidgetNormal(tr("RAM"), tr("RAM:"), "${membar 10,75} ${memperc} %"));
	widgets.append(new WidgetNormal(tr("Swap"), tr("SWAP:"), "${swapbar 10,75}"));
	widgets.append(new WidgetNormal(tr("Uptime"), tr("Uptime:"), "${uptime}"));
	widgets.append(new WidgetJump);

	// Widgets de discos.
	widgets.append(new WidgetTitle(tr("Disks")));
	widgets.append(new WidgetSpace);
	widgets.append(new WidgetNormal(tr("Disk system used"), tr("System (/):"), "${fs_used /}"));
	widgets.append(new WidgetNormal(tr("Disk system bar"), QString::null, "${fs_bar 10,100 /}", 10));
	widgets.append(new WidgetNormal(tr("Disk home used"), tr("Home (/home):"), "${fs_used /home}"));
	widgets.append(new WidgetNormal(tr("Disk home bar"), QString::null, "${fs_bar 10,100 /home}", 10));
	widgets.append(new WidgetJump);

	// Widgets de red.
	widgets.append(new WidgetTitle(tr("Network")));
	widgets.append(new WidgetSpace);
	widgets.append(new WidgetNormal(tr("Network upspeed"), tr("Upspeed:"), "${upspeedgraph eth0 10,75 B7B2AD B7B2AD}"));
	widgets.append(new WidgetNormal(tr("Network downspeed"), tr("Downspeed:"), "${downspeedgraph eth0 10,75 B7B2AD B7B2AD}"));
	widgets.append(new WidgetNormal(tr("Network total uploaded"), tr("Uploaded:"), "${totalup eth0}"));
	widgets.append(new WidgetNormal(tr("Network total downloaded"), tr("Downloaded:"), "${totaldown eth0}"));
	widgets.append(new WidgetNormal(tr("Network local IP"), tr("Local IP:"), "${addr eth0}"));
	WidgetNormal *widgetPublicIp = new WidgetNormal(tr("Network public IP"), tr("Public IP:"), "${execi 10000 ~/.ConkyWizardTheme/scripts/ip.py}");
	widgetPublicIp->setIsScript(true);
	widgetPublicIp->setNameScript("ip");
	widgetPublicIp->setTypeScript(WidgetNormal::ScriptBash);
	widgetPublicIp->setScript("#!/bin/bash\nwget http://checkip.dyndns.org/ -q -O - |\ngrep -Eo '\\<[[:digit:]]{1,3}(\\.[[:digit:]]{1,3}){3}\\>'\n");
	widgets.append(widgetPublicIp);
	widgets.append(new WidgetJump);

	// Widgets de temperatura.
	widgets.append(new WidgetTitle(tr("Temperatures")));
	widgets.append(new WidgetSpace);
	widgets.append(new WidgetNormal(tr("Temperature CPU"), tr("CPU:"), "${execi 4 sensors | grep -A 0 'temp2' | cut -c15-18} ºC"));
	widgets.append(new WidgetNormal(tr("Temperature hard disk"), tr("Hard disk:"), "${execi 4 sensors | grep -A 0 'temp1' | cut -c15-18} ºC"));
	widgets.append(new WidgetJump);

	// Widgets de fecha.
	widgets.append(new WidgetTitle(tr("Time and date")));
	widgets.append(new WidgetSpace);
	widgets.append(new WidgetNormal(tr("Time and date in short format"), "${time %H:%M}  ${time %d/%m/%Y}", QString::null, 100));
	widgets.append(new WidgetNormal(tr("Date in long format"), "${time %A}, ${time %d} ${time %B} ${time %Y}", QString::null, 100));
}

void DockConky::DisableWidgetsScripts()
{
	for(int i = 0; i < widgets.count(); i++){
		if(widgets.at(i)->TypeWidget() == WidgetConky::NormalType){
			WidgetNormal *widgetNormal = dynamic_cast<WidgetNormal*>(widgets.at(i));
			if(widgetNormal->IsScript()) widgetNormal->setEnabled(false);
		}
	}
}

// _____________________________________________________________________________
// MÉTODOS PARA EL MANEJO DE LOS WIDGETS.

WidgetConky* DockConky::at(int i) const
{
	return widgets.at(i);
}
void DockConky::addWidget(WidgetConky *widgetConky)
{
	widgets.append(widgetConky);
}

void DockConky::insert(int i, WidgetConky *widgetConky)
{
	widgets.insert(i, widgetConky);
}

void DockConky::move(int from, int to)
{
	widgets.move(from, to);
}

void DockConky::removeWidget(int i)
{
	widgets.removeAt(i);
}

void DockConky::removeWidget(WidgetConky *widgetConky)
{
	widgets.removeOne(widgetConky);
}

// _____________________________________________________________________________
// MÉTODOS PARA LA CREACIÓN DE LOS ARCHIVOS.

void DockConky::NewLaunchFile()
{
	// Guardamos el archivo del lanzador.
	QFile file(QDir::homePath() + "/.ConkyWizardLaunch");
	if (!file.open(QIODevice::WriteOnly)) return;

	QTextStream out(&file);
	out << "#!/bin/bash" << endl;
	out << QString("sleep %1 && conky -c ~/.ConkyWizardTheme/ConkyWizardTheme").arg(delay) << endl;

	out.flush();

	file.close();

	file.setPermissions(QFile::ReadUser	| QFile::WriteUser | QFile::ExeUser	|
						QFile::ReadGroup | QFile::ExeGroup |
						QFile::ReadOther | QFile::ExeOther);
}

void DockConky::NewDockFile()
{
	// Creamos la ruta.
	QDir dir;
	dir.mkpath(QDir::homePath() + "/.ConkyWizardTheme/");

	// Guardamos el archivo de configuración.
	QFile file(QDir::homePath() + "/.ConkyWizardTheme/ConkyWizardTheme");
	if (!file.open(QIODevice::WriteOnly)) return;

	QTextStream out(&file);
	out << "# Generated with Conky Wizard" << endl;
	out << "# Copyright (C) 2010 José David Abad García" << endl;
	out << "# GPL Version 3" << endl;
	out << "# "<< endl;
	out << "# WARNING! All changes made in this file will be lost if the program runs again!" << endl;
	out << endl;
	out << "# Default Fonts" << endl;
	out << "use_xft yes" << endl;
	out << QString("xftfont %1").arg(Font2ConkyString(fontText)) << endl;
	out << "override_utf8_locale yes" << endl;
	out << endl;
	out << "# Performance Settings" << endl;
	out << "update_interval 1" << endl;
	out << "total_run_times 0" << endl;
	out << "double_buffer yes" << endl;
	out << "no_buffers yes" << endl;
	out << "net_avg_samples 2" << endl;
	out << "text_buffer_size 1024" << endl;
	out << endl;
	out << "# Window Settings" << endl;
	out << "own_window yes" << endl;
	out << "own_window_transparent yes" << endl;
	out << "own_window_type override" << endl;
	out << "own_window_hints undecorated,below,sticky,skip_taskbar,skip_pager" << endl;
	out << endl;
	out << "# Window border" << endl;
	out << "draw_borders no" << endl;
	out << "draw_shades no" << endl;
	out << endl;
	out << "# Default Color" << endl;
	out << QString("default_color %1").arg(colorText.name().remove('#').toUpper()) << endl;
	out << endl;
	out << "# Color Title." << endl;
	out << QString("color0 %1").arg(colorTitle.name().remove('#').toUpper()) << endl;
	out << endl;

	out << "# Size and position" << endl;
	out << QString("minimum_size %1 %2").arg(width).arg(height) << endl;
	out << QString("gap_x %1").arg(displacementX) << endl;
	out << QString("gap_y %1").arg(displacementY) << endl;
	switch(alignment){
	case Qt::AlignTop:
		out << "alignment top_left" << endl;
		out << endl;
		out << "TEXT" << endl;
		out << QString("${image ~/.ConkyWizardTheme/pix/background.png -p 0,0 -s %1x%2}").arg(width).arg(height) << endl;
		WidgetsHorizontallyPlaced(out);
		break;
	case Qt::AlignLeft:
		out << "alignment top_left" << endl;
		out << endl;
		out << "TEXT" << endl;
		out << QString("${image ~/.ConkyWizardTheme/pix/background.png -p 0,0 -s %1x%2}").arg(width).arg(height) << endl;
		WidgetsVerticallyPlaced(out);
		break;
	case Qt::AlignRight:
		out << "alignment top_right" << endl;
		out << endl;
		out << "TEXT" << endl;
		out << QString("${image ~/.ConkyWizardTheme/pix/background.png -p 0,0 -s %1x%2}").arg(width).arg(height) << endl;
		WidgetsVerticallyPlaced(out);
		break;
	default: // Qt::AlignBottom:
		out << "alignment bottom_left" << endl;
		out << endl;
		out << "TEXT" << endl;
		out << QString("${image ~/.ConkyWizardTheme/pix/background.png -p 0,0 -s %1x%2}").arg(width).arg(height) << endl << endl;
		WidgetsHorizontallyPlaced(out);
		break;
	}
	out.flush();

	file.close();
}

void DockConky::NewDockScripts()
{
	// Escribimos los elementos.
	for(int i = 0; i < widgets.count(); ++i){
		if(WidgetNormal *normal = dynamic_cast<WidgetNormal*> (widgets[i]))
			if(normal->IsScript() && !normal->NameScript().isEmpty()){
				// Creamos la ruta.
				QDir dir;
				dir.mkpath(QDir::homePath() + "/.ConkyWizardTheme/scripts");

				QString extension;
				switch(normal->TypeScript()){
				case WidgetNormal::ScriptBash:
					extension = ".sh";
					break;
				default: //case WidgetNormal::ScriptPython:
					extension = ".py";
					break;
				}

				// Guardamos el script.
				QFile file(QDir::homePath() + "/.ConkyWizardTheme/scripts/" + normal->NameScript() + extension);
				if (!file.open(QIODevice::WriteOnly)) return;
				QTextStream out(&file);
				normal->WriteTheScript(out);
				out.flush();

				file.close();

				file.setPermissions(QFile::ReadUser	| QFile::WriteUser | QFile::ExeUser	|
									QFile::ReadGroup | QFile::ExeGroup |
									QFile::ReadOther | QFile::ExeOther);
			}
	}
}

// _____________________________________________________________________________
// MÉTODOS PARA ESCRIBIR LOS WIDGETS HORIZONTALES O VERTICALES.

void DockConky::WidgetsHorizontallyPlaced(QTextStream &out)
{
	int column = 0;
	int columnCount = 0;
	int row = 0;
	int rowCount = 0;

	//Contamos el número de columnas y fila máxima.
	for(int i = 0; i < widgets.count(); ++i){
		if(WidgetJump *jump = dynamic_cast<WidgetJump*> (widgets[i])){
			Q_UNUSED(jump);
			rowCount = rowCount > row? rowCount: row;
			row = 0;
			++columnCount;
		} else ++row;
	}
	++columnCount;

	// Creamos las columnas.
	QList<WidgetConky*> tableWidgets[columnCount];

	// Llenamos las columnas.
	column = 0;
	for(int i = 0; i < widgets.count(); ++i){
		if(WidgetJump *jump = dynamic_cast<WidgetJump*> (widgets[i])){
			Q_UNUSED(jump);
			++column;
		} else {
			tableWidgets[column].append(widgets[i]);;
		}
	}

	// Preparamos la geometría.
	int iOffset1 = alignment == Qt::AlignTop? 0: offset + edge;
	int iOffset2 = iOffset1;

	// Escribimos los elementos.
	for(int i = 0; i < rowCount; ++i){
		for(int j = 0; j < columnCount; ++j){
			if(tableWidgets[j].count() > i && tableWidgets[j][i]->Enabled()) tableWidgets[j][i]->WriteTheLine(this, iOffset2, space, out);
			iOffset2 += space + separation;
		}
		out << endl;
		iOffset2 = iOffset1;
	}
}

void DockConky::WidgetsVerticallyPlaced(QTextStream &out)
{
	// Preparamos la geometría.
	int iOffset = alignment == Qt::AlignLeft? 0: offset;

	int iSpace = (int) (width - 2 * edge - offset);
	iSpace = iSpace > space? iSpace: space;

	// Escribimos los elementos.
	for(int i = 0; i < widgets.count(); ++i){
		if(widgets[i]->Enabled()) widgets[i]->WriteTheLine(this, iOffset + edge, iSpace, out);
		out << endl;
	}
}

// _____________________________________________________________________________
// MÉTODOS AUXILIARES.

QString DockConky::Font2ConkyString(QFont font)
{
	QString result(font.family());

	if(font.bold()) result += ":bold";
	if(font.italic()) result += ":italic";

	result += QString(":size=%1").arg(font.pointSize());

	return result;
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
	\see DockConky
*/
QDataStream &operator<<(QDataStream &out, const DockConky &dockConky)
{
	out << dockConky.Alignment();

	out << dockConky.Edge()
		<< dockConky.Separation()
		<< dockConky.Space();

	out << dockConky.Offset()
		<< dockConky.DisplacementX()
		<< dockConky.DisplacementY()
		<< dockConky.Width()
		<< dockConky.Height();

	out << dockConky.ColorTitle()
		<< dockConky.ColorText();

	out << dockConky.FontTitle()
		<< dockConky.FontText();

	out << dockConky.Delay();

	out << dockConky.count();

	for(int i = 0; i < dockConky.count(); i++){
		WidgetConky* widgetConky = dockConky.at(i);
		out << widgetConky->TypeWidget();

		if(widgetConky->TypeWidget() == WidgetConky::TitleType){
			WidgetTitle *widgetTitle = dynamic_cast<WidgetTitle*>(widgetConky);
			out << *widgetTitle;
		} else if(widgetConky->TypeWidget() == WidgetConky::NormalType){
			WidgetNormal *widgetNormal = dynamic_cast<WidgetNormal*>(widgetConky);
			out << *widgetNormal;
		}

		printf("\n");
	}

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
	\see DockConky
*/
QDataStream &operator>>(QDataStream &in, DockConky &dockConky)
{
	int alignment;
	int edge;
	int separation;
	int space;
	int offset;
	int displacementX;
	int displacementY;
	int width;
	int height;
	QColor colorTitle;
	QColor colorText;
	QFont fontTitle;
	QFont fontText;
	int delay;

	in >> alignment;
	in >> edge
	   >> separation
	   >> space;
	in >> offset
	   >> displacementX
	   >> displacementY
	   >> width
	   >> height;
	in >> colorTitle
	   >> colorText;
	in >> fontTitle
	   >> fontText;
	in >> delay;

	dockConky.setAlignment(Qt::AlignmentFlag(alignment));
	dockConky.setEdge(edge);
	dockConky.setSeparation(separation);
	dockConky.setSpace(space);
	dockConky.setOffset(offset);
	dockConky.setDisplacementX(displacementX);
	dockConky.setDisplacementY(displacementY);
	dockConky.setWidth(width);
	dockConky.setHeight(height);
	dockConky.setColorTitle(colorTitle);
	dockConky.setColorText(colorText);
	dockConky.setFontTitle(fontTitle);
	dockConky.setFontText(fontText);
	dockConky.setDelay(delay);

	int count;
	in >> count;
	int typeWidget;
	for(int i = 0; i < count; i++){
		in >> typeWidget;

		if(typeWidget == WidgetConky::JumpType){
			dockConky.addWidget(new WidgetJump);
		} else if(typeWidget == WidgetConky::SpaceType){
			dockConky.addWidget(new WidgetSpace);
		} else if(typeWidget == WidgetConky::TitleType){
			WidgetTitle *widgetTitle = new WidgetTitle;
			in >> *widgetTitle;
			dockConky.addWidget(widgetTitle);
		} else if(typeWidget == WidgetConky::NormalType){
			WidgetNormal *widgetNormal = new WidgetNormal;
			in >> *widgetNormal;
			dockConky.addWidget(widgetNormal);
		}
	}

	return in;
}
