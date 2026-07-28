#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseInstancedMeshPlanter.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
class USceneComponent;
class UStaticMesh;

USTRUCT(BlueprintType)
struct HOUSEOFATONAL_API FHouseInstancedMeshPlanting
{
	GENERATED_BODY()

	/** Mesh shared by every transform in Instances. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planting")
	TObjectPtr<UStaticMesh> Mesh = nullptr;

	/**
	 * Actor-local transforms. Add elements with + and edit them directly in the
	 * level viewport using their transform widgets.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planting", meta = (MakeEditWidget))
	TArray<FTransform> Instances;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rendering")
	bool bCastShadow = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rendering")
	bool bAffectDistanceFieldLighting = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rendering")
	bool bAffectDynamicIndirectLighting = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled = ECollisionEnabled::NoCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rendering", meta = (ClampMin = "0"))
	int32 MinLOD = 0;
};

/**
 * Manually art-directed instanced-mesh planter.
 *
 * Add mesh groups and transforms in the Details panel. Each mesh group is
 * rendered by one HISM component while every transform remains individually
 * editable through a viewport widget.
 */
UCLASS(Blueprintable)
class HOUSEOFATONAL_API AHouseInstancedMeshPlanter : public AActor
{
	GENERATED_BODY()

public:
	AHouseInstancedMeshPlanter();

	virtual void OnConstruction(const FTransform& Transform) override;

	/** Mesh groups and their manually placed actor-local instances. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Instanced Planting", meta = (TitleProperty = "Mesh"))
	TArray<FHouseInstancedMeshPlanting> MeshPlantings;

	/** Rebuilds the generated HISM components after bulk property edits. */
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Instanced Planting")
	void RebuildInstances();

	/** Number of currently generated instances, useful for scene auditing. */
	UFUNCTION(BlueprintPure, Category = "Instanced Planting")
	int32 GetInstanceCount() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UHierarchicalInstancedStaticMeshComponent>> GeneratedComponents;

	void DestroyGeneratedComponents();
};
