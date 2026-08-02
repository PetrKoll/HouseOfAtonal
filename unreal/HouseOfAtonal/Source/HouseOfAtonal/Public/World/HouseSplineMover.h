#pragma once

#include "GameFramework/Actor.h"
#include "HouseSplineMover.generated.h"

class AHouseRouteSpline;
class UStaticMeshComponent;

/** Lightweight base for individually editable diorama traffic actors. */
UCLASS(Abstract, BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseSplineMover : public AActor
{
	GENERATED_BODY()

public:
	AHouseSplineMover();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mover")
	TObjectPtr<UStaticMeshComponent> Visual;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mover")
	TObjectPtr<AHouseRouteSpline> Route;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover",
		meta = (ClampMin = "0.0", Units = "cm/s"))
	float Speed = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover",
		meta = (Units = "cm"))
	float StartDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover")
	bool bReverseDirection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover")
	bool bMovementEnabled = true;

	/** Preserve a manual actor rotation as an offset from the spline heading. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover|Orientation")
	bool bPreservePlacedRotationOffset = true;

	/** Additional heading correction applied after the spline rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover|Orientation")
	FRotator RouteRotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance",
		meta = (ClampMin = "0.016", ClampMax = "0.2", Units = "s"))
	float UpdateInterval = 0.033f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	bool bCastPlaceholderShadow = true;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Mover")
	void SnapToRoute();

	UFUNCTION(BlueprintPure, Category = "Mover")
	float GetDistanceAlongRoute() const { return DistanceAlongRoute; }

protected:
	virtual void BeginPlay() override;
	virtual float GetMovementMultiplier() const;
	void ApplyRouteTransform();
	FRotator GetRouteRotation(float Distance) const;

	float DistanceAlongRoute = 0.0f;
};
