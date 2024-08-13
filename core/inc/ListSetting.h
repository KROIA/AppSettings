#pragma once

#include "AppSettings_base.h"
#include "ISetting.h"
#include <QVariant>
#include <vector>

namespace AppSettings
{
	class APP_SETTINGS_EXPORT ListSetting : public ISetting
	{
	public:
		ListSetting();
		ListSetting(const ListSetting& other);
		ListSetting(const QString& name);
		ListSetting(const QString& name, const std::vector<QVariant>& list);

		bool operator==(const ListSetting& other);
		bool operator!=(const ListSetting& other);

		ListSetting& operator=(const ListSetting& other);
		QVariant operator[](size_t index) { return m_list[index]; }
		void set(size_t index, const QVariant& value) { m_list[index] = value; emit valueChanged(); }

		void setName(const QString& name) override { m_name = name; emit nameChanged(); }
		QString getName() const override { return m_name; }
		QString toString() const override;

		size_t size() const { return m_list.size(); }
		void reserve(size_t size) { m_list.reserve(size); }
		void push_back(const QVariant& value) {
			m_list.push_back(value); 
			emit valueChanged();
		}
		void emplace_back(QVariant&& value) { 
			m_list.emplace_back(std::move(value)); 
			emit valueChanged();
		}
		void setData(const std::vector<QVariant>& data)
		{
			m_list = data;
			emit valueChanged();
		}
		void clear() { 
			m_list.clear();
			emit valueChanged();
		}

		const std::vector<QVariant>& getData() const {	return m_list;	}

	protected:
		void save(QJsonObject &settings) const override;
		bool load(const QJsonObject &settings) override;

	private:

		QString m_name;
		std::vector<QVariant> m_list;
	};
}