#pragma once

#include "AppSettings_base.h"
#include <QStringList>
#include <QString>
#include <QVariant>
#include <QJsonValue>

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

		/// <summary>
		/// Interface to save the selection to a json file.
		/// </summary>
		QJsonValue toJson() const;

		/// <summary>
		/// Loads the selection from a json file.
		/// </summary>
		/// <param name="value"></param>
		/// <returns>true, of the loading was successfull</returns>
		bool fromJson(const QJsonValue& value);
	private:
		QStringList m_selection;
		size_t m_selectedIndex;
	};
}

Q_DECLARE_METATYPE(AppSettings::Selection)
