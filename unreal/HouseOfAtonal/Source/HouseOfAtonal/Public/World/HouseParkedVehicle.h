#pragma once

#include "GameFramework/Actor.h"
#include "HouseParkedVehicle.generated.h"

class UStaticMeshComponent;

/** Individually editable zero-tick parked car; V1 renders a cube. */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseParkedVehicle : public AActor
{
	GENERATED_BODY()

public:
	AHouseParkedVehicle();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parked Vehicle")
	TObjectPtr<UStaticMeshComponent> Visual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parked Vehicle")
	bool bOccupied = false;
};
