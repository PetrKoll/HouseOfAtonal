#include "World/HouseParkedVehicle.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AHouseParkedVehicle::AHouseParkedVehicle()
{
	PrimaryActorTick.bCanEverTick = false;

	Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placeholder"));
	SetRootComponent(Visual);
	Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Visual->SetCastShadow(false);
	Visual->SetRelativeScale3D(FVector(0.044f, 0.019f, 0.014f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		Visual->SetStaticMesh(Cube.Object);
	}
}
