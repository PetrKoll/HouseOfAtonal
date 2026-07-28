#include "World/HouseMenuLevelGameMode.h"

#include "EngineUtils.h"
#include "Experience/HouseExperienceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "World/HouseMenuLevelPawn.h"
#include "World/HouseViewArrivalPoint.h"

AHouseMenuLevelGameMode::AHouseMenuLevelGameMode()
{
	DefaultPawnClass = AHouseMenuLevelPawn::StaticClass();
}

void AHouseMenuLevelGameMode::StartPlay()
{
	Super::StartPlay();
	if (!GetWorld() ||
		!GetWorld()->GetMapName().Contains(TEXT("L_ViewLevel")))
	{
		return;
	}

	UHouseExperienceSubsystem* Experience =
		GetGameInstance()->GetSubsystem<UHouseExperienceSubsystem>();
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Experience || !Pawn)
	{
		return;
	}

	for (TActorIterator<AHouseViewArrivalPoint> It(GetWorld()); It; ++It)
	{
		if (It->Destination == Experience->GetLocation())
		{
			Pawn->SetActorLocationAndRotation(
				It->GetActorLocation(),
				It->GetActorRotation());
			return;
		}
	}
}
