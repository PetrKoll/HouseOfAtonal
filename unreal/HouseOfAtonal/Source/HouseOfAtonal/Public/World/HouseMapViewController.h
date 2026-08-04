#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseMapViewController.generated.h"

UENUM(BlueprintType)
enum class EHouseMapViewMode : uint8
{
	Neighborhood,
	Transition,
	Map
};
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseMapViewController : public AActor
{
	GENERATED_BODY()

public:
	AHouseMapViewController();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Map View|Viewpoints")
	TObjectPtr<AActor> NeighborhoodViewpoint;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Map View|Viewpoints")
	TObjectPtr<AActor> MapViewpoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map View|Transition",
		meta = (ClampMin = "0.1", UIMin = "1.0", UIMax = "15.0", Units = "s"))
	float TransitionDuration = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map View|Transition",
		meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float WorldSwapAlpha = 0.52f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map View|Editor Preview")
	EHouseMapViewMode EditorPreviewMode = EHouseMapViewMode::Neighborhood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map View|Groups")
	FName NeighborhoodTag = TEXT("House.MapView.Neighborhood");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map View|Groups")
	FName MapTag = TEXT("House.MapView.Map");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map View|Groups")
	FName TransitionCloudTag = TEXT("House.MapView.TransitionCloud");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map View|Groups")
	FName NeighborhoodLabelTag = TEXT("House.MapView.NeighborhoodLabel");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map View|Groups")
	FName MapLabelTag = TEXT("House.MapView.MapLabel");

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Map View|Editor Preview")
	void ApplyEditorPreview();

	UFUNCTION(BlueprintCallable, Category = "Map View")
	void ToggleMapView();

	UFUNCTION(BlueprintCallable, Category = "Map View")
	void EnterMapView();

	UFUNCTION(BlueprintCallable, Category = "Map View")
	void ExitMapView();

	UFUNCTION(BlueprintPure, Category = "Map View")
	bool IsInMapView() const { return bMapViewActive; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMapViewModeChanged, bool, bIsMapView);

	UPROPERTY(BlueprintAssignable, Category = "Map View")
	FMapViewModeChanged OnMapViewModeChanged;

private:
	void StartTransition(bool bToMap);
	void ApplyMode(EHouseMapViewMode Mode, bool bEditorPreview);
	void SetTaggedActorsVisible(FName Tag, bool bVisible, bool bEditorPreview) const;
	APawn* ResolvePlayerPawn() const;

	bool bMapViewActive = false;
	bool bTransitionActive = false;
	bool bTransitionToMap = false;
	bool bWorldSwapped = false;
	float TransitionElapsed = 0.0f;
	FTransform TransitionStartTransform;
	FTransform TransitionTargetTransform;
	FTransform SavedNeighborhoodTransform;
};
