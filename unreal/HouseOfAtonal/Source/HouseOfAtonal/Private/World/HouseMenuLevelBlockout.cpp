#include "World/HouseMenuLevelBlockout.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AHouseMenuLevelBlockout::AHouseMenuLevelBlockout()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	auto CreateInstances = [this](const FName Name)
	{
		UHierarchicalInstancedStaticMeshComponent* Component =
			CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(Name);
		Component->SetupAttachment(SceneRoot);
		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Component->SetMobility(EComponentMobility::Static);
		return Component;
	};

	GroundInstances = CreateInstances(TEXT("Ground"));
	RoadInstances = CreateInstances(TEXT("Roads"));
	SidewalkInstances = CreateInstances(TEXT("Sidewalks"));
	BuildingInstances = CreateInstances(TEXT("Buildings"));
	RoofInstances = CreateInstances(TEXT("RoofBlockouts"));
	TreePlaceholders = CreateInstances(TEXT("TreeAnchors"));
	CarPlaceholders = CreateInstances(TEXT("CarAnchors"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		for (UHierarchicalInstancedStaticMeshComponent* Component :
			{GroundInstances, RoadInstances, SidewalkInstances,
			 BuildingInstances, RoofInstances, TreePlaceholders,
			 CarPlaceholders})
		{
			Component->SetStaticMesh(CubeFinder.Object);
		}
	}

	HeroHouse = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeroHouse"));
	HeroHouse->SetupAttachment(SceneRoot);
	HeroHouse->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeroHouse->SetMobility(EComponentMobility::Static);

	PrimaryTrafficRoute =
		CreateDefaultSubobject<USplineComponent>(TEXT("TrafficRoutePrimary"));
	PrimaryTrafficRoute->SetupAttachment(SceneRoot);
	SecondaryTrafficRoute =
		CreateDefaultSubobject<USplineComponent>(TEXT("TrafficRouteSecondary"));
	SecondaryTrafficRoute->SetupAttachment(SceneRoot);
	CentralPedestrianLoop =
		CreateDefaultSubobject<USplineComponent>(TEXT("PedestrianLoopCentral"));
	CentralPedestrianLoop->SetupAttachment(SceneRoot);

	GoldenHourLight = CreateDefaultSubobject<UDirectionalLightComponent>(
		TEXT("GoldenHourDirectionalLight"));
	GoldenHourLight->SetupAttachment(SceneRoot);
	GoldenHourLight->SetRelativeRotation(FRotator(-28.0f, -42.0f, 0.0f));
	GoldenHourLight->SetCastShadows(true);

	HeroBeamLight = CreateDefaultSubobject<USpotLightComponent>(
		TEXT("HeroBeamLight"));
	HeroBeamLight->SetupAttachment(SceneRoot);
	HeroBeamLight->SetRelativeLocation(FVector(-130.0f, -110.0f, 190.0f));
	HeroBeamLight->SetRelativeRotation(FRotator(-48.0f, 40.0f, 0.0f));
	HeroBeamLight->SetInnerConeAngle(16.0f);
	HeroBeamLight->SetOuterConeAngle(28.0f);
	HeroBeamLight->SetAttenuationRadius(520.0f);
	HeroBeamLight->SetCastShadows(true);
}

void AHouseMenuLevelBlockout::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RebuildDiorama();
}

