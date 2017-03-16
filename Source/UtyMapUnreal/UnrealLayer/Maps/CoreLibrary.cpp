#include "UtyMapUnreal.h"
#include "CoreLibrary.h"
#include "ExportLib.hpp"
#include "Callbacks.hpp"
#include "MapTileAdapter.h"
#include "Async.h"

// test defines
#define TEST_ASSETS_PATH "C:/workspace/UnrealProjects/utymap_max/core/test/test_assets/"

#define TEST_EXTERNAL_ASSETS_PATH TEST_ASSETS_PATH "../../../unity/demo/Assets/StreamingAssets/"

#define TEST_MAPCSS_PATH TEST_ASSETS_PATH "mapcss/"
#define TEST_MAPCSS_DEFAULT TEST_EXTERNAL_ASSETS_PATH "MapCss/default/default.mapcss"

#define TEST_JSON_FILE TEST_EXTERNAL_ASSETS_PATH "Osm/moscow.osm.json"
#define TEST_PBF_FILE TEST_EXTERNAL_ASSETS_PATH "Osm/berlin.osm.pbf"
#define TEST_XML_FILE TEST_EXTERNAL_ASSETS_PATH "Osm/berlin.osm.xml"

#define TEST_SHAPE_POINT_FILE TEST_ASSETS_PATH "shape/artificial/point"
#define TEST_SHAPE_LINE_FILE TEST_ASSETS_PATH "shape/artificial/line"
#define TEST_SHAPE_POLY_FILE TEST_ASSETS_PATH "shape/artificial/poly"
#define TEST_SHAPE_MULTIPOLY_FILE TEST_ASSETS_PATH "shape/artificial/multipoly"

std::mutex UCoreLibrary::__lockObj;
UMapTileAdapter* UCoreLibrary::_mapTileAdapter;
const std::string UCoreLibrary::InMemoryStoreKey = "InMemory";
const std::string UCoreLibrary::PersistentStoreKey = "Persistent";
bool UCoreLibrary::_isConfigured = false;
std::string UCoreLibrary::OnErrorCaller;

UCoreLibrary::~UCoreLibrary()
{
	// NOTE: do not allow to call cleanup as configure method can be called only once (see above)
	// So, let OS release resources once app has been finished
	//cleanup();
}

void UCoreLibrary::Init(UMapTileAdapter* mapTileAdapter)
{
	_mapTileAdapter = mapTileAdapter;
}

void UCoreLibrary::Configure(const std::string& IndexFolderPath, const std::string& MapDataFile, const std::string& MapCssFile, TLodRange<int32>& LodRange, UMapTileAdapter* MapTileAdapter)
{
	{
		std::lock_guard<std::mutex> lock(__lockObj);

		// Do files exist?
		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(UTF8_TO_TCHAR(MapDataFile.c_str())))
		{
			UE_LOG(UtyInit, Error, TEXT("Cannot find Map %s"), UTF8_TO_TCHAR(MapDataFile.c_str()));
			return;
		}
		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(UTF8_TO_TCHAR(MapCssFile.c_str())))
		{
			UE_LOG(UtyInit, Error, TEXT("Cannot find MapCSS %s"), UTF8_TO_TCHAR(MapCssFile.c_str()));
			return;
		}
		
		if ( !MapTileAdapter->IsValidLowLevel())
		{
			UE_LOG(UtyInit, Error, TEXT("MapTileAdapter invalid"));
			return;
		}
		_mapTileAdapter = MapTileAdapter;

		if (_isConfigured)
		{
			return;
		}
		OnErrorCaller = "Configure";

		configure(IndexFolderPath.c_str(), OnError);
		registerInMemoryStore(InMemoryStoreKey.c_str());
		registerPersistentStore(PersistentStoreKey.c_str(), IndexFolderPath.c_str());
		addToStoreInRange(InMemoryStoreKey.c_str(), MapCssFile.c_str(), MapDataFile.c_str(), LodRange.Minimum, LodRange.Maximum, &UCoreLibrary::OnError);

		_isConfigured = true;
	}
}

void UCoreLibrary::AddToStore(EMapStorageType* storageType, const std::string& stylePath, const std::string& path, TLodRange<int>* levelOfDetails)
{
	UE_LOG(UtyProcessing, Display, TEXT("add to %s storage: data: %s using style: %s"), UTF8_TO_TCHAR(UCoreLibrary::GetStoreKey(storageType).c_str()), UTF8_TO_TCHAR(path.c_str()), UTF8_TO_TCHAR(stylePath.c_str()));
	addToStoreInRange(GetStoreKey(storageType).c_str(), stylePath.c_str(), path.c_str(), levelOfDetails->Minimum, levelOfDetails->Maximum, &UCoreLibrary::OnError);
}

void UCoreLibrary::AddToStore(EMapStorageType* storageType, const std::string& stylePath, const std::string& path, FQuadKey* quadKey)
{
	UE_LOG(UtyProcessing, Display, TEXT("add to %s storage: data: %s using style: %s"), UTF8_TO_TCHAR(UCoreLibrary::GetStoreKey(storageType).c_str()), UTF8_TO_TCHAR(path.c_str()), UTF8_TO_TCHAR(stylePath.c_str()));
	addToStoreInQuadKey(GetStoreKey(storageType).c_str(), stylePath.c_str(), path.c_str(), quadKey->TileX, quadKey->TileY, quadKey->LevelOfDetail, &UCoreLibrary::OnError);
}

