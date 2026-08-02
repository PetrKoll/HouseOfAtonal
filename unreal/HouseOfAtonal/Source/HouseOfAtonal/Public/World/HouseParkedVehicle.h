#pragma once

#include "GameFramework/Actor.h"
#include "HouseParkedVehicle.generated.h"

class UStaticMeshComponent;
class UStaticMesh;

/** Individually editable zero-tick parked car; V1 renders a cube. */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseParkedVehicle : public AActor
{
	GENERATED_BODY()

public:
	AHouseParkedVehicle();
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parked Vehicle")
	TObjectPtr<UStaticMeshComponent> Visual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parked Vehicle")
	bool bOccupied = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parked Vehicle",
		meta = (ClampMin = "0", ClampMax = "2"))
	int32 VehicleVariant = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parked Vehicle|Visual")
	FRotator VehicleMeshRotation = FRotator(0.0f, -90.0f, 0.0f);

private:
	void ApplyVehicleVariant();

	UPROPERTY()
	TArray<TObjectPtr<UStaticMesh>> VehicleMeshes;
};
