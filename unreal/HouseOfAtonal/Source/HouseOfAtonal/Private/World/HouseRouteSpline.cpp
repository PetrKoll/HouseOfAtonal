#include "World/HouseRouteSpline.h"

#include "Components/SplineComponent.h"

AHouseRouteSpline::AHouseRouteSpline()
{
	PrimaryActorTick.bCanEverTick = false;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Route"));
	SetRootComponent(Spline);
	Spline->SetDrawDebug(true);
}

void AHouseRouteSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Spline->SetClosedLoop(bClosedLoop);
}

float AHouseRouteSpline::GetRouteLength() const
{
	return Spline ? Spline->GetSplineLength() : 0.0f;
}
