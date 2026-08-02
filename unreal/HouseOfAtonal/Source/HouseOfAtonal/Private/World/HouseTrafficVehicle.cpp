#include "World/HouseTrafficVehicle.h"

#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

AHouseTrafficVehicle::AHouseTrafficVehicle()
{
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
	Visual->SetCastShadow(true);
	bCastPlaceholderShadow = true;
	ApplyVehicleVariant();
	Speed = 10.0f;
}

void AHouseTrafficVehicle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplyVehicleVariant();
}

void AHouseTrafficVehicle::ApplyVehicleVariant()
{
	if (Visual && VehicleMeshes.IsValidIndex(VehicleVariant))
	{
		Visual->SetStaticMesh(VehicleMeshes[VehicleVariant]);
		Visual->SetRelativeRotation(VehicleMeshRotation);
		Visual->SetCastShadow(true);
	}
}

float AHouseTrafficVehicle::GetMovementMultiplier() const
{
	if (!GetWorld())
	{
		return 1.0f;
	}

	const FVector Forward = GetActorForwardVector();
	for (TActorIterator<AHouseTrafficVehicle> It(GetWorld()); It; ++It)
	{
		const AHouseTrafficVehicle* Other = *It;
		if (Other == this)
		{
			continue;
		}
		const FVector Offset = Other->GetActorLocation() - GetActorLocation();
		const float Distance = Offset.Size2D();
		const bool bAhead = FVector::DotProduct(
			Forward, Offset.GetSafeNormal2D()) > 0.15f;
		if (bAhead && Distance < IntersectionSafetyRadius)
		{
			return 0.0f;
		}
		if (Other->Route == Route &&
			Other->bReverseDirection == bReverseDirection &&
			bAhead &&
			Distance < MinimumSpacing)
		{
			return FMath::Clamp(
				(Distance - IntersectionSafetyRadius) /
					FMath::Max(0.1f, MinimumSpacing - IntersectionSafetyRadius),
				0.0f,
				1.0f);
		}
	}
	return 1.0f;
}
