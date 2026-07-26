using UnrealBuildTool;

public class HouseOfAtonal : ModuleRules
{
	public HouseOfAtonal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"EnhancedInput",
				"HeadMountedDisplay",
				"InputCore",
				"SlateCore",
				"UMG"
			}
		);
	}
}
