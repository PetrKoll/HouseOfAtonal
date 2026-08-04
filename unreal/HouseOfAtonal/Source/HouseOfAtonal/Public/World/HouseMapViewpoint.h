#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseMapViewpoint.generated.h"

class UArrowComponent;
class USceneComponent;

UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseMapViewpoint : public AActor
{
	GENERATED_BODY()

public:
	AHouseMapViewpoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Viewpoint")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Viewpoint")
	TObjectPtr<UArrowComponent> ViewDirection;
};
