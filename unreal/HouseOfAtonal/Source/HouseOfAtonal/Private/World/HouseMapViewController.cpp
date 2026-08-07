#include "World/HouseMapViewController.h"

#include "Components/LightComponent.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

AHouseMapViewController::AHouseMapViewController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AHouseMapViewController::BeginPlay()
{
	Super::BeginPlay();
	CacheAuthoredLightIntensities();
	for (const TPair<TWeakObjectPtr<ULightComponent>, float>& Pair : AuthoredLightIntensities)
	{
		if (ULightComponent* Light = Pair.Key.Get())
		{
			const AActor* LightOwner = Light->GetOwner();
			Light->SetIntensity(
				LightOwner && LightOwner->ActorHasTag(MapLightTag) ? 0.0f : Pair.Value);
		}
	}
	ApplyMode(EHouseMapViewMode::Neighborhood, false);
}

void AHouseMapViewController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!bTransitionActive)
	{
		SetActorTickEnabled(false);
		return;
	}

	APawn* Pawn = ResolvePlayerPawn();
	if (!Pawn)
	{
		bTransitionActive = false;
		SetActorTickEnabled(false);
		return;
	}

	TransitionElapsed += DeltaSeconds;
	const float Alpha = FMath::Clamp(TransitionElapsed / TransitionDuration, 0.0f, 1.0f);
	const float SmoothAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);
	ApplyLightTransition(SmoothAlpha);
	const FVector Location = FMath::Lerp(
		TransitionStartTransform.GetLocation(), TransitionTargetTransform.GetLocation(), SmoothAlpha);
	Pawn->SetActorLocation(Location);

	if (!bWorldSwapped && Alpha >= WorldSwapAlpha)
	{
		bWorldSwapped = true;
		ApplyMode(bTransitionToMap ? EHouseMapViewMode::Map : EHouseMapViewMode::Neighborhood, false);
	}

	if (Alpha >= 1.0f)
	{
		Pawn->SetActorLocation(TransitionTargetTransform.GetLocation());
		bTransitionActive = false;
		bMapViewActive = bTransitionToMap;
		SetActorTickEnabled(false);
		OnMapViewModeChanged.Broadcast(bMapViewActive);
	}
}

void AHouseMapViewController::ApplyEditorPreview()
{
	ApplyMode(EditorPreviewMode, true);
}

void AHouseMapViewController::ToggleMapView()
{
	if (bTransitionActive)
	{
		return;
	}
	bMapViewActive ? ExitMapView() : EnterMapView();
}

void AHouseMapViewController::EnterMapView()
{
	StartTransition(true);
}

void AHouseMapViewController::ExitMapView()
{
	StartTransition(false);
}

void AHouseMapViewController::StartTransition(const bool bToMap)
{
	APawn* Pawn = ResolvePlayerPawn();
	if (!Pawn || !IsValid(MapViewpoint))
	{
		return;
	}

	if (bToMap)
	{
		SavedNeighborhoodTransform = Pawn->GetActorTransform();
	}

	bTransitionToMap = bToMap;
	bTransitionActive = true;
	bWorldSwapped = false;
	TransitionElapsed = 0.0f;
	TransitionStartTransform = Pawn->GetActorTransform();
	FVector TargetLocation = Pawn->GetActorLocation();
	if (bToMap)
	{
		TargetLocation = MapViewpoint->GetActorLocation();
	}
	else if (!SavedNeighborhoodTransform.Equals(FTransform::Identity))
	{
		TargetLocation = SavedNeighborhoodTransform.GetLocation();
	}
	else if (IsValid(NeighborhoodViewpoint))
	{
		TargetLocation = NeighborhoodViewpoint->GetActorLocation();
	}
	TransitionTargetTransform = Pawn->GetActorTransform();
	TransitionTargetTransform.SetLocation(TargetLocation);
	PrepareLightTransition();
	ApplyMode(EHouseMapViewMode::Transition, false);
	SetActorTickEnabled(true);
}

