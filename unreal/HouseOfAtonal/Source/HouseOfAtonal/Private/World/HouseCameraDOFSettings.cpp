#include "World/HouseCameraDOFSettings.h"

#include "Components/PostProcessComponent.h"
#include "Components/SceneComponent.h"

AHouseCameraDOFSettings::AHouseCameraDOFSettings()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(Root);
	PostProcess->bUnbound = true;
	PostProcess->Priority = 100.0f;
	ApplySettings();
}

void AHouseCameraDOFSettings::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplySettings();
}

void AHouseCameraDOFSettings::ApplySettings()
{
	if (!PostProcess)
	{
		return;
	}

	FPostProcessSettings& Settings = PostProcess->Settings;
	Settings.bOverride_DepthOfFieldFocalDistance = bEnableDepthOfField;
	Settings.bOverride_DepthOfFieldFstop = bEnableDepthOfField;
	Settings.bOverride_DepthOfFieldFocalRegion = bEnableDepthOfField;
	Settings.bOverride_DepthOfFieldNearTransitionRegion = bEnableDepthOfField;
	Settings.bOverride_DepthOfFieldFarTransitionRegion = bEnableDepthOfField;
	Settings.DepthOfFieldFocalDistance = FocusDistance;
	Settings.DepthOfFieldFstop = Aperture;
	Settings.DepthOfFieldFocalRegion = FocalRegion;
	Settings.DepthOfFieldNearTransitionRegion = NearTransition;
	Settings.DepthOfFieldFarTransitionRegion = FarTransition;
	PostProcess->BlendWeight = bEnableDepthOfField ? BlendWeight : 0.0f;
}
