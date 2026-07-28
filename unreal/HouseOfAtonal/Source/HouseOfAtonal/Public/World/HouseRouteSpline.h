#pragma once

#include "GameFramework/Actor.h"
#include "HouseRouteSpline.generated.h"

class USplineComponent;

/** Hand-editable route shared by cars or pedestrians. */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseRouteSpline : public AActor
{
	GENERATED_BODY()

public:
	AHouseRouteSpline();
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Route")
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	bool bClosedLoop = true;

	UFUNCTION(BlueprintPure, Category = "Route")
	float GetRouteLength() const;
};
