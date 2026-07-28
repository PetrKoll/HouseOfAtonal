#pragma once

#include "GameFramework/Pawn.h"
#include "HouseMenuLevelPawn.generated.h"

class UCameraComponent;
class USceneComponent;

/**
 * Stationary XR viewpoint for the tabletop Menu Level.
 *
 * The pawn intentionally binds no locomotion or teleport input. OpenXR still
 * drives the camera inside the tracking origin, giving the diorama real
 * head-motion parallax.
 */
UCLASS()
class HOUSEOFATONAL_API AHouseMenuLevelPawn : public APawn
{
	GENERATED_BODY()

public:
	AHouseMenuLevelPawn();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Menu Level")
	TObjectPtr<USceneComponent> TrackingOrigin;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level")
	TObjectPtr<UCameraComponent> HeadCamera;
};
