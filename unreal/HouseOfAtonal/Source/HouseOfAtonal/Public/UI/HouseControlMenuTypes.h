#pragma once

#include "CoreMinimal.h"
#include "HouseControlMenuTypes.generated.h"

UENUM(BlueprintType)
enum class EHouseControlMenuCategory : uint8
{
	None,
	Destination,
	Weather,
	TimeOfDay,
	HouseState
};
