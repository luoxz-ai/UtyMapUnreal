#pragma once

#include <string>
#include <vector>

namespace UtyDepend
{
	namespace Config
	{
		// Represens a config entry.
		class IConfigSection
		{
			// Returns the set of ConfigSections.
			// @param xpath">xpath// 
		public:
			virtual std::vector<IConfigSection*> GetSections(const std::string& xpath) = 0;

			// Returns JsonConfigSection.
			// @param xpath">xpath// 
			// @return IConfigSection
			virtual IConfigSection* GetSection(const std::string& xpath) = 0;

			// Returns string.
			// @param xpath">xpath
			// @param defaultValue">// 
			// @return String value
			virtual std::string GetString(const std::string& xpath, const std::string& defaultValue = "") = 0;

			// Returns int.
			// @param xpath">// 
			// @param defaultValue">// 
			// @return Int value
			virtual int GetInt(const std::string& xpath, int defaultValue) = 0;

			// Returns float.
			// @param xpath">xpath// 
			// @param defaultValue">Default value
			// @return Float value
			virtual float GetFloat(const std::string& xpath, float defaultValue) = 0;

			// Returns bool.
			// @param xpath">xpath// 
			// @param defaultValue">Default value
			// @return Boolean
			virtual bool GetBool(const std::string& xpath, bool defaultValue) = 0;
		};
	}
}
