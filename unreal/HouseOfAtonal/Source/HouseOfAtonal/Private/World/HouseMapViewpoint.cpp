#include "World/HouseMapViewpoint.h"

#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

AHouseMapViewpoint::AHouseMapViewpoint()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	ViewDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("ViewDirection"));
	ViewDirection->SetupAttachment(Root);
	ViewDirection->ArrowColor = FColor(70, 190, 255);
	ViewDirection->ArrowSize = 2.0f;
}
