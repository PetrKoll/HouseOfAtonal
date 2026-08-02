#pragma once

#include "GameFramework/Actor.h"
#include "HouseTrafficManager.generated.h"

class AHouseRouteSpline;
class UHierarchicalInstancedStaticMeshComponent;
class USceneComponent;

/** One independently editable car spawned as a runtime HISM instance. */
USTRUCT(BlueprintType)
struct HOUSEOFATONAL_API FHouseTrafficVehicleSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	TObjectPtr<AHouseRouteSpline> Route;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle",
		meta = (ClampMin = "0", ClampMax = "2"))
	int32 VehicleVariant = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle",
		meta = (Units = "cm"))
	float StartDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle",
		meta = (ClampMin = "0.0", Units = "cm/s"))
	float Speed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	bool bReverseDirection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Transform")
	FVector Scale = FVector(0.01f);

	/** Rotation added to the route heading before rendering this car. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Transform")
	FRotator RotationOffset = FRotator(0.0f, -90.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Spacing",
		meta = (ClampMin = "0.0", Units = "cm"))
	float MinimumSpacing = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Spacing",
		meta = (ClampMin = "0.0", Units = "cm"))
	float IntersectionSafetyRadius = 5.0f;
};

/** Spawns and moves all traffic as three runtime HISM groups. */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseTrafficManager : public AActor
{
	GENERATED_BODY()

public:
	AHouseTrafficManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Complete authoring data. No individual vehicle actors are required. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	TArray<FHouseTrafficVehicleSettings> Vehicles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Performance",
		meta = (ClampMin = "0.016", ClampMax = "0.2", Units = "s"))
	float UpdateInterval = 0.05f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traffic|Instances")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> CarInstancesA;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traffic|Instances")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> CarInstancesB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traffic|Instances")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> CarInstancesC;

private:
	struct FRuntimeCar
	{
		TWeakObjectPtr<AHouseRouteSpline> Route;
		float Distance = 0.0f;
		float Speed = 0.0f;
		float MinimumSpacing = 0.0f;
		float IntersectionSafetyRadius = 0.0f;
		bool bReverse = false;
		int32 Variant = 0;
		int32 InstanceIndex = INDEX_NONE;
		FVector Scale = FVector::OneVector;
		FRotator RotationOffset = FRotator::ZeroRotator;
		FVector LastLocation = FVector::ZeroVector;
	};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	TArray<FRuntimeCar> RuntimeCars;

	UHierarchicalInstancedStaticMeshComponent* GetVariantComponent(int32 Variant) const;
	FTransform MakeCarTransform(const FRuntimeCar& Car) const;
	float GetMovementMultiplier(int32 CarIndex) const;
};