void AHouseMapViewController::ApplyMode(
	const EHouseMapViewMode Mode, const bool bEditorPreview)
{
	const bool bShowNeighborhood = Mode == EHouseMapViewMode::Neighborhood ||
		(Mode == EHouseMapViewMode::Transition && bTransitionToMap);
	const bool bShowMap = Mode == EHouseMapViewMode::Map ||
		(Mode == EHouseMapViewMode::Transition && !bTransitionToMap);
	const bool bShowClouds = Mode == EHouseMapViewMode::Transition || Mode == EHouseMapViewMode::Map;

	SetTaggedActorsVisible(NeighborhoodTag, bShowNeighborhood, bEditorPreview);
	SetTaggedActorsVisible(NeighborhoodLabelTag, bShowNeighborhood, bEditorPreview);
	SetTaggedActorsVisible(MapTag, bShowMap, bEditorPreview);
	SetTaggedActorsVisible(MapLabelTag, bShowMap, bEditorPreview);
	SetTaggedActorsVisible(TransitionCloudTag, bShowClouds, bEditorPreview);
	SetTaggedActorsVisible(
		NeighborhoodLightTag,
		Mode == EHouseMapViewMode::Neighborhood || Mode == EHouseMapViewMode::Transition,
		bEditorPreview);
	SetTaggedActorsVisible(
		MapLightTag,
		Mode == EHouseMapViewMode::Map || Mode == EHouseMapViewMode::Transition,
		bEditorPreview);
	ApplyCustomVisibilityRules(Mode, bEditorPreview);
}

void AHouseMapViewController::ApplyCustomVisibilityRules(
	const EHouseMapViewMode Mode, const bool bEditorPreview) const
{
	for (const FHouseMapVisibilityRule& Rule : VisibilityRules)
	{
		if (!IsValid(Rule.Actor))
		{
			continue;
		}

		const bool bVisible = Mode == EHouseMapViewMode::Neighborhood
			? Rule.bVisibleInNeighborhood
			: Mode == EHouseMapViewMode::Map
				? Rule.bVisibleInMap
				: (bTransitionToMap
					? Rule.bVisibleInNeighborhood
					: Rule.bVisibleInMap);
		Rule.Actor->SetActorHiddenInGame(!bVisible);
#if WITH_EDITOR
		if (bEditorPreview)
		{
			Rule.Actor->SetIsTemporarilyHiddenInEditor(!bVisible);
		}
#endif
	}
}

void AHouseMapViewController::SetTaggedActorsVisible(
	const FName Tag, const bool bVisible, const bool bEditorPreview) const
{
	if (!GetWorld() || Tag.IsNone())
	{
		return;
	}

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || Actor == this || !Actor->ActorHasTag(Tag))
		{
			continue;
		}
		Actor->SetActorHiddenInGame(!bVisible);
#if WITH_EDITOR
		if (bEditorPreview)
		{
			Actor->SetIsTemporarilyHiddenInEditor(!bVisible);
		}
#endif
	}
}

void AHouseMapViewController::CacheAuthoredLightIntensities()
{
	if (!GetWorld())
	{
		return;
	}
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->ActorHasTag(NeighborhoodLightTag) && !Actor->ActorHasTag(MapLightTag))
		{
			continue;
		}
		if (ULightComponent* Light = Actor->FindComponentByClass<ULightComponent>())
		{
			AuthoredLightIntensities.FindOrAdd(Light) = Light->Intensity;
		}
	}
}

void AHouseMapViewController::PrepareLightTransition()
{
	if (AuthoredLightIntensities.IsEmpty())
	{
		CacheAuthoredLightIntensities();
	}
	LightTransitionStarts.Reset();
	LightTransitionTargets.Reset();

	for (const TPair<TWeakObjectPtr<ULightComponent>, float>& Pair : AuthoredLightIntensities)
	{
		ULightComponent* Light = Pair.Key.Get();
		AActor* LightOwner = Light ? Light->GetOwner() : nullptr;
		if (!Light || !LightOwner)
		{
			continue;
		}
		const bool bIsMapLight = LightOwner->ActorHasTag(MapLightTag);
		LightTransitionStarts.Add(Light, Light->Intensity);
		LightTransitionTargets.Add(Light, bIsMapLight == bTransitionToMap ? Pair.Value : 0.0f);
		LightOwner->SetActorHiddenInGame(false);
	}
}

void AHouseMapViewController::ApplyLightTransition(const float Alpha)
{
	for (const TPair<TWeakObjectPtr<ULightComponent>, float>& Pair : LightTransitionTargets)
	{
		ULightComponent* Light = Pair.Key.Get();
		const float* Start = LightTransitionStarts.Find(Pair.Key);
		if (Light && Start)
		{
			Light->SetIntensity(FMath::Lerp(*Start, Pair.Value, Alpha));
		}
	}
}

APawn* AHouseMapViewController::ResolvePlayerPawn() const
{
	if (!GetWorld())
	{
		return nullptr;
	}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	return PlayerController ? PlayerController->GetPawn() : nullptr;
}