void AHouseMenuLevelBlockout::RebuildDiorama()
{
	ClearGenerated();
	FRandomStream Random(RandomSeed);

	const int32 SafeGridSize = FMath::Max(3, GridSize);
	TArray<float> BlockSizes;
	BlockSizes.Reserve(SafeGridSize);
	float TotalBlocks = 0.0f;
	for (int32 Index = 0; Index < SafeGridSize; ++Index)
	{
		const float Variation =
			Random.FRandRange(-BlockVariation, BlockVariation);
		const float Size = AverageBlockSize * (1.0f + Variation);
		BlockSizes.Add(Size);
		TotalBlocks += Size;
	}

	const float TotalSize =
		TotalBlocks + RoadWidth * static_cast<float>(SafeGridSize + 1);
	AddBoxInstance(
		GroundInstances,
		FVector(0.0f, 0.0f, -1.0f),
		FVector(TotalSize + 50.0f, TotalSize + 50.0f, 2.0f));

	TArray<float> Centers;
	Centers.SetNum(SafeGridSize);
	float Cursor = -TotalSize * 0.5f + RoadWidth;
	for (int32 Index = 0; Index < SafeGridSize; ++Index)
	{
		Centers[Index] = Cursor + BlockSizes[Index] * 0.5f;
		Cursor += BlockSizes[Index] + RoadWidth;
	}

	Cursor = -TotalSize * 0.5f + RoadWidth * 0.5f;
	for (int32 Boundary = 0; Boundary <= SafeGridSize; ++Boundary)
	{
		AddBoxInstance(
			RoadInstances,
			FVector(Cursor, 0.0f, 0.05f),
			FVector(RoadWidth, TotalSize, 0.1f));
		AddBoxInstance(
			RoadInstances,
			FVector(0.0f, Cursor, 0.04f),
			FVector(TotalSize, RoadWidth, 0.08f));
		if (Boundary < SafeGridSize)
		{
			Cursor += BlockSizes[Boundary] + RoadWidth;
		}
	}

	const int32 CenterIndex = SafeGridSize / 2;
	int32 RemainingTallBuildings = FMath::Max(0, TallBuildingCount);
	for (int32 X = 0; X < SafeGridSize; ++X)
	{
		for (int32 Y = 0; Y < SafeGridSize; ++Y)
		{
			const float BlockX = BlockSizes[X];
			const float BlockY = BlockSizes[Y];
			const FVector BlockCenter(Centers[X], Centers[Y], 0.25f);
			AddBoxInstance(
				SidewalkInstances,
				BlockCenter,
				FVector(BlockX, BlockY, 0.5f));

			const bool bHeroBlock = X == CenterIndex && Y == CenterIndex;
			const int32 LotsPerAxis = bHeroBlock ? 3 : Random.RandRange(2, 3);
			const float LotX = (BlockX - SidewalkWidth * 2.0f) / LotsPerAxis;
			const float LotY = (BlockY - SidewalkWidth * 2.0f) / LotsPerAxis;

			for (int32 LotIndexX = 0; LotIndexX < LotsPerAxis; ++LotIndexX)
			{
				for (int32 LotIndexY = 0; LotIndexY < LotsPerAxis; ++LotIndexY)
				{
					if (bHeroBlock &&
						LotIndexX >= 1 &&
						LotIndexY >= 1)
					{
						continue;
					}

					if (Random.FRand() < 0.16f)
					{
						continue;
					}

					const float FootprintX = LotX * Random.FRandRange(0.68f, 0.9f);
					const float FootprintY = LotY * Random.FRandRange(0.68f, 0.9f);
					int32 Floors = Random.RandRange(
						FMath::Max(1, MinimumFloors),
						FMath::Max(MinimumFloors, MaximumFloors));
					if (RemainingTallBuildings > 0 &&
						!bHeroBlock &&
						Random.FRand() < 0.045f)
					{
						Floors = FMath::Max(Floors, 3);
						--RemainingTallBuildings;
					}

					const float Height = Floors * Random.FRandRange(4.8f, 5.8f);
					const FVector BuildingCenter(
						BlockCenter.X - BlockX * 0.5f + SidewalkWidth +
							(LotIndexX + 0.5f) * LotX,
						BlockCenter.Y - BlockY * 0.5f + SidewalkWidth +
							(LotIndexY + 0.5f) * LotY,
						0.5f + Height * 0.5f);
					AddBoxInstance(
						BuildingInstances,
						BuildingCenter,
						FVector(FootprintX, FootprintY, Height),
						FRotator(0.0f, Random.FRandRange(-4.0f, 4.0f), 0.0f));

					if (Random.FRand() < 0.48f)
					{
						AddBoxInstance(
							RoofInstances,
							BuildingCenter + FVector(0.0f, 0.0f, Height * 0.5f + 1.0f),
							FVector(FootprintX * 0.82f, FootprintY * 0.82f, 2.0f),
							FRotator(
								Random.FRandRange(-16.0f, 16.0f),
								Random.FRandRange(-3.0f, 3.0f),
								0.0f));
					}
				}
			}

			if (bGenerateTreePlaceholders)
			{
				for (int32 TreeIndex = 0; TreeIndex < 3; ++TreeIndex)
				{
					const FVector TreePosition(
						BlockCenter.X + Random.FRandRange(-BlockX * 0.4f, BlockX * 0.4f),
						BlockCenter.Y + Random.FRandRange(-BlockY * 0.4f, BlockY * 0.4f),
						2.5f);
					AddBoxInstance(
						TreePlaceholders,
						TreePosition,
						FVector(1.2f, 1.2f, 5.0f));
				}
			}
		}
	}

	if (bGenerateCarPlaceholders)
	{
		for (int32 Index = 0; Index < 18; ++Index)
		{
			const int32 RoadIndex = Random.RandRange(0, SafeGridSize - 1);
			const bool bHorizontal = Index % 2 == 0;
			const FVector Position = bHorizontal
				? FVector(
					Random.FRandRange(-TotalSize * 0.45f, TotalSize * 0.45f),
					Centers[RoadIndex] - BlockSizes[RoadIndex] * 0.5f - RoadWidth * 0.5f,
					0.8f)
				: FVector(
					Centers[RoadIndex] - BlockSizes[RoadIndex] * 0.5f - RoadWidth * 0.5f,
					Random.FRandRange(-TotalSize * 0.45f, TotalSize * 0.45f),
					0.8f);
			AddBoxInstance(
				CarPlaceholders,
				Position,
				FVector(4.4f, 1.9f, 1.4f),
				FRotator(0.0f, bHorizontal ? 0.0f : 90.0f, 0.0f));
		}
	}

	ConfigureHero();
	BuildRoutes(Centers);

	GoldenHourLight->SetLightColor(GoldenHourColor);
	GoldenHourLight->SetIntensity(GoldenHourIntensity);
	HeroBeamLight->SetLightColor(GoldenHourColor);
	HeroBeamLight->SetIntensity(HeroBeamIntensity);
	HeroBeamLight->SetVolumetricScatteringIntensity(
		HeroBeamVolumetricScattering);
}

