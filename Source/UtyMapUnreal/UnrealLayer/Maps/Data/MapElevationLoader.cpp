#include "UtyMapUnreal.h"
#include "MapElevationLoader.h"

using namespace UtyDepend::Config;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Data
			{

				double FMapElevationLoader::Load(FQuadKey* quadKey, FGeoCoordinate* coordinate)
				{
					return UCoreLibrary::GetElevation(quadKey, &_eleDataType, coordinate);
				}

				void FMapElevationLoader::Configure(IConfigSection* configSection)
				{
					_eleDataType = static_cast<EElevationDataType>(configSection->GetInt(std::string("data/elevation/type"), 0));
				}
			}
		}
	}
}
