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
#include "DialogNewEdit.h"
#include "ui_DialogNewEdit.h"

// _____________________________________________________________________________
// CONSTRUCTORES Y DESTRUCTOR.

DialogNewEdit::DialogNewEdit(QWidget *parent) : QDialog(parent), ui(new Ui::DialogNewEdit)
{
	ui->setupUi(this);
	init_Script();

	widgetNormal = new WidgetNormal;
	this->setWindowTitle(tr("New widget"));
}

DialogNewEdit::DialogNewEdit(WidgetNormal *_widgetNormal, QWidget *parent) :
		QDialog(parent), ui(new Ui::DialogNewEdit), widgetNormal(_widgetNormal)
{
	ui->setupUi(this);
	init_Script();

	ui->Name->setText(widgetNormal->Name());
	ui->Ratio->setValue(widgetNormal->Ratio());
	on_Ratio_valueChanged(widgetNormal->Ratio());
	ui->Question->setText(widgetNormal->Question());
	ui->Response->setText(widgetNormal->Response());

	ui->IsScript->setChecked(widgetNormal->IsScript());
	ui->NameScript->setText(widgetNormal->NameScript());
	switch(widgetNormal->TypeScript()){
	case WidgetNormal::ScriptBash:
		ui->TypeScript->setCurrentIndex(0);
		break;
	default: // case WidgetNormal::ScriptPython:
		ui->TypeScript->setCurrentIndex(1);
		break;
	}
	ui->Script->setText(widgetNormal->Script());

	this->setWindowTitle(tr("Edit widget"));
}

DialogNewEdit::~DialogNewEdit()
{
	delete ui;
}

// _____________________________________________________________________________
// MÉTODOS DE INICIALIZACIÓN.

void DialogNewEdit::init_Script()
{
	ui->TypeScript->addItem(".sh");
	ui->TypeScript->addItem(".py");
	ui->TypeScript->setCurrentIndex(0);
}

// _____________________________________________________________________________
// PROPIEDADES.

WidgetNormal* DialogNewEdit::Response()
{
	UpdateWidget();
	return widgetNormal;
}

// _____________________________________________________________________________
// MÉTODOS.

void DialogNewEdit::UpdateWidget()
{
	widgetNormal->setName(ui->Name->text());
	widgetNormal->setRatio(ui->Ratio->value());
	widgetNormal->setQuestion(ui->Question->text());
	widgetNormal->setResponse(ui->Response->text());
	widgetNormal->setIsScript(ui->IsScript->isChecked());
	widgetNormal->setNameScript(ui->NameScript->text());
	widgetNormal->setTypeScript((WidgetNormal::ETypeScript) ui->TypeScript->currentIndex());
	widgetNormal->setScript(ui->Script->toPlainText());
}

// _____________________________________________________________________________
// EVENTOS.

void DialogNewEdit::changeEvent(QEvent *event)
{
	QDialog::changeEvent(event);
	switch (event->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

// _____________________________________________________________________________
// SLOTS.

void DialogNewEdit::on_Ratio_valueChanged(int value)
{
	ui->LabelQuestion->setText(tr("Question %1 %").arg(value));
	ui->LabelResponse->setText(tr("Response %1 %").arg(100 - value));

	switch(value){
	case 0:
		ui->Question->setEnabled(false);
		ui->Response->setEnabled(true);
		break;
	case 100:
		ui->Question->setEnabled(true);
		ui->Response->setEnabled(false);
		break;
	default:
		ui->Question->setEnabled(true);
		ui->Response->setEnabled(true);
		break;
	}
}
