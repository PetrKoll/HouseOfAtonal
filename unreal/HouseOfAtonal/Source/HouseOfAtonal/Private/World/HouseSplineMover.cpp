#include "World/HouseSplineMover.h"

#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "World/HouseRouteSpline.h"

AHouseSplineMover::AHouseSplineMover()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = UpdateInterval;

	Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placeholder"));
	SetRootComponent(Visual);
	Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Visual->SetCastShadow(true);
}

void AHouseSplineMover::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.TickInterval = UpdateInterval;
	Visual->SetCastShadow(bCastPlaceholderShadow);
	DistanceAlongRoute = StartDistance;
	if (bPreservePlacedRotationOffset && Route && Route->Spline)
	{
		RouteRotationOffset =
			(GetActorRotation() - GetRouteRotation(DistanceAlongRoute))
			.GetNormalized();
	}
	ApplyRouteTransform();
}

void AHouseSplineMover::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!bMovementEnabled || !Route || !Route->Spline)
	{
		return;
	}

	const float Direction = bReverseDirection ? -1.0f : 1.0f;
	DistanceAlongRoute +=
		Speed * GetMovementMultiplier() * Direction * DeltaSeconds;
	const float Length = Route->Spline->GetSplineLength();
	if (Length > KINDA_SMALL_NUMBER)
	{
		DistanceAlongRoute = FMath::Fmod(DistanceAlongRoute, Length);
		if (DistanceAlongRoute < 0.0f)
		{
			DistanceAlongRoute += Length;
		}
	}
	ApplyRouteTransform();
}

float AHouseSplineMover::GetMovementMultiplier() const
{
	return 1.0f;
}

void AHouseSplineMover::SnapToRoute()
{
	DistanceAlongRoute = StartDistance;
	ApplyRouteTransform();
}

void AHouseSplineMover::ApplyRouteTransform()
{
	if (!Route || !Route->Spline)
	{
		return;
	}
	const FVector Location = Route->Spline->GetLocationAtDistanceAlongSpline(
		DistanceAlongRoute, ESplineCoordinateSpace::World);
	FRotator Rotation = GetRouteRotation(DistanceAlongRoute);
	Rotation += RouteRotationOffset;
	SetActorLocationAndRotation(Location, Rotation.GetNormalized());
}

FRotator AHouseSplineMover::GetRouteRotation(const float Distance) const
{
	if (!Route || !Route->Spline)
	{
		return FRotator::ZeroRotator;
	}

	FRotator Rotation = Route->Spline->GetRotationAtDistanceAlongSpline(
		Distance, ESplineCoordinateSpace::World);
	if (bReverseDirection)
	{
		Rotation.Yaw += 180.0f;
	}
	return Rotation.GetNormalized();
}