void UCoreLibrary::AddElementToStore(EMapStorageType* storageType, const std::string& stylePath, FElement* element, TLodRange<int>* levelOfDetails)
{
//C# TO C++ CONVERTER WARNING: C# to C++ Converter has converted this array to a pointer. You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: double[] coordinates = new double[element.Geometry.Length*2];
	auto coordinatesSize = element->Geometry.size() * 2;
	double* coordinates = new double[coordinatesSize];
	for (int i = 0; i < element->Geometry.size(); ++i)
	{
		coordinates[ i*2 ] = element->Geometry[ i ].Latitude;
		coordinates[ i*2 + 1 ] = element->Geometry[ i ].Longitude;
	}

//C# TO C++ CONVERTER WARNING: C# to C++ Converter has converted this array to a pointer. You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: string[] tags = new string[element.Tags.Count * 2];
	auto tagSize = element->Geometry.size() * 2;
	const char** tags = new const char*[ element->Tags.size() * 2 ];
	int i = 0;
	for (const auto& n : element->Tags) {
		tags[i * 2] = new char[n.first.size() + 1];
		tags[i * 2 + 1] = new char[n.second.size() + 1];
		++i;
	}

//C# TO C++ CONVERTER TODO TASK: Native C++ has no direct way to determine the length or upper bound of a specific dimension of an array:
	addToStoreElement(GetStoreKey(storageType).c_str(), stylePath.c_str(), element->Id, coordinates, coordinatesSize, tags, tagSize, levelOfDetails->Minimum, levelOfDetails->Maximum, &UCoreLibrary::OnError);
}

bool UCoreLibrary::HasData(FQuadKey* quadKey)
{
	return hasData(quadKey->TileX, quadKey->TileY, quadKey->LevelOfDetail);
}

double UCoreLibrary::GetElevation(FQuadKey* quadKey, EElevationDataType* elevationDataType, FGeoCoordinate* coordinate)
{
	return getElevation(quadKey->TileX, quadKey->TileY, quadKey->LevelOfDetail, static_cast<int>(*elevationDataType), coordinate->Latitude, coordinate->Longitude);
}

void UCoreLibrary::LoadQuadKey(const std::string& stylePath, FQuadKey* quadKey, EElevationDataType* elevationDataType)
{
	if (!_mapTileAdapter->IsValidLowLevel())
	{
		UE_LOG(UtyProcessing, Error, TEXT("MapTileAdapter not valid anymore in CoreLibrary. Not loading, as no callbacks could be received"));
		return;
	}

	auto Result = Async<void>(EAsyncExecution::Thread, [=]() {
		OnErrorCaller = "LoadQuadKey";
		return loadQuadKey(
			stylePath.c_str(),
			quadKey->TileX,
			quadKey->TileY,
			quadKey->LevelOfDetail,
			static_cast<int>(*elevationDataType),
			&UCoreLibrary::OnMeshBuilt,
			&UCoreLibrary::OnElementLoaded,
			&UCoreLibrary::OnError
		);
	});
}

std::string UCoreLibrary::GetStoreKey(EMapStorageType* storageType)
{
	return *storageType == EMapStorageType::InMemory ? InMemoryStoreKey : PersistentStoreKey;
}

void UCoreLibrary::OnError(const char* msg)
{
	//_lockObj.lock();
	UE_LOG(UtyProcessing, Error, TEXT("Error in %s function of CoreLibrary:"), UTF8_TO_TCHAR(OnErrorCaller.c_str()));
	UE_LOG(UtyProcessing, Error, TEXT("%s"), UTF8_TO_TCHAR(msg));
	//_lockObj.unlock();
}

void UCoreLibrary::OnMeshBuilt(const char* name, /* name */ const double* vertices, int vertexSize, /* vertices (x, y, elevation) */ const int* triangles, int triSize, /* triangle indices */ const int* colors, int colorSize, /* rgba colors */ const double* uvs, int uvSize, /* absolute texture uvs */ const int* uvMap, int uvMapSize)
{
	if (!_mapTileAdapter->IsValidLowLevel())
	{
		UE_LOG(UtyProcessing, Error, TEXT("maptileadpter invalid - dropping mesh"));
		return;
	}
	__lockObj.lock();
	//UE_LOG(UtyProcessing, Display, TEXT("Received mesh from core library: %s"), UTF8_TO_TCHAR(name));
	_mapTileAdapter->AdaptMesh(name, vertices, vertexSize, triangles, triSize, colors, colorSize, uvs, uvSize, uvMap, uvMapSize);
	__lockObj.unlock();
}

void UCoreLibrary::OnElementLoaded(uint64 id, /* element id */ const char** tags, int tagsSize, /* tags */ const double* vertices, int vertexSize, /* vertices (x, y, elevation) */ const char** style, int styleSize)
{
	if (!_mapTileAdapter->IsValidLowLevel())
	{
		UE_LOG(UtyProcessing, Error, TEXT("maptileadpter invalid - dropping element"));
		return;
	}
	__lockObj.lock();
	//UE_LOG(UtyProcessing, Display, TEXT("Received element from core library: %s"), UTF8_TO_TCHAR(std::to_string(id).c_str()));
	_mapTileAdapter->AdaptElement(id, tags, tagsSize, vertices, vertexSize, style, styleSize);
	__lockObj.unlock();
}
