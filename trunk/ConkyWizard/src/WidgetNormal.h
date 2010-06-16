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
#ifndef WIDGETNORMAL_H
#define WIDGETNORMAL_H

#include "WidgetConky.h"

class WidgetNormal : public WidgetConky
{
	Q_ENUMS(ETypeScript)
	Q_PROPERTY(QString question READ Question WRITE setQuestion)
	Q_PROPERTY(QString response READ Response WRITE setResponse)
	Q_PROPERTY(bool isScript READ IsScript WRITE setIsScript)
	Q_PROPERTY(QString nameScript READ NameScript WRITE setNameScript)
	Q_PROPERTY(ETypeScript typeScript READ TypeScript WRITE setTypeScript)
	Q_PROPERTY(QString script READ Script WRITE setScript)

public:
	WidgetNormal(QString _name, QString _question, QString _response, int _ratio = 35, QObject *parent = 0);
	WidgetNormal(QObject *parent = 0);
	~WidgetNormal();

	enum ETypeScript{ScriptBash = 0, ScriptPython = 1};

	void setRatio(int _ratio) { ratio = _ratio; }
	int Ratio() const { return ratio; }
	void setQuestion(const QString _question) { question = _question; }
	QString Question() const { return question; }
	void setResponse(const QString _response) { response = _response; }
	QString Response() const { return response; }
	void setIsScript(bool _isScript) { isScript = _isScript; }
	bool IsScript() const { return isScript; }
	void setNameScript(const QString _nameScript) { nameScript = _nameScript; }
	QString NameScript() const { return nameScript; }
	void setTypeScript(ETypeScript _typeScript) { typeScript = _typeScript; }
	ETypeScript TypeScript() const { return typeScript; }
	void setScript(const QString _script) { script = _script; }
	QString Script() const { return script; }

	void WriteTheLine(DockConky* dockConky, int position, int space, QTextStream &out);
	void WriteTheScript(QTextStream &out);

private:
	QString question;
	QString response;
	int ratio;

	bool isScript;
	QString nameScript;
	ETypeScript typeScript;
	QString script;
};

QDataStream &operator<<(QDataStream &out, const WidgetNormal &widgetNormal);
QDataStream &operator>>(QDataStream &in, WidgetNormal &widgetNormal);

#endif // WIDGETNORMAL_H
