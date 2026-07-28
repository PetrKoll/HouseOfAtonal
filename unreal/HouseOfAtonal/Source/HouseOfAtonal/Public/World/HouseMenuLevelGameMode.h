#pragma once

#include "GameFramework/GameModeBase.h"
#include "HouseMenuLevelGameMode.generated.h"

class AHouseMenuLevelBlockout;

/**
 * Self-contained V1 Menu Level setup.
 *
 * It guarantees a stationary XR pawn and can spawn the parametric diorama in
 * an otherwise empty map. The generated actor remains fully editable and can
 * later be placed explicitly in the final level.
 */
UCLASS()
class HOUSEOFATONAL_API AHouseMenuLevelGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHouseMenuLevelGameMode();
	virtual void StartPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Menu Level")
	TSubclassOf<AHouseMenuLevelBlockout> DioramaClass;

	UPROPERTY(EditDefaultsOnly, Category = "Menu Level")
	bool bSpawnDioramaWhenMissing = true;

	UPROPERTY(EditDefaultsOnly, Category = "Menu Level|Composition",
		meta = (Units = "cm"))
	FVector DioramaOffsetFromView = FVector(285.0f, 0.0f, -115.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Menu Level|Composition")
	FRotator DioramaRotation = FRotator(0.0f, 45.0f, 0.0f);
};
