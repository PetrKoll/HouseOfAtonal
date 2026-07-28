#include "World/HouseViewArrivalPoint.h"

#include "Components/ArrowComponent.h"

AHouseViewArrivalPoint::AHouseViewArrivalPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	Direction = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"));
	SetRootComponent(Direction);
}
