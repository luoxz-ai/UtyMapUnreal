#include "UtyMapUnreal.h"
#include "ElementEditor.h"
#include "CoreLibrary.h"
#include "Callbacks.hpp"


using namespace UtyDepend;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Data
			{
				FElementEditor::FElementEditor(UStylesheet* stylesheet/*, IPathResolver* resolver*/) : _stylesheet(stylesheet)/*, _resolver(resolver)*/
				{
					// todo: implement
				}

				void FElementEditor::Add(EMapStorageType storageType, FElement* element, TLodRange<int>* levelOfDetails)
				{
					UCoreLibrary::AddElementToStore(&storageType, /*_resolver->Resolve(*/_stylesheet->GetPath(), element, levelOfDetails);
				}

				void FElementEditor::Edit(EMapStorageType storageType, FElement* element, TLodRange<int>* levelOfDetails)
				{
					UE_LOG(UtyLog, Error, TEXT("Element editing not implemented yet."));
				}

				void FElementEditor::Delete(EMapStorageType storageType, long long elementId, TLodRange<int>* levelOfDetails)
				{
					UE_LOG(UtyLog, Error, TEXT("Element deletion not implemented yet."));
				}
			}
		}
	}
}
