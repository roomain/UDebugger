#pragma once
#include <QSettings>
#include <qcolor.h>
#include <list>


/*@brief singleton containing shared configuration*/
class DebugWorkspace
{

private:
	static const QString m_tagColor;
	std::map<QString, QColor> m_colorations;

	static [[nodiscard]] QColor toColor(const QString& a_color);
	static [[nodiscard]] QString toString(const QColor& a_color);

public:
	static DebugWorkspace& instance();
	bool loadConfiguration(const QString& a_filename);
	void saveConfiguration(const QString& a_filename);
	void setColor(const QString& a_name, const QColor& a_color);
	void removeColor(const QString& a_name);
	inline [[nodiscard]] const std::map<QString, QColor>& getColors()const { return m_colorations; }
	QColor getColor(const QString& a_name)const;
};

