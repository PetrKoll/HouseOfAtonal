#pragma once

#include "World/HouseSplineMover.h"
#include "HousePedestrian.generated.h"

/** Individually editable pedestrian; V1 renders a cylinder. */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHousePedestrian : public AHouseSplineMover
{
	GENERATED_BODY()

public:
	AHousePedestrian();
};
