#pragma once

#include "CoreMinimal.h"
#include "HouseExperienceTypes.generated.h"

UENUM(BlueprintType)
enum class EHouseLocation : uint8
{
	Menu,
	Exterior,
	Interior,
	Atrium,
	RoomOne,
	RoomTwo
};

UENUM(BlueprintType)
enum class EHouseWeatherPreset : uint8
{
	Clear,
	Cloudy,
	Rain,
	Storm,
	Fog,
	Snow
};

UENUM(BlueprintType)
enum class EHouseTimePreset : uint8
{
	Dawn,
	Morning,
	Noon,
	GoldenHour,
	Evening,
	Night
};