void AHouseMenuLevelBlockout::ClearGenerated()
{
	for (UHierarchicalInstancedStaticMeshComponent* Component :
		{GroundInstances, RoadInstances, SidewalkInstances, BuildingInstances,
		 RoofInstances, TreePlaceholders, CarPlaceholders})
	{
		Component->ClearInstances();
	}
}

void AHouseMenuLevelBlockout::ConfigureHero()
{
	HeroHouse->SetStaticMesh(HeroHouseMesh);
	HeroHouse->SetRelativeLocation(FVector(0.0f, 0.0f, HeroHouseVerticalOffset));
	HeroHouse->SetRelativeRotation(HeroHouseRotation);
	HeroHouse->SetRelativeScale3D(FVector(DioramaScale));
}

void AHouseMenuLevelBlockout::BuildRoutes(const TArray<float>& Centers)
{
	for (USplineComponent* Spline :
		{PrimaryTrafficRoute, SecondaryTrafficRoute, CentralPedestrianLoop})
	{
		Spline->ClearSplinePoints(false);
	}
	if (Centers.Num() < 3)
	{
		return;
	}

	const float Extent =
		FMath::Abs(Centers.Last() - Centers[0]) * 0.6f + AverageBlockSize;
	PrimaryTrafficRoute->AddSplinePoint(
		FVector(-Extent, 0.0f, 0.8f), ESplineCoordinateSpace::Local, false);
	PrimaryTrafficRoute->AddSplinePoint(
		FVector(Extent, 0.0f, 0.8f), ESplineCoordinateSpace::Local, true);
	SecondaryTrafficRoute->AddSplinePoint(
		FVector(0.0f, -Extent, 0.8f), ESplineCoordinateSpace::Local, false);
	SecondaryTrafficRoute->AddSplinePoint(
		FVector(0.0f, Extent, 0.8f), ESplineCoordinateSpace::Local, true);

	const float LoopExtent = AverageBlockSize * 0.62f;
	for (const FVector& Point :
		{FVector(-LoopExtent, -LoopExtent, 0.8f),
		 FVector(LoopExtent, -LoopExtent, 0.8f),
		 FVector(LoopExtent, LoopExtent, 0.8f),
		 FVector(-LoopExtent, LoopExtent, 0.8f)})
	{
		CentralPedestrianLoop->AddSplinePoint(
			Point, ESplineCoordinateSpace::Local, false);
	}
	CentralPedestrianLoop->SetClosedLoop(true, true);
}

void AHouseMenuLevelBlockout::AddBoxInstance(
	UHierarchicalInstancedStaticMeshComponent* Component,
	const FVector& Center,
	const FVector& Size,
	const FRotator& Rotation) const
{
	if (!IsValid(Component) || !Component->GetStaticMesh())
	{
		return;
	}
	Component->AddInstance(FTransform(
		Rotation,
		Center,
		Size / 100.0f));
}
