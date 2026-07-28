#include "World/HouseMenuLevelGameMode.h"

#include "Camera/PlayerCameraManager.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "World/HouseMenuLevelBlockout.h"
#include "World/HouseMenuLevelPawn.h"

AHouseMenuLevelGameMode::AHouseMenuLevelGameMode()
{
	DefaultPawnClass = AHouseMenuLevelPawn::StaticClass();
	DioramaClass = AHouseMenuLevelBlockout::StaticClass();
}

void AHouseMenuLevelGameMode::StartPlay()
{
	Super::StartPlay();

	if (!bSpawnDioramaWhenMissing || !DioramaClass)
	{
		return;
	}

	for (TActorIterator<AHouseMenuLevelBlockout> It(GetWorld()); It; ++It)
	{
		return;
	}

	APlayerCameraManager* CameraManager =
		UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (!CameraManager)
	{
		return;
	}

	const FVector ViewLocation = CameraManager->GetCameraLocation();
	const FRotator ViewRotation = CameraManager->GetCameraRotation();
	const FRotator YawOnlyRotation(0.0f, ViewRotation.Yaw, 0.0f);
	const FVector WorldOffset = YawOnlyRotation.RotateVector(
		DioramaOffsetFromView);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AHouseMenuLevelBlockout>(
		DioramaClass,
		ViewLocation + WorldOffset,
		YawOnlyRotation + DioramaRotation,
		SpawnParameters);
}
