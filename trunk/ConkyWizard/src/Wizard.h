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
#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>
#include <QPixmap>
#include <QFile>

class DockConky;
class ImageBackground;
class WidgetModel;

namespace Ui
{
	class Wizard;
}

class Wizard : public QWizard
{
	Q_OBJECT

public:
	explicit Wizard(QWidget *parent = 0);
	explicit Wizard(const QString &url, QWidget *parent = 0);
	~Wizard();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::Wizard *ui;

	void init_Graphics();
	bool init_LoadDefault();
	void init_LoadUrl(const QString url);
	void init_Gui();

	WidgetModel *widgetModel;

	DockConky* dockConky;
	ImageBackground* imageBackground;

	QPixmap screenTop, screenLeft, screenRight, screenDown;
	QPixmap sizeTop, sizeLeft, sizeRight, sizeDown;

	void UpdateGuiAlignment(const Qt::AlignmentFlag alignment, const bool optimize = true);
	QIcon NewIconColor(const QColor color) const;

	void SaveFile();

private slots:
	void on_Wizard_finished(int result);

	void on_TopPosition_clicked();
	void on_LeftPosition_clicked();
	void on_RightPosition_clicked();
	void on_DownPosition_clicked();

	void on_ColorTitle_clicked();
	void on_ColorText_clicked();
	void on_ColorLine_clicked();
	void on_ColorDots_clicked();
	void on_ColorBackground_clicked();

	void on_SliderTransparency_valueChanged(int value);

	void on_ButtonUp_clicked();
	void on_ButtonDown_clicked();
	void on_ButtonAddJump_clicked();
	void on_ButtonAddSpace_clicked();
	void on_ButtonAddTitle_clicked();
	void on_ButtonAddNormal_clicked();
	void on_ButtonEditWidget_clicked();
	void on_ButtonDeleteWidget_clicked();
};

#endif // WIZARD_H
