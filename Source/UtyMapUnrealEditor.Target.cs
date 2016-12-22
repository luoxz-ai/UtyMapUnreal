// Copyright is held by Continental and the author Max Seidenstuecker

using UnrealBuildTool;
using System.Collections.Generic;

public class UtyMapUnrealEditorTarget : TargetRules
{
	public UtyMapUnrealEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "UtyMapUnreal" } );
	}
}
