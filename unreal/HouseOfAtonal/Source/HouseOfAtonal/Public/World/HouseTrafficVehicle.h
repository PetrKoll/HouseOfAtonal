#pragma once

#include "World/HouseSplineMover.h"
#include "HouseTrafficVehicle.generated.h"

/** Individually editable moving car; V1 renders a cube. */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseTrafficVehicle : public AHouseSplineMover
{
	GENERATED_BODY()

public:
	AHouseTrafficVehicle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic",
		meta = (ClampMin = "0.5", Units = "cm"))
	float MinimumSpacing = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic",
		meta = (ClampMin = "0.5", Units = "cm"))
	float IntersectionSafetyRadius = 5.0f;

protected:
	virtual float GetMovementMultiplier() const override;
};
