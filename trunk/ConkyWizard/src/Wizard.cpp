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
#include "Wizard.h"
#include "ui_Wizard.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QProcess>
#include <QColorDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QTreeWidgetItem>
#include <QDir>
#include <QFile>
#include <QDataStream>

#include "WidgetJump.h"
#include "WidgetSpace.h"
#include "WidgetTitle.h"
#include "WidgetNormal.h"

#include "DockConky.h"
#include "ImageBackground.h"

#include "WidgetModel.h"
#include "DialogLoadFile.h"
#include "DialogNewEdit.h"

// _____________________________________________________________________________
// CONSTRUCTORES Y DESTRUCTOR.

Wizard::Wizard(QWidget *parent) : QWizard(parent), ui(new Ui::Wizard)
{
	ui->setupUi(this);
	this->setWindowTitle(tr("%1 - Version %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
	connect(this, SIGNAL(finished(int)), this, SLOT(on_Wizard_finished(int)));

	init_Graphics();
	bool result = init_LoadDefault();
	init_Gui();
	UpdateGuiAlignment(dockConky->Alignment(), result);
}

Wizard::Wizard(const QString &url, QWidget *parent) : QWizard(parent), ui(new Ui::Wizard)
{
	ui->setupUi(this);
	this->setWindowTitle(tr("%1 - Version %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
	connect(this, SIGNAL(finished(int)), this, SLOT(on_Wizard_finished(int)));

	init_Graphics();
	init_LoadUrl(url);
	init_Gui();
	UpdateGuiAlignment(dockConky->Alignment());
}

Wizard::~Wizard()
{
	delete ui;
	delete dockConky;
	delete imageBackground;
	delete widgetModel;
}

// _____________________________________________________________________________
// MÉTODOS DE INICIALIZACIÓN.

void Wizard::init_Graphics()
{
	// Iniciar los gráficos del diálogo.
	setPixmap(QWizard::LogoPixmap, QPixmap(":/graphics/Logo.png"));
	setPixmap(QWizard::BannerPixmap, QPixmap(":/graphics/Banner.png"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/graphics/Watermark.png"));

	// Iniciar los gráficos de las pantallas.
	screenTop.load(":/graphics/FlatScreenUp.png");
	screenLeft.load(":/graphics/FlatScreenLeft.png");
	screenRight.load(":/graphics/FlatScreenRight.png");
	screenDown.load(":/graphics/FlatScreenDown.png");

	// Iniciar los gráficos de los esquemas.
	sizeTop.load(":/graphics/SizePositionUp.png");
	sizeLeft.load(":/graphics/SizePositionLeft.png");
	sizeRight.load(":/graphics/SizePositionRight.png");
	sizeDown.load(":/graphics/SizePositionDown.png");
}

bool Wizard::init_LoadDefault()
{
	// FASE 1. EXISTENCIA DEL ARCHIVO.
	QFile file(QDir::homePath() + "/.ConkyWizardTheme/default.ConkyWizard");
	if(file.exists()){
		// FASE 2. ABRIR ARCHIVO.
		if(file.open(QIODevice::ReadOnly)){
			QString applicationName;
			QString applicationVersion;
			QDataStream in(&file);
			in.setVersion(QDataStream::Qt_4_6);
			in >> applicationName;
			in >> applicationVersion;

			// FASE 3. VALIDAR FORMATO.
			if(applicationName == qApp->applicationName() &&
			   applicationVersion == qApp->applicationVersion()){

				// FASE 4. PREGUNTAR y TOMAR DECISIÓN FINAL.
				int response = QMessageBox::question(this,
													 tr("Configuration"),
													 tr("You want to use the previous saved configuration?"),
													 QMessageBox::Yes | QMessageBox::No);
				if (response == QMessageBox::Yes){
					dockConky = new DockConky;
					in >> *dockConky;

					imageBackground = new ImageBackground;
					in >> *imageBackground;

					QDialog *dialog = new DialogLoadFile;
					dialog->exec();

					dockConky->DisableWidgetsScripts();

					delete dialog;

					return false;
				} else {
					// NO se usa la configuración guardada.
					dockConky = new DockConky;
					dockConky->BasicWidgets();
					imageBackground = new ImageBackground;
					return true;
				}
			} else {
				// NO es válido el formato.
				QMessageBox::information(this,
										 tr("Error loading the file"),
										 tr("Not valid format"),
										 QMessageBox::Ok);

				dockConky = new DockConky;
				dockConky->BasicWidgets();
				imageBackground = new ImageBackground;
				return true;
			}

			file.close();
		} else {
			// NO se puede abrir el archivo.
			QMessageBox::information(this,
									 tr("Error loading the file"),
									 tr("Unable to open the file"),
									 QMessageBox::Ok);

			dockConky = new DockConky;
			dockConky->BasicWidgets();
			imageBackground = new ImageBackground;
			return true;
		}
	} else {
		// NO existe el archivo.
		dockConky = new DockConky;
		dockConky->BasicWidgets();
		imageBackground = new ImageBackground;
		return true;
	}

	return true;
}

void Wizard::init_LoadUrl(const QString url)
{
	// FASE 1. EXISTENCIA DEL ARCHIVO.
	QFile file(QDir::currentPath() + "/." + url);
	if(file.exists()){
		// FASE 2. ABRIR ARCHIVO.
		if(file.open(QIODevice::ReadOnly)){
			QString applicationName;
			QString applicationVersion;
			QDataStream in(&file);
			in.setVersion(QDataStream::Qt_4_6);
			in >> applicationName;
			in >> applicationVersion;

			// FASE 3. VALIDAR FORMATO.
			if(applicationName == qApp->applicationName() &&
			   applicationVersion == qApp->applicationVersion()){

				// FASE 4. NOTIFICAR y TOMAR DECISIÓN FINAL.
				QDialog *dialog = new DialogLoadFile;
				dialog->exec();

				dockConky = new DockConky;
				in >> *dockConky;
				dockConky->DisableWidgetsScripts();

				imageBackground = new ImageBackground;
				in >> *imageBackground;

				delete dialog;
			} else {
				// NO es válido el formato.
				QMessageBox::information(this,
										 tr("Error loading the file"),
										 tr("Not valid format"),
										 QMessageBox::Ok);

				emit finished(QDialog::Rejected);
			}

			file.close();
		} else {
			// NO se puede abrir el archivo.
			QMessageBox::information(this,
									 tr("Error loading the file"),
									 tr("Unable to open the file"),
									 QMessageBox::Ok);

			emit finished(QDialog::Rejected);
		}
	} else {
		// NO existe el archivo.
		QMessageBox::information(this,
								 tr("Error loading the file"),
								 tr("No such file or incorrect path"),
								 QMessageBox::Ok);

		emit finished(QDialog::Rejected);
	}
}

void Wizard::init_Gui()
{
	ui->TitlesFont->setCurrentFont(dockConky->FontTitle().family());
	ui->TitlesSize->setValue(dockConky->FontTitle().pointSize());
	if(dockConky->FontTitle().bold()) ui->TitlesBold->setCheckState(Qt::Checked);
	else ui->TitlesBold->setCheckState(Qt::Unchecked);
	if(dockConky->FontTitle().italic()) ui->TitlesItalic->setCheckState(Qt::Checked);
	else ui->TitlesItalic->setCheckState(Qt::Unchecked);

	ui->TextFont->setCurrentFont(dockConky->FontText().family());
	ui->TextSize->setValue(dockConky->FontText().pointSize());
	if(dockConky->FontText().bold()) ui->TextBold->setCheckState(Qt::Checked);
	else ui->TextBold->setCheckState(Qt::Unchecked);
	if(dockConky->FontText().italic()) ui->TextItalic->setCheckState(Qt::Checked);
	else ui->TextItalic->setCheckState(Qt::Unchecked);

	ui->DisplacementX->setValue(dockConky->DisplacementX());
	ui->DisplacementY->setValue(dockConky->DisplacementY());
	ui->WidthValue->setValue(dockConky->Width());
	ui->HeightValue->setValue(dockConky->Height());
	ui->Space->setValue(dockConky->Space());
	ui->Separation->setValue(dockConky->Separation());

	ui->Delay->setValue(dockConky->Delay());

	if(imageBackground->Transparency()){
		ui->Transparency->setCheckState(Qt::Checked);
		ui->SliderTransparency->setEnabled(true);
		ui->LabelTransparency->setEnabled(true);
	} else {
		ui->Transparency->setCheckState(Qt::Unchecked);
		ui->SliderTransparency->setEnabled(false);
		ui->LabelTransparency->setEnabled(false);
	}
	ui->SliderTransparency->setValue(imageBackground->Alpha());

	// Los iconos de color.
	ui->ColorTitle->setIcon(NewIconColor(dockConky->ColorTitle()));
	ui->ColorText->setIcon(NewIconColor(dockConky->ColorText()));
	ui->ColorLine->setIcon(NewIconColor(imageBackground->ColorLine()));
	ui->ColorDots->setIcon(NewIconColor(imageBackground->ColorDots()));
	ui->ColorBackground->setIcon(NewIconColor(imageBackground->ColorBackground()));

	// Iniciar "WidgetModel"
	widgetModel = new WidgetModel;
	widgetModel->setWidgets(dockConky->Widgets());
	ui->WidgetsManagement->setModel(widgetModel);
}

// _____________________________________________________________________________
// EVENTOS.

void Wizard::changeEvent(QEvent *event)
{
	QWizard::changeEvent(event);
	switch (event->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

// _____________________________________________________________________________
// SLOTS PARA EL MANEJO DE LAS POSICIONES.

void Wizard::on_TopPosition_clicked()
{
	UpdateGuiAlignment(Qt::AlignTop);
}

void Wizard::on_LeftPosition_clicked()
{
	UpdateGuiAlignment(Qt::AlignLeft);
}

void Wizard::on_RightPosition_clicked()
{
	UpdateGuiAlignment(Qt::AlignRight);
}

void Wizard::on_DownPosition_clicked()
{
	UpdateGuiAlignment(Qt::AlignBottom);
}

// _____________________________________________________________________________
// SLOTS PARA EL MANEJO DE LOS COLORES.

void Wizard::on_ColorTitle_clicked()
{
	QColor color = QColorDialog::getColor(dockConky->ColorTitle(), this);
	if(color.isValid()){
		dockConky->setColorTitle(color);
		ui->ColorTitle->setIcon(NewIconColor(color));
	}
}

void Wizard::on_ColorText_clicked()
{
	QColor color = QColorDialog::getColor(dockConky->ColorText(), this);
	if(color.isValid()){
		dockConky->setColorText(color);
		ui->ColorText->setIcon(NewIconColor(color));
	}
}

void Wizard::on_ColorLine_clicked()
{
	QColor color = QColorDialog::getColor(imageBackground->ColorLine(), this);
	if(color.isValid()){
		imageBackground->setColorLine(color);
		ui->ColorLine->setIcon(NewIconColor(color));
	}
}

void Wizard::on_ColorDots_clicked()
{
	QColor color = QColorDialog::getColor(imageBackground->ColorDots(), this);
	if(color.isValid()){
		imageBackground->setColorDots(color);
		ui->ColorDots->setIcon(NewIconColor(color));
	}
}

void Wizard::on_ColorBackground_clicked()
{
	QColor color = QColorDialog::getColor(imageBackground->ColorBackground(), this);
	if(color.isValid()){
		imageBackground->setColorBackground(color);
		ui->ColorBackground->setIcon(NewIconColor(color));
	}
}

void Wizard::on_SliderTransparency_valueChanged(int value)
{

	ui->LabelTransparency->setText(QString::number((quint32) (value*100/255)) + " %");
}

// _____________________________________________________________________________
// SLOTS PARA EL MANEJO DE LOS "WIDGETS CONKY".

void Wizard::on_ButtonUp_clicked()
{
	QModelIndex index = ui->WidgetsManagement->selectionModel()->currentIndex();
	widgetModel->moveUpWidget(index.row(), index);
}

void Wizard::on_ButtonDown_clicked()
{
	QModelIndex index = ui->WidgetsManagement->selectionModel()->currentIndex();
	widgetModel->moveDownWidget(index.row(), index);
}

void Wizard::on_ButtonAddJump_clicked()
{
	int response = QMessageBox::question(this,
										 tr("Add column break Widget"),
										 tr("You want to insert a column break widget?"),
										 QMessageBox::Ok | QMessageBox::Cancel);

	if (response == QMessageBox::Ok){
		QModelIndex index = ui->WidgetsManagement->selectionModel()->currentIndex();
		widgetModel->insertWidget(index.row(), index, new WidgetJump);
	}
}

void Wizard::on_ButtonAddSpace_clicked()
{
	int response = QMessageBox::question(this,
										 tr("Add newline Widget"),
										 tr("You want to insert a newline widget?"),
										 QMessageBox::Ok | QMessageBox::Cancel);

	if (response == QMessageBox::Ok){
		QModelIndex index = ui->WidgetsManagement->selectionModel()->currentIndex();
		widgetModel->insertWidget(index.row(), index, new WidgetSpace);
	}
}

void Wizard::on_ButtonAddTitle_clicked()
{
	bool response = 0;
	QString text = QInputDialog::getText(this,
										 tr("Add title Widget"),
										 tr("Insert title (name) text:"),
										 QLineEdit::Normal,
										 NULL,
										 &response);

	if (response && !text.isEmpty()){
		WidgetTitle *widgetTitle = new WidgetTitle(text);

		QModelIndex index = ui->WidgetsManagement->selectionModel()->currentIndex();
		widgetModel->insertWidget(index.row(), index, widgetTitle);
	}
}

void Wizard::on_ButtonAddNormal_clicked()
{
	DialogNewEdit* dialog = new DialogNewEdit;
	int response = dialog->exec();

	if (response == 1){
		WidgetNormal *widgetNormal = dialog->Response();

		QModelIndex index = ui->WidgetsManagement->selectionModel()->currentIndex();
		widgetModel->insertWidget(index.row(), index, widgetNormal);
	}

	delete dialog;
}

void Wizard::on_ButtonEditWidget_clicked()
{
	QModelIndex index = ui->WidgetsManagement->selectionModel()->currentIndex();
	WidgetConky *widgetConky = dockConky->Widgets()->at(index.row());

	if(widgetConky->TypeWidget() == WidgetConky::NormalType){
		DialogNewEdit* dialog = new DialogNewEdit(dynamic_cast<WidgetNormal*>(widgetConky));
		dialog->exec();
		dialog->UpdateWidget();

		delete dialog;
	}
}

void Wizard::on_ButtonDeleteWidget_clicked()
{
	int response = QMessageBox::question(this,
										 tr("Delete Widget"),
										 tr("You want to delete the selected widget?"),
										 QMessageBox::Ok | QMessageBox::Cancel);

	if (response == QMessageBox::Ok){
		QModelIndex index = ui->WidgetsManagement->selectionModel()->currentIndex();
		widgetModel->removeWidget(index.row(), index);
	}
}

// _____________________________________________________________________________
// SLOTS PARA EL MANEJO DEL PROPIO ASISTENTE.

void Wizard::on_Wizard_finished(int result)
{
	QFont fontTitle;
	QFont fontText;

	switch(result){
	case QDialog::Accepted:
		printf("ConkyWizard --> The wizard ends with the result \"Accepted\"\n");

		fontTitle = ui->TitlesFont->currentFont();
		fontTitle.setPointSize(ui->TitlesSize->value());
		if(ui->TitlesBold->checkState() == Qt::Checked) fontTitle.setBold(true);
		else fontTitle.setBold(false);
		if(ui->TitlesItalic->checkState() == Qt::Checked) fontTitle.setItalic(true);
		else fontTitle.setItalic(false);
		dockConky->setFontTitle(fontTitle);

		fontText = ui->TextFont->currentFont();
		fontText.setPointSize(ui->TextSize->value());
		if(ui->TextBold->checkState() == Qt::Checked) fontText.setBold(true);
		else  fontText.setBold(false);
		if(ui->TextItalic->checkState() == Qt::Checked) fontText.setItalic(true);
		else fontText.setItalic(false);
		dockConky->setFontText(fontText);

		dockConky->setOffset(imageBackground->Offset());
		dockConky->setDisplacementX(ui->DisplacementX->value());
		dockConky->setDisplacementY(ui->DisplacementY->value());
		dockConky->setWidth(ui->WidthValue->value());
		dockConky->setHeight(ui->HeightValue->value());
		dockConky->setSpace(ui->Space->value());
		dockConky->setSeparation(ui->Separation->value());

		dockConky->setDelay(ui->Delay->value());

		if(ui->Transparency->checkState() == Qt::Checked) imageBackground->setTransparency(true);
		else  imageBackground->setTransparency(false);
		imageBackground->setAlpha(ui->SliderTransparency->value());

		// Generamos los dos elementos.
		dockConky->NewLaunchFile();
		dockConky->NewDockFile();
		dockConky->NewDockScripts();
		imageBackground->NewImage(dockConky->Alignment(), dockConky->Width(), dockConky->Height());

		// Guardamos la configuración.
		SaveFile();

		// Lanzamos Conky.
		if(ui->RunConky->isChecked()){
			QProcess::startDetached("conky", QStringList() << "-c" << QDir::homePath() + "/.ConkyWizardTheme/ConkyWizardTheme");
		}
		break;
	default: //case QDialog::Rejected:
		printf("ConkyWizard --> The wizard ends with the result \"Rejected\"\n");
		break;
	}
}

// _____________________________________________________________________________
// MÉTODOS AUXILIARES.

void Wizard::UpdateGuiAlignment(const Qt::AlignmentFlag alignment, const bool optimize)
{
	int x, y, width, height;

	switch (alignment) {
	case Qt::AlignTop:
		ui->ImageScreen->setPixmap(screenTop);
		ui->ImageSize->setPixmap(sizeTop);
		ui->LabelWidth->setText(tr("W (screen width minus the width of the left and right panels)"));
		ui->LabelHeight->setText(tr("H (Conky panel height)"));
		if(optimize == true){
			x = QApplication::desktop()->availableGeometry().left();
			y = QApplication::desktop()->availableGeometry().top();
			width = QApplication::desktop()->availableGeometry().width();
			height = 192;
		} else {
			ui->TopPosition->setChecked(true);
			ui->LeftPosition->setChecked(false);
			ui->RightPosition->setChecked(false);
			ui->DownPosition->setChecked(false);
		}
		ui->SpaceImage->setVisible(true);
		ui->SpaceGroup->setVisible(true);

		dockConky->setAlignment(Qt::AlignTop);
		break;
	case Qt::AlignLeft:
		ui->ImageScreen->setPixmap(screenLeft);
		ui->ImageSize->setPixmap(sizeLeft);
		ui->LabelWidth->setText(tr("W (Conky panel width)"));
		ui->LabelHeight->setText(tr("H (screen height minus the height of upper and lower panels)"));
		if(optimize == true){
			x = QApplication::desktop()->availableGeometry().left();
			y = QApplication::desktop()->availableGeometry().top();
			width = 256;
			height = QApplication::desktop()->availableGeometry().height();
		} else  {
			ui->TopPosition->setChecked(false);
			ui->LeftPosition->setChecked(true);
			ui->RightPosition->setChecked(false);
			ui->DownPosition->setChecked(false);
		}
		ui->SpaceImage->setVisible(false);
		ui->SpaceGroup->setVisible(false);

		dockConky->setAlignment(Qt::AlignLeft);
		break;
	case Qt::AlignRight:
		ui->ImageScreen->setPixmap(screenRight);
		ui->ImageSize->setPixmap(sizeRight);
		ui->LabelWidth->setText(tr("W (Conky panel width)"));
		ui->LabelHeight->setText(tr("H (screen height minus the height of upper and lower panels)"));
		if(optimize == true){
			x = QApplication::desktop()->screenGeometry().width() - QApplication::desktop()->availableGeometry().right();
			y = QApplication::desktop()->availableGeometry().top();
			width = 256;
			height = QApplication::desktop()->availableGeometry().height();
		} else {
			ui->TopPosition->setChecked(false);
			ui->LeftPosition->setChecked(false);
			ui->RightPosition->setChecked(true);
			ui->DownPosition->setChecked(false);
		}
		ui->SpaceImage->setVisible(false);
		ui->SpaceGroup->setVisible(false);

		dockConky->setAlignment(Qt::AlignRight);
		break;
	default: // Qt::AlignBottom
		ui->ImageScreen->setPixmap(screenDown);
		ui->ImageSize->setPixmap(sizeDown);
		ui->LabelWidth->setText(tr("W (screen width minus the width of the left and right panels)"));
		ui->LabelHeight->setText(tr("H (Conky panel height)"));
		if(optimize == true){
			x = QApplication::desktop()->availableGeometry().left();
			y = QApplication::desktop()->screenGeometry().height() - QApplication::desktop()->availableGeometry().bottom();
			width = QApplication::desktop()->availableGeometry().width();
			height = 192;
		} else  {
			ui->TopPosition->setChecked(false);
			ui->LeftPosition->setChecked(false);
			ui->RightPosition->setChecked(false);
			ui->DownPosition->setChecked(true);
		}
		ui->SpaceImage->setVisible(true);
		ui->SpaceGroup->setVisible(true);

		dockConky->setAlignment(Qt::AlignBottom);
		break;
	}

	if(optimize == true){
		ui->DisplacementX->setValue(x);
		ui->DisplacementY->setValue(y);
		ui->WidthValue->setValue(width);
		ui->HeightValue->setValue(height);
	}
}

QIcon Wizard::NewIconColor(const QColor color) const
{
	QPixmap pixmap(32, 12);
	pixmap.fill(color);

	QPainter painter(&pixmap);
	painter.setPen(Qt::black);
	painter.drawRect(pixmap.rect().adjusted(0, 0, -1, -1));

	QIcon icon(pixmap);

	return icon;
}

// _____________________________________________________________________________
// MÉTODOS PARA EL MANEJO DE ARCHIVOS.

void Wizard::SaveFile()
{
	QDir dir;
	dir.mkpath(QDir::homePath() + "/.ConkyWizardTheme");

	QFile file(QDir::homePath() + "/.ConkyWizardTheme/default.ConkyWizard");
	if (!file.open(QIODevice::WriteOnly)) return;

	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_4_6);
	out << qApp->applicationName();
	out << qApp->applicationVersion();

	out << *dockConky;
	out << *imageBackground;

	file.close();
}
