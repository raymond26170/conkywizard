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
#ifndef DIALOGNEWEDIT_H
#define DIALOGNEWEDIT_H

#include <QDialog>
#include "WidgetNormal.h"

namespace Ui
{
	class DialogNewEdit;
}

class DialogNewEdit : public QDialog
{
	Q_OBJECT
	Q_PROPERTY(WidgetNormal* widgetNormal READ Response)

public:
	explicit DialogNewEdit(QWidget *parent = 0);
	explicit DialogNewEdit(WidgetNormal *_widgetNormal, QWidget *parent = 0);
	~DialogNewEdit();

	void UpdateWidget();
	WidgetNormal *Response();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::DialogNewEdit *ui;

	void init_Script();

	WidgetNormal* widgetNormal;

private slots:
	void on_Ratio_valueChanged(int value);
};

#endif // DIALOGNEWEDIT_H
