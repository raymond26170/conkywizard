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
#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QString>

#include "Wizard.h"

#include <stdio.h>
using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qApp->setApplicationName("ConkyWizard");
	qApp->setApplicationVersion("1.0 Beta 1");
	qApp->setOrganizationName("Copyright (C) 2010 José David Abad García");

	QTranslator translator;
	bool isLoad = translator.load(QString(":/translations/ConkyWizard_%1").arg(QLocale::system().name()));
	if(isLoad) a.installTranslator(&translator);

	if (argc == 2) {
		printf("Los argumentos son: %s\n", argv[1]);
	}

	Wizard w;
	w.show();
	return a.exec();
	return 0;
}
