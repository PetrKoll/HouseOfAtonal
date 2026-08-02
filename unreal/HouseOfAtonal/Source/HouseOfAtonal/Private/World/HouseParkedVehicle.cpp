#include "World/HouseParkedVehicle.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AHouseParkedVehicle::AHouseParkedVehicle()
{
	PrimaryActorTick.bCanEverTick = false;

	Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placeholder"));
	SetRootComponent(Visual);
	Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Visual->SetCastShadow(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CarA(
		TEXT("/Game/HouseOfAtonal/models/cars/car_A.car_A"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CarB(
		TEXT("/Game/HouseOfAtonal/models/cars/car_b.car_b"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CarC(
		TEXT("/Game/HouseOfAtonal/models/cars/car_C.car_C"));
	if (CarA.Succeeded())
	{
		VehicleMeshes.Add(CarA.Object);
	}
	if (CarB.Succeeded())
	{
		VehicleMeshes.Add(CarB.Object);
	}
	if (CarC.Succeeded())
	{
		VehicleMeshes.Add(CarC.Object);
	}
	ApplyVehicleVariant();
}

void AHouseParkedVehicle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplyVehicleVariant();
}

void AHouseParkedVehicle::ApplyVehicleVariant()
{
	if (Visual && VehicleMeshes.IsValidIndex(VehicleVariant))
	{
		Visual->SetStaticMesh(VehicleMeshes[VehicleVariant]);
		Visual->SetRelativeRotation(VehicleMeshRotation);
		Visual->SetCastShadow(true);
	}
}
