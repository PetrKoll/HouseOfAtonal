#pragma once

#include "World/HouseSplineMover.h"
#include "HouseTrafficVehicle.generated.h"

class UStaticMesh;

/** Individually editable moving car; V1 renders a cube. */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseTrafficVehicle : public AHouseSplineMover
{
	GENERATED_BODY()

public:
	AHouseTrafficVehicle();
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic",
		meta = (ClampMin = "0", ClampMax = "2"))
	int32 VehicleVariant = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Visual")
	FRotator VehicleMeshRotation = FRotator(0.0f, -90.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic",
		meta = (ClampMin = "0.5", Units = "cm"))
	float MinimumSpacing = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic",
		meta = (ClampMin = "0.5", Units = "cm"))
	float IntersectionSafetyRadius = 5.0f;

protected:
	virtual float GetMovementMultiplier() const override;

private:
	void ApplyVehicleVariant();

	UPROPERTY()
	TArray<TObjectPtr<UStaticMesh>> VehicleMeshes;
};
