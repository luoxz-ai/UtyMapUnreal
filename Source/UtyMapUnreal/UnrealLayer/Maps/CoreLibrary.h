#pragma once

#include "MapStorageType.h"
#include "Range.h"
#include "QuadKey.h"
#include "Element.h"
#include "ElevationDataType.h"
#include "GeoCoordinate.h"
#include <string>
#include <mutex>
#include "Range.h"
#include "MapTileAdapter.h"
#include "CoreLibrary.generated.h"

// send newly built mesh back to MapDataLoader
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMeshReceivedFromCoreLibrary, ATile*, Tile, FMesh&, Mesh);

// send newly built mesh back to MapDataLoader
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FElementReceivedFromCoreLibrary, ATile*, Tile, FElement&, Element);

// Provides the way to build tile encapsulating Utymap implementation.
UCLASS()
class UCoreLibrary : public UObject
{
	GENERATED_BODY()

private:
	static const std::string InMemoryStoreKey;
	static const std::string PersistentStoreKey;
	static std::string GetStoreKey(EMapStorageType* storageType);
	static std::string OnErrorCaller;

	static std::mutex __lockObj;
	static FCriticalSection CoreLibraryMutex;

	static bool _isConfigured;
	~UCoreLibrary();
	static UMapTileAdapter* _mapTileAdapter;


public:
	static void Init(UMapTileAdapter* mapTileAdapter);

	// Configure utymap. Should be called before any core API usage.
	// @param stringPath - Path to string table
	// @param mapDataPath - Path for map data
	// @param onError - OnError callback
	static void Configure(const std::string& stringPath, const std::string& mapDataPath, const std::string& MapCssFile, TLodRange<int32>& LodRange, UMapTileAdapter* MapTileAdapter);
	
	// Adds map data to in-memory storage to specific level of detail Range.
	// Supported formats: shapefile, osm xml, osm pbf.
	//
	// @param storageType - Map data storage
	// @param stylePath - Stylesheet path
	// @param path - Path to file
	// @param levelOfDetails - Specifies level of details for which data should be imported
	// @param onError - OnError callback
	static void AddToStore(EMapStorageType* storageType, const std::string& stylePath, const std::string& path, TLodRange<int>* levelOfDetails);

	//  Adds map data to in-memory storage to specific quadkey.
	//  Supported formats: shapefile, osm xml, osm pbf.
	//
	// @param storageType - Map data storage
	// @param stylePath - Stylesheet path
	// @param path - Path to file
	// @param quadKey - QuadKey
	// @param onError - OnError callback
	static void AddToStore(EMapStorageType* storageType, const std::string& stylePath, const std::string& path, FQuadKey* quadKey);

	static void AddElementToStore(EMapStorageType* storageType, const std::string& stylePath, FElement* element, TLodRange<int>* levelOfDetails);

	// Checks whether there is data for given quadkey.
	// @return  True if there is data for given quadkey.
	static bool HasData(FQuadKey* quadKey);

	// Gets elevation for given coordinate using specific elevation data.
	// @return Height under sea level.
	// @remarks Elevation data should be present on disk.
	static double GetElevation(FQuadKey* quadKey, EElevationDataType* elevationDataType, FGeoCoordinate* coordinate);

	// Loads quadkey
	// @param stylePath - Stylesheet path
	// @param quadKey - QuadKey
	// @param elevationDataType - Elevation data type.
	// @param onMeshBuilt
	// @param onElementLoaded
	// @param onError
	static void LoadQuadKey(const std::string& stylePath, FQuadKey* quadKey, EElevationDataType* elevationDataType);

	static void OnError(const char* msg);
	static void OnMeshBuilt(const char* name,   // name 
		const double* vertices, int vertexSize, // vertices (x, y, elevation)
		const int* triangles, int triSize,      // triangle indices
		const int* colors, int colorSize,       // rgba colors
		const double* uvs, int uvSize,          // absolute texture uvs
		const int* uvMap, int uvMapSize);       // map with info about used atlas and texture region
	static void OnElementLoaded(uint64 id,      // element id
		const char** tags, int tagsSize,        // tags
		const double* vertices, int vertexSize, // vertices (x, y, elevation)
		const char** style, int styleSize);     // mapcss styles (key, value)
};