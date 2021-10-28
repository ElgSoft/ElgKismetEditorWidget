// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class ElgKismetEditorWidget : ModuleRules
{
	public ElgKismetEditorWidget(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = false;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/BP"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/BPWrappers"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Widgets"));

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/BP"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/BPWrappers"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Widgets"));

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
				new string[] {
				}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "Kismet",
                "EditorStyle",
                "BlueprintGraph",
                "UMGEditor",
				"Blutility",
                "UMG",
				"DeveloperSettings",
				"UnrealEd",
                "EditorSubsystem",
				"GraphEditor",
				"InputCore",
				"KismetWidgets",
				"GraphEditor",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
