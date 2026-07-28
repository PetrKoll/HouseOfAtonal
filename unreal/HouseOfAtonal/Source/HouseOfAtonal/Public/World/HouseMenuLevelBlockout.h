#pragma once

#include "GameFramework/Actor.h"
#include "HouseMenuLevelBlockout.generated.h"

class UDirectionalLightComponent;
class UHierarchicalInstancedStaticMeshComponent;
class USceneComponent;
class USplineComponent;
class USpotLightComponent;
class UStaticMesh;
class UStaticMeshComponent;

/**
 * Parametric tabletop Menu Level blockout.
 *
 * One Unreal centimeter represents one real-world meter by default. The
 * imported hero house therefore uses DioramaScale while the generated context
 * is authored directly in tabletop centimeters.
 */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseMenuLevelBlockout : public AActor
{
	GENERATED_BODY()

public:
	AHouseMenuLevelBlockout();
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Menu Level")
	void RebuildDiorama();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Menu Level")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Generated")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> GroundInstances;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Generated")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> RoadInstances;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Generated")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> SidewalkInstances;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Generated")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> BuildingInstances;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Generated")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> RoofInstances;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Generated")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> TreePlaceholders;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Generated")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> CarPlaceholders;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Hero")
	TObjectPtr<UStaticMeshComponent> HeroHouse;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Routes")
	TObjectPtr<USplineComponent> PrimaryTrafficRoute;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Routes")
	TObjectPtr<USplineComponent> SecondaryTrafficRoute;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Routes")
	TObjectPtr<USplineComponent> CentralPedestrianLoop;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Lighting")
	TObjectPtr<UDirectionalLightComponent> GoldenHourLight;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Lighting")
	TObjectPtr<USpotLightComponent> HeroBeamLight;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Layout",
		meta = (ClampMin = "3", ClampMax = "12"))
	int32 GridSize = 7;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Layout",
		meta = (ClampMin = "20.0", ClampMax = "80.0", Units = "cm"))
	float AverageBlockSize = 42.0f;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Layout",
		meta = (ClampMin = "4.0", ClampMax = "20.0", Units = "cm"))
	float RoadWidth = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Layout",
		meta = (ClampMin = "1.0", ClampMax = "8.0", Units = "cm"))
	float SidewalkWidth = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Layout")
	int32 RandomSeed = 1701;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Layout",
		meta = (ClampMin = "0.0", ClampMax = "0.4"))
	float BlockVariation = 0.16f;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Hero",
		meta = (ClampMin = "0.001", ClampMax = "1.0"))
	float DioramaScale = 0.01f;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Hero")
	TObjectPtr<UStaticMesh> HeroHouseMesh;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Hero")
	FRotator HeroHouseRotation = FRotator(0.0f, 35.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Menu Level|Hero",
		meta = (ClampMin = "-50.0", ClampMax = "50.0", Units = "cm"))
	float HeroHouseVerticalOffset = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Context",
		meta = (ClampMin = "1", ClampMax = "6"))
	int32 MinimumFloors = 1;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Context",
		meta = (ClampMin = "1", ClampMax = "6"))
	int32 MaximumFloors = 3;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Context",
		meta = (ClampMin = "0", ClampMax = "4"))
	int32 TallBuildingCount = 2;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Context")
	bool bGenerateTreePlaceholders = true;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Context")
	bool bGenerateCarPlaceholders = true;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Lighting")
	FLinearColor GoldenHourColor =
		FLinearColor(1.0f, 0.47f, 0.18f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Menu Level|Lighting",
		meta = (ClampMin = "0.0"))
	float GoldenHourIntensity = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Lighting",
		meta = (ClampMin = "0.0"))
	float HeroBeamIntensity = 18000.0f;

	UPROPERTY(EditAnywhere, Category = "Menu Level|Lighting",
		meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float HeroBeamVolumetricScattering = 0.0f;

private:
	void ClearGenerated();
	void ConfigureHero();
	void BuildRoutes(const TArray<float>& Centers);
	void AddBoxInstance(
		UHierarchicalInstancedStaticMeshComponent* Component,
		const FVector& Center,
		const FVector& Size,
		const FRotator& Rotation = FRotator::ZeroRotator) const;
};
