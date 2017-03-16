using UnrealBuildTool;
using System;
using System.IO;
using System.Diagnostics;
using System.Collections.Generic;

public class UtyMapUnreal : ModuleRules
{
	public UtyMapUnreal(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ShaderCore", "RenderCore", "RHI", "RuntimeMeshComponent" });

        // Add all subdirecotries of Utymap layer to include path
        var UnrealLayerPath = Path.Combine(ModuleDirectory, "UnrealLayer");
        foreach (var SubFolder in Directory.EnumerateDirectories(UnrealLayerPath, "*", SearchOption.AllDirectories))
        {
            PublicIncludePaths.Add(SubFolder);
        }

        // turn on RTTI for packed vertices in RuntimeMeshCompoenent (used for multiple UVs)
        bUseRTTI = true;

        // enable exception hanlding for UtyMap library
        UEBuildConfiguration.bForceEnableExceptions = true;

        // UtyMap core include dirs
        var UtyMapPath = "C:/workspace/UnrealProjects/utymap_max/core";
        PublicIncludePaths.Add(Path.Combine(UtyMapPath, "lib"));
        PublicIncludePaths.Add(Path.Combine(UtyMapPath, "shared"));
        PublicIncludePaths.Add(Path.Combine(UtyMapPath, "src"));

        // boost for UtyMap
        var BoostIncludeDir = "C:/workspace/UnrealProjects/libraries/boost_1_62_0";
        PublicIncludePaths.Add(BoostIncludeDir);

        // setup DLL usage
        Definitions.AddRange(
            new string[] {
                "UTYMAP_USING_DLL"
            }
        );

        PublicLibraryPaths.AddRange(
            new string[] {
                "C:/workspace/UnrealProjects/utymap_max/build/shared/Release"
            }
            );

        PublicAdditionalLibraries.AddRange(
            new string[] {
                "UtyMap.Shared.lib"
            }
        );

        PublicDelayLoadDLLs.Add("UtyMap.Shared.dll");
	}
}
