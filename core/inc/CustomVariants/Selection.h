#pragma once

#include "AppSettings_base.h"
#include <QStringList>
#include <QString>
#include <QVariant>

namespace AppSettings
{
	/// <summary>
	/// A selection can be used to select one option out of a list of possible options.
	/// It contains a list of possible options which each are strings.
	/// Only one can be selected at a time.
	/// </summary>
	class Selection
	{
		
	public:
		Selection();
		Selection(const Selection& other);
		Selection(const QStringList& selection);
		Selection(const QStringList& selection, size_t currentIndex);

		bool operator==(const Selection& other) const;
		bool operator!=(const Selection& other) const;

		bool select(size_t index);
		bool select(const QString& element);

		size_t getSelectedIndex() const { return m_selectedIndex; }
		const QString& getSelected() const { return m_selection[m_selectedIndex]; }

		const QStringList& getSelection() const { return m_selection; }


	private:
		const QStringList m_selection;
		size_t m_selectedIndex;
	};
}

Q_DECLARE_METATYPE(AppSettings::Selection)
