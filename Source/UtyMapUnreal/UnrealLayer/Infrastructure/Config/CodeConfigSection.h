#pragma once

#include "IConfigSection.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>



using namespace UtyDepend::Config;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Infrastructure
		{
			namespace Config
			{
				// Represents code-based configuration section.
				/// <remarks> This is naive, not performance optimized implementation.</remarks>
				class FCodeConfigSection : public IConfigSection
				{
				private:
					//struct FVoidPointerHash {
					//	size_t operator()(const std::string &string) const
					//	{
					//		return std::hash<std::string>()
					//		// todo: check if this hash is valid
					//		int hash = 17;
					//		for (int i = 0; i < string.first.size(); i++)
					//		{
					//			hash = hash * 29 + string.first[i];
					//		}
					//		return hash;
					//	}
					//};

					const std::string _keyPrefix;
					std::unordered_map<std::string, std::string> _keyValueMap;

				public:
					// Creates instance of <see cref="CodeConfigSection"/>.
					FCodeConfigSection();

				private:
					FCodeConfigSection(const std::string& keyPrefix, std::unordered_map<std::string, std::string>& keyValueMap);

				public:
					// Adds specific setting.
					FCodeConfigSection* Add(const std::string& path, std::string value);


					
					std::vector<IConfigSection*> GetSections(const std::string& xpath);

					
					IConfigSection* GetSection(const std::string& xpath);

					
					std::string GetString(const std::string& xpath, const std::string& defaultValue);

					
					int GetInt(const std::string& xpath, int defaultValue);

					
					float GetFloat(const std::string& xpath, float defaultValue);

					
					bool GetBool(const std::string& xpath, bool defaultValue);

				private:
					std::string GetKey(const std::string& path);
				};
			}
		}
	}
}
