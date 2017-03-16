#pragma once

#include "ElevationDataType.h"
#include "IConfigurable.h"
#include "QuadKey.h"
#include "GeoCoordinate.h"
#include "IConfigSection.h"

using namespace UtyDepend::Config;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Data
			{
				// Exposes elevation api for external usage.
				class FMapElevationLoader : public IConfigurable
				{
				private:
					EElevationDataType _eleDataType = static_cast<EElevationDataType>(0);

				public:
					// Loads elevation for given quadkey and point.
					double Load(FQuadKey* quadKey, FGeoCoordinate* coordinate);
					void Configure(IConfigSection* configSection);
				};
			}
		}
	}
}
