// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Porter : ModuleRules
{
	public Porter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTasks", "NavigationSystem", "AIModule", "UMG"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PrivateIncludePaths.Add("Porter");
		PrivateIncludePaths.Add("Porter/Hero");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
