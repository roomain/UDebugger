#include "DebugWorkspace.h"
#include <algorithm>
#include <qfile.h>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>

const QString DebugWorkspace::m_tagColor = "CONF_COLORS";

DebugWorkspace& DebugWorkspace::instance()
{
	static DebugWorkspace s_instance;
	return s_instance;
}


bool DebugWorkspace::loadConfiguration(const QString& a_filename)
{
	bool bRet = false;
	QFile file(a_filename);
	if (file.open(QIODevice::ReadOnly))
	{
		QDomDocument xmlDoc("Debug");
		bRet = static_cast<bool>(xmlDoc.setContent(&file));
		if (bRet)
		{
			m_colorations.clear();
			auto root = xmlDoc.firstChildElement("UniversalDebug");
			QDomElement nodeColors = root.firstChildElement("ClassColor");
			if (!nodeColors.isNull())
			{
				auto colorList = nodeColors.childNodes();
				for (int iIndex = 0; iIndex < colorList.size(); ++iIndex)
				{
					auto node = colorList.at(iIndex).toElement();
					if (node.nodeName().compare("Color") == 0)
					{
						QString classname = node.attribute("class");
						QString colorValue = node.attribute("color");
						m_colorations.emplace(node.attribute("class"), toColor(node.attribute("color")));
					}
				}
			}
		}
	}
	return bRet;
}

void DebugWorkspace::saveConfiguration(const QString& a_filename)
{
	QFile file(a_filename);
	if (file.open(QIODevice::WriteOnly))
	{
		QXmlStreamWriter stream(&file);
		stream.setAutoFormatting(true);
		stream.setAutoFormattingIndent(true);
		stream.writeStartDocument();

		stream.writeStartElement("UniversalDebug");
		
		stream.writeStartElement("ClassColor");
		for (const auto iter : m_colorations)
		{
			stream.writeStartElement("Color");
			stream.writeAttribute("class", iter.first);
			stream.writeAttribute("color", DebugWorkspace::toString(iter.second));
			stream.writeEndElement();
		}
		stream.writeEndElement();
		stream.writeEndElement();

		stream.writeEndDocument();
	}
}

void DebugWorkspace::setColor(const QString& a_name, const QColor& a_color)
{
	auto iter = std::ranges::find_if(m_colorations, [&a_name](const auto& a_coloration)
		{
			return a_coloration.first.compare(a_name) == 0;
		});

	if (iter != m_colorations.end())
	{
		iter->second = a_color;
	}
	else
	{
		m_colorations.emplace(a_name, a_color);
	}
}

void DebugWorkspace::removeColor(const QString& a_name)
{
	auto iter = std::ranges::find_if(m_colorations, [&a_name](const auto& a_coloration)
		{
			return a_coloration.first.compare(a_name) == 0;
		});

	if (iter != m_colorations.end())
		m_colorations.erase(iter);
}

QColor DebugWorkspace::getColor(const QString& a_name)const
{
	auto iter = std::ranges::find_if(m_colorations, [&a_name](const auto& a_coloration)
		{
			return a_coloration.first.compare(a_name) == 0;
		});

	if (iter != m_colorations.end())
		return iter->second;

	return QColor();
}

QString DebugWorkspace::toString(const QColor& a_color)
{
	return QString("%1;%2;%3").arg(a_color.red()).arg(a_color.green()).arg(a_color.blue());
}

QColor DebugWorkspace::toColor(const QString& a_color)
{
	auto lComponent = a_color.split(";");
	if (lComponent.size() == 3)
		return QColor(lComponent[0].toInt(), lComponent[1].toInt(), lComponent[2].toInt());
	return QColor();
}