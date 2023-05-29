#pragma once
#include <qcolor.h>
#include <qicon.h>
#include <QDialog>
#include "ui_ColorationEditor.h"

class ColorationEditor : public QDialog
{
	Q_OBJECT

public:
	ColorationEditor(QWidget *parent = nullptr);
	~ColorationEditor();

private:
	Ui::ColorationEditorClass ui;
	QColor m_curColor;

	static QIcon genIconColor(const QColor& a_color);
	void onItemSelected(QListWidgetItem* a_current, QListWidgetItem* a_previous);
	void onTextChanged(const QString& a_text);

	void onColor();
	void onValid();
	void onRemove();
};
