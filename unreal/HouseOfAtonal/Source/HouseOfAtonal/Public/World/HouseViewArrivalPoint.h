#pragma once

#include "Experience/HouseExperienceTypes.h"
#include "GameFramework/Actor.h"
#include "HouseViewArrivalPoint.generated.h"

class UArrowComponent;

/** Manually movable player destination inside the View Level. */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseViewArrivalPoint : public AActor
{
	GENERATED_BODY()

public:
	AHouseViewArrivalPoint();

	UPROPERTY(VisibleAnywhere, Category = "Arrival")
	TObjectPtr<UArrowComponent> Direction;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Arrival")
	EHouseLocation Destination = EHouseLocation::Atrium;
};
