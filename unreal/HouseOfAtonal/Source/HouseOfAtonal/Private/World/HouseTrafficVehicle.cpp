#include "World/HouseTrafficVehicle.h"

#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

AHouseTrafficVehicle::AHouseTrafficVehicle()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		Visual->SetStaticMesh(Cube.Object);
	}
	Visual->SetRelativeScale3D(FVector(0.044f, 0.019f, 0.014f));
	Speed = 10.0f;
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
