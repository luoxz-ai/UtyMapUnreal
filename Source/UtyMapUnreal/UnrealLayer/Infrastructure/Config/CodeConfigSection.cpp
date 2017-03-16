#include "UtyMapUnreal.h"
#include "CodeConfigSection.h"

using namespace UtyDepend::Config;
namespace UtyMap
{
	namespace Unreal
	{
		namespace Infrastructure
		{
			namespace Config
			{

				FCodeConfigSection::FCodeConfigSection() : _keyPrefix(std::string("")), _keyValueMap(std::unordered_map<std::string, std::string>())
				{
				}

				FCodeConfigSection::FCodeConfigSection(const std::string& keyPrefix, std::unordered_map<std::string, std::string>& keyValueMap) : _keyPrefix(keyPrefix), _keyValueMap(keyValueMap)
				{
				}

				FCodeConfigSection* FCodeConfigSection::Add(const std::string& path, std::string value)
				{
					auto key = GetKey(path);
					if (_keyValueMap.find(key) != _keyValueMap.end())
					{
						_keyValueMap[ key ] = value;
					}
					else
					{
						_keyValueMap.insert(std::make_pair(key, value));
					}
					return this;
				}

				std::vector<IConfigSection*> FCodeConfigSection::GetSections(const std::string& xpath)
				{
					// todo_ log
					//throw NotSupportedException();
					return std::vector<IConfigSection*>();
				}

				IConfigSection* FCodeConfigSection::GetSection(const std::string& xpath)
				{
					return new FCodeConfigSection(GetKey(xpath), _keyValueMap);
				}

				std::string FCodeConfigSection::GetString(const std::string& xpath, const std::string& defaultValue)
				{
					auto key = GetKey(xpath);
					if (_keyValueMap.find(key) != _keyValueMap.end())
					{
//C# TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'ToString':
						return _keyValueMap[ key ];
					}
					return defaultValue;
				}

				int FCodeConfigSection::GetInt(const std::string& xpath, int defaultValue)
				{
					// todo: it won't work as long as it gets fed a formated string
					return std::stoi(xpath);
					//return int::TryParse(GetString(xpath, std::string("")), value) ? value : defaultValue;
				}

				float FCodeConfigSection::GetFloat(const std::string& xpath, float defaultValue)
				{
					// todo: it won't work as long as it gets fed a formated string
					return std::stof(xpath); 
					//return float::TryParse(GetString(xpath, std::string("")), value) ? value : defaultValue;
				}

				bool FCodeConfigSection::GetBool(const std::string& xpath, bool defaultValue)
				{
					// todo: it won't work as long as it gets fed a formated string
					return (std::stoi(xpath) != 0);
					//return bool::TryParse(GetString(xpath, std::string("")), value) ? value : defaultValue;
				}

				std::string FCodeConfigSection::GetKey(const std::string& path)
				{
					// todo: it won't work as long as it gets fed a formated string
					return _keyValueMap[path];
					//return std::string::Format(std::string(R"({0}/{1})"), _keyPrefix, path);
				}
			}
		}
	}
}
