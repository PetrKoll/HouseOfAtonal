#include "World/HouseInstancedMeshPlanter.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"

AHouseInstancedMeshPlanter::AHouseInstancedMeshPlanter()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);
}

void AHouseInstancedMeshPlanter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RebuildInstances();
}

void AHouseInstancedMeshPlanter::RebuildInstances()
{
	DestroyGeneratedComponents();

	for (int32 PlantingIndex = 0; PlantingIndex < MeshPlantings.Num(); ++PlantingIndex)
	{
		const FHouseInstancedMeshPlanting& Planting = MeshPlantings[PlantingIndex];
		if (!IsValid(Planting.Mesh) || Planting.Instances.IsEmpty())
		{
			continue;
		}

		const FName ComponentName(*FString::Printf(TEXT("HISM_%02d_%s"), PlantingIndex, *Planting.Mesh->GetName()));
		UHierarchicalInstancedStaticMeshComponent* HISM =
			NewObject<UHierarchicalInstancedStaticMeshComponent>(this, ComponentName, RF_Transactional);

		HISM->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		HISM->SetupAttachment(SceneRoot);
		HISM->SetStaticMesh(Planting.Mesh);
		HISM->SetCollisionEnabled(Planting.CollisionEnabled);
		HISM->SetCastShadow(Planting.bCastShadow);
		HISM->bAffectDistanceFieldLighting = Planting.bAffectDistanceFieldLighting;
		HISM->bAffectDynamicIndirectLighting = Planting.bAffectDynamicIndirectLighting;
		HISM->MinLOD = Planting.MinLOD;
		HISM->SetMobility(EComponentMobility::Static);

		AddInstanceComponent(HISM);
		HISM->RegisterComponent();

		for (const FTransform& InstanceTransform : Planting.Instances)
		{
			HISM->AddInstance(InstanceTransform, false);
		}

		HISM->BuildTreeIfOutdated(true, false);
		GeneratedComponents.Add(HISM);
	}
}

int32 AHouseInstancedMeshPlanter::GetInstanceCount() const
{
	int32 Count = 0;
	for (const FHouseInstancedMeshPlanting& Planting : MeshPlantings)
	{
		if (IsValid(Planting.Mesh))
		{
			Count += Planting.Instances.Num();
		}
	}
	return Count;
}

void AHouseInstancedMeshPlanter::DestroyGeneratedComponents()
{
	for (UHierarchicalInstancedStaticMeshComponent* Component : GeneratedComponents)
	{
		if (IsValid(Component))
		{
			RemoveInstanceComponent(Component);
			Component->DestroyComponent();
		}
	}

	GeneratedComponents.Reset();

	// Covers generated components restored by editor transactions or Blueprint
	// reconstruction before the transient cache has been repopulated.
	TInlineComponentArray<UHierarchicalInstancedStaticMeshComponent*> ExistingComponents(this);
	for (UHierarchicalInstancedStaticMeshComponent* Component : ExistingComponents)
	{
		if (IsValid(Component) && Component->CreationMethod == EComponentCreationMethod::UserConstructionScript)
		{
			RemoveInstanceComponent(Component);
			Component->DestroyComponent();
		}
	}
}
