// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sprata7 : ModuleRules
{
	public Sprata7(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
