// Copyright is held by Continental and the author Max Seidenstuecker

using UnrealBuildTool;
using System; // Console.WriteLine("");
using System.IO;
using System.Diagnostics;
using System.Collections.Generic;

public class UtyMapUnreal : ModuleRules
{
	public UtyMapUnreal(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RuntimeMeshComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

       // UtyMap core include dirs
        var UtyMapPath = "C:/workspace/projects/utymap/core";
        PublicIncludePaths.Add(Path.Combine(UtyMapPath, "lib"));
        PublicIncludePaths.Add(Path.Combine(UtyMapPath, "shared"));
        PublicIncludePaths.Add(Path.Combine(UtyMapPath, "src"));

        // boost for UtyMap
        var BoostIncludeDir = "C:/workspace/projects/libraries/boost_1_62_0";
        PublicIncludePaths.Add(BoostIncludeDir);

        PublicLibraryPaths.AddRange(
            new string[] {
                "C:/workspace/projects/utymap/build/shared/Debug"
            }
            );

        PublicAdditionalLibraries.AddRange(
            new string[] {
                "UtyMap.Shared.lib"
            }
        );

        // Delay-load the DLL, so we can load it from the right place first
        PublicDelayLoadDLLs.Add("UtyMap.Shared.dll");

        Definitions.AddRange(
            new string[] {
                "UTYMAP_USING_DLL"
            }
        );
    }
}
