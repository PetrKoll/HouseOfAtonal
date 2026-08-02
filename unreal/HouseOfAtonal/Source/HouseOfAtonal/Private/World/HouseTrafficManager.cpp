#include "World/HouseTrafficManager.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "World/HouseRouteSpline.h"

AHouseTrafficManager::AHouseTrafficManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = UpdateInterval;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	auto ConfigureInstances = [this](
		const FName Name,
		UHierarchicalInstancedStaticMeshComponent*& Component)
	{
		Component = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(Name);
		Component->SetupAttachment(Root);
		Component->SetMobility(EComponentMobility::Movable);
		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Component->SetCastShadow(true);
		Component->bAffectDynamicIndirectLighting = true;
		Component->bAffectDistanceFieldLighting = true;
	};

	UHierarchicalInstancedStaticMeshComponent* InstancesA = nullptr;
	UHierarchicalInstancedStaticMeshComponent* InstancesB = nullptr;
	UHierarchicalInstancedStaticMeshComponent* InstancesC = nullptr;
	ConfigureInstances(TEXT("CarInstancesA"), InstancesA);
	ConfigureInstances(TEXT("CarInstancesB"), InstancesB);
	ConfigureInstances(TEXT("CarInstancesC"), InstancesC);
	CarInstancesA = InstancesA;
	CarInstancesB = InstancesB;
	CarInstancesC = InstancesC;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CarA(
		TEXT("/Game/HouseOfAtonal/models/cars/car_A.car_A"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CarB(
		TEXT("/Game/HouseOfAtonal/models/cars/car_b.car_b"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CarC(
		TEXT("/Game/HouseOfAtonal/models/cars/car_C.car_C"));
	if (CarA.Succeeded()) CarInstancesA->SetStaticMesh(CarA.Object);
	if (CarB.Succeeded()) CarInstancesB->SetStaticMesh(CarB.Object);
	if (CarC.Succeeded()) CarInstancesC->SetStaticMesh(CarC.Object);
}

void AHouseTrafficManager::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.TickInterval = UpdateInterval;
	RuntimeCars.Reset();
	CarInstancesA->ClearInstances();
	CarInstancesB->ClearInstances();
	CarInstancesC->ClearInstances();

	for (const FHouseTrafficVehicleSettings& Settings : Vehicles)
	{
		if (!Settings.bEnabled || !Settings.Route || !Settings.Route->Spline)
		{
			continue;
		}

		FRuntimeCar& Car = RuntimeCars.AddDefaulted_GetRef();
		Car.Route = Settings.Route;
		Car.Distance = Settings.StartDistance;
		Car.Speed = Settings.Speed;
		Car.MinimumSpacing = Settings.MinimumSpacing;
		Car.IntersectionSafetyRadius = Settings.IntersectionSafetyRadius;
		Car.bReverse = Settings.bReverseDirection;
		Car.Variant = FMath::Clamp(Settings.VehicleVariant, 0, 2);
		Car.Scale = Settings.Scale;
		Car.RotationOffset = Settings.RotationOffset;

		const FTransform Transform = MakeCarTransform(Car);
		Car.LastLocation = Transform.GetLocation();
		if (UHierarchicalInstancedStaticMeshComponent* Instances =
			GetVariantComponent(Car.Variant))
		{
			Car.InstanceIndex = Instances->AddInstance(Transform, true);
		}
	}
}

void AHouseTrafficManager::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (int32 Index = 0; Index < RuntimeCars.Num(); ++Index)
	{
		FRuntimeCar& Car = RuntimeCars[Index];
		AHouseRouteSpline* Route = Car.Route.Get();
		if (!Route || !Route->Spline || Car.InstanceIndex == INDEX_NONE)
		{
			continue;
		}

		const float Direction = Car.bReverse ? -1.0f : 1.0f;
		Car.Distance += Car.Speed * GetMovementMultiplier(Index) * Direction * DeltaSeconds;
		const float Length = Route->Spline->GetSplineLength();
		if (Length > KINDA_SMALL_NUMBER)
		{
			Car.Distance = FMath::Fmod(Car.Distance, Length);
			if (Car.Distance < 0.0f) Car.Distance += Length;
		}

		const FTransform Transform = MakeCarTransform(Car);
		Car.LastLocation = Transform.GetLocation();
		GetVariantComponent(Car.Variant)->UpdateInstanceTransform(
			Car.InstanceIndex, Transform, true, true, true);
	}
}

UHierarchicalInstancedStaticMeshComponent* AHouseTrafficManager::GetVariantComponent(
	const int32 Variant) const
{
	switch (Variant)
	{
	case 1: return CarInstancesB;
	case 2: return CarInstancesC;
	default: return CarInstancesA;
	}
}

FTransform AHouseTrafficManager::MakeCarTransform(const FRuntimeCar& Car) const
{
	AHouseRouteSpline* Route = Car.Route.Get();
	if (!Route || !Route->Spline)
	{
		return FTransform::Identity;
	}

	const FVector Location = Route->Spline->GetLocationAtDistanceAlongSpline(
		Car.Distance, ESplineCoordinateSpace::World);
	FRotator Rotation = Route->Spline->GetRotationAtDistanceAlongSpline(
		Car.Distance, ESplineCoordinateSpace::World);
	if (Car.bReverse) Rotation.Yaw += 180.0f;
	Rotation += Car.RotationOffset;
	return FTransform(Rotation.GetNormalized(), Location, Car.Scale);
}

float AHouseTrafficManager::GetMovementMultiplier(const int32 CarIndex) const
{
	const FRuntimeCar& Car = RuntimeCars[CarIndex];
	for (int32 OtherIndex = 0; OtherIndex < RuntimeCars.Num(); ++OtherIndex)
	{
		if (OtherIndex == CarIndex) continue;
		const FRuntimeCar& Other = RuntimeCars[OtherIndex];
		const float Distance = FVector::Dist2D(Car.LastLocation, Other.LastLocation);
		if (Distance < Car.IntersectionSafetyRadius)
		{
			return 0.0f;
		}
		if (Car.Route == Other.Route && Car.bReverse == Other.bReverse &&
			Distance < Car.MinimumSpacing)
		{
			return FMath::Clamp(
				(Distance - Car.IntersectionSafetyRadius) /
				FMath::Max(0.1f, Car.MinimumSpacing - Car.IntersectionSafetyRadius),
				0.0f, 1.0f);
		}
	}
	return 1.0f;
}
