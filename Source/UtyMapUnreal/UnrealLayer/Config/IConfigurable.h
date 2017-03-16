#pragma once


namespace UtyDepend { namespace Config { class IConfigSection; } }

namespace UtyDepend
{
	namespace Config
	{
		// Defines configurable class behaviour.
		class IConfigurable
		{
			// Configures object using configuration section.
			// @param configSection">Configuration section
		public:
			virtual void Configure(IConfigSection* configSection) = 0;
		};
	}
}
