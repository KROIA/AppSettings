#include "CustomVariants/Selection.h"


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
}