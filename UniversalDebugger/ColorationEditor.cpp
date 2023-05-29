#include "ColorationEditor.h"
#include <qpixmap.h>
#include <qcolordialog.h>
#include "DebugWorkspace.h"

ColorationEditor::ColorationEditor(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	for (const auto& iter : DebugWorkspace::instance().getColors())
	{
		auto pItem = new QListWidgetItem(iter.first);
		pItem->setData(Qt::UserRole, iter.second);
		pItem->setIcon(genIconColor(iter.second));
		ui.lwdgColor->addItem(pItem);
	}

	ui.pBtnColor->setIcon(genIconColor(m_curColor));
	QObject::connect(ui.lEdtName, &QLineEdit::textChanged, this, &ColorationEditor::onTextChanged);
	QObject::connect(ui.lwdgColor, &QListWidget::currentItemChanged, this, &ColorationEditor::onItemSelected);
	QObject::connect(ui.pBtnColor, QOverload<bool>::of(&QPushButton::clicked), this, &ColorationEditor::onColor);
	QObject::connect(ui.pBtnValid, QOverload<bool>::of(&QPushButton::clicked), this, &ColorationEditor::onValid);
	QObject::connect(ui.pBtnRemove, QOverload<bool>::of(&QPushButton::clicked), this, &ColorationEditor::onRemove);
}

ColorationEditor::~ColorationEditor()
{}


QIcon ColorationEditor::genIconColor(const QColor& a_color)
{
	QPixmap pix(32, 32);
	pix.fill(a_color);
	return QIcon(pix);
}

void ColorationEditor::onItemSelected(QListWidgetItem* a_current, QListWidgetItem* a_previous)
{
	if (a_current)
	{
		ui.lEdtName->setText(a_current->text());		
	}
	else
	{
		ui.lEdtName->setText("");
	}
}

void ColorationEditor::onTextChanged(const QString& a_text)
{
	ui.pBtnValid->setEnabled(!a_text.isEmpty());
	ui.pBtnRemove->setEnabled(false);
	QListWidgetItem* pSelectedItem = nullptr;
	const int iCount = ui.lwdgColor->count();
	for (int i = 0; i < iCount && (pSelectedItem == nullptr); ++i)
	{
		auto pItem = ui.lwdgColor->item(i);
		if (pItem->text().compare(a_text, Qt::CaseSensitive) == 0)
		{
			if (!pItem->isSelected())
				ui.lwdgColor->setCurrentItem(pItem);
			pSelectedItem = pItem;
			ui.pBtnRemove->setEnabled(true);
		}
	}

	if (pSelectedItem)
	{
		m_curColor = pSelectedItem->data(Qt::UserRole).value<QColor>();
		ui.pBtnColor->setIcon(genIconColor(m_curColor));
	}
	else
	{
		m_curColor = QColor(0, 0, 0);
		ui.pBtnColor->setIcon(genIconColor(m_curColor));
	}
}

void ColorationEditor::onColor()
{
	QColorDialog diag(m_curColor);
	if (diag.exec() == QDialog::Accepted)
	{
		m_curColor = diag.selectedColor();
		ui.pBtnColor->setIcon(genIconColor(m_curColor));
	}
}

void ColorationEditor::onValid()
{
	if (ui.pBtnRemove->isEnabled())
	{
		auto pItem = ui.lwdgColor->currentItem();
		pItem->setIcon(genIconColor(m_curColor));
		// update workspace
		DebugWorkspace::instance().setColor(pItem->text(), m_curColor);
	}
	else
	{
		// new
		auto pItem = new QListWidgetItem(ui.lEdtName->text());
		pItem->setData(Qt::UserRole, m_curColor);
		pItem->setIcon(genIconColor(m_curColor));
		ui.lwdgColor->addItem(pItem);
		DebugWorkspace::instance().setColor(ui.lEdtName->text(), m_curColor);
		ui.lwdgColor->setCurrentItem(pItem);
		ui.pBtnRemove->setEnabled(true);
	}
}

void ColorationEditor::onRemove()
{
	auto pItem = ui.lwdgColor->currentItem();
	if (pItem)
	{
		// remove from workspace
		ui.lwdgColor->removeItemWidget(pItem);
		delete pItem;
	}
}