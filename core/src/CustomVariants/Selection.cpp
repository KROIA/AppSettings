#include "CustomVariants/Selection.h"
#include <QJsonObject>
#include <QJsonArray>

namespace AppSettings
{
	Selection::Selection()
		: m_selection()
		, m_selectedIndex(0)
	{

	}
	Selection::Selection(const Selection& other)
		: m_selection(other.m_selection)
		, m_selectedIndex(other.m_selectedIndex)
	{

	}
	Selection::Selection(const QStringList& selection)
		: m_selection(selection)
		, m_selectedIndex(0)
	{

	}
	Selection::Selection(const QStringList& selection, size_t currentIndex)
		: m_selection(selection)
		, m_selectedIndex(currentIndex)
	{
		if (currentIndex >= m_selection.size())
			currentIndex = m_selection.size() - 1;
	}

	bool Selection::operator==(const Selection& other) const
	{
		if (m_selection.size() != other.m_selection.size())
			return false;
		if (m_selectedIndex != other.m_selectedIndex)
			return false;
		return m_selection == other.m_selection;
	}
	bool Selection::operator!=(const Selection& other) const
	{
		return !operator==(other);
	}

	bool Selection::select(size_t index)
	{
		if (index >= m_selection.size())
			return false;
		m_selectedIndex = index;
		return true;
	}
	bool Selection::select(const QString& element)
	{
		for (size_t i = 0; i < m_selection.size(); ++i)
		{
			if (m_selection[i] == element)
			{
				m_selectedIndex = i;
				return true;
			}
		}
		return false;
	}

	QJsonValue Selection::toJson() const
	{
		QJsonObject obj;
		obj["selection"] = QJsonArray::fromStringList(m_selection);
		obj["selected"] = m_selection[m_selectedIndex];
		return obj;
	}
	bool Selection::fromJson(const QJsonValue& value)
	{
		if(!value.isObject())
			return false;
		QJsonObject obj = value.toObject();
		if (!obj.contains("selection") || !obj.contains("selected"))
			return false;
		QJsonValue selection = obj["selection"];
		QJsonValue selected = obj["selected"];
		if (!selection.isArray() || !selected.isString())
			return false;
		QJsonArray array = selection.toArray();
		m_selection.clear();
		for(const auto & val : array)
		{
			if (!val.isString())
				return false;
			m_selection.push_back(val.toString());
		}
		QString selectedStr = selected.toString();
		for (size_t i = 0; i < m_selection.size(); ++i)
		{
			if (m_selection[i] == selectedStr)
			{
				m_selectedIndex = i;
				return true;
			}
		}
		return false;
	}
}