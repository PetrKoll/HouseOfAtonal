#pragma once

#include "GameFramework/GameModeBase.h"
#include "HouseMenuLevelGameMode.generated.h"

/**
 * Stationary XR game mode for the hand-authored Menu Level.
 *
 * Scene composition and the player viewpoint are intentionally owned by
 * normal level actors. This class never moves or regenerates the city.
 */
UCLASS()
class HOUSEOFATONAL_API AHouseMenuLevelGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHouseMenuLevelGameMode();
	virtual void StartPlay() override;
};
