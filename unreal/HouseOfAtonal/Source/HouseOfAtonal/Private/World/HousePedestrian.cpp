#include "World/HousePedestrian.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AHousePedestrian::AHousePedestrian()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(
		TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (Cylinder.Succeeded())
	{
		Visual->SetStaticMesh(Cylinder.Object);
	}
	Visual->SetRelativeScale3D(FVector(0.006f, 0.006f, 0.018f));
	Speed = 2.3f;
	UpdateInterval = 0.05f;
}
