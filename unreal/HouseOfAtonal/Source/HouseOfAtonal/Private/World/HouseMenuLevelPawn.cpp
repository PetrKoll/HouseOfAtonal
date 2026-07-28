#include "World/HouseMenuLevelPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"

AHouseMenuLevelPawn::AHouseMenuLevelPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	TrackingOrigin = CreateDefaultSubobject<USceneComponent>(
		TEXT("TrackingOrigin"));
	SetRootComponent(TrackingOrigin);

	HeadCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HeadCamera"));
	HeadCamera->SetupAttachment(TrackingOrigin);
	HeadCamera->bLockToHmd = true;
}
