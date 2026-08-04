#include "World/HouseMapViewController.h"

#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

AHouseMapViewController::AHouseMapViewController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
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
	const FVector Location = FMath::Lerp(
		TransitionStartTransform.GetLocation(), TransitionTargetTransform.GetLocation(), SmoothAlpha);
	const FQuat Rotation = FQuat::Slerp(
		TransitionStartTransform.GetRotation(), TransitionTargetTransform.GetRotation(), SmoothAlpha);
	Pawn->SetActorLocationAndRotation(Location, Rotation.Rotator());

	if (!bWorldSwapped && Alpha >= WorldSwapAlpha)
	{
		bWorldSwapped = true;
		ApplyMode(bTransitionToMap ? EHouseMapViewMode::Map : EHouseMapViewMode::Neighborhood, false);
	}

	if (Alpha >= 1.0f)
	{
		Pawn->SetActorTransform(TransitionTargetTransform);
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
	TransitionTargetTransform = bToMap
		? MapViewpoint->GetActorTransform()
		: (SavedNeighborhoodTransform.Equals(FTransform::Identity)
			? (IsValid(NeighborhoodViewpoint)
				? NeighborhoodViewpoint->GetActorTransform()
				: Pawn->GetActorTransform())
			: SavedNeighborhoodTransform);
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

APawn* AHouseMapViewController::ResolvePlayerPawn() const
{
	if (!GetWorld())
	{
		return nullptr;
	}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	return PlayerController ? PlayerController->GetPawn() : nullptr;
}
