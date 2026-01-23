// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IpvMulti3 : ModuleRules
{
	public IpvMulti3(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemUtils" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "OnlineSubsystem",
			"OnlineSubsystemSteam", "UMG", "Slate", "SlateCore", "Networking", "Sockets" });
	}
}
