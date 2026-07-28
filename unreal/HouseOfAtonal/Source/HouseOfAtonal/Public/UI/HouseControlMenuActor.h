#pragma once

#include "GameFramework/Actor.h"
#include "HouseControlMenuActor.generated.h"

class USceneComponent;
class UUserWidget;
class UWidgetComponent;

/**
 * World-space host for the Control Menu.
 *
 * The actor records its summon offset in the user's horizontal reference frame
 * and follows that target with damped positional and rotational lag.
 */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseControlMenuActor : public AActor
{
	GENERATED_BODY()

public:
	AHouseControlMenuActor();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Control Menu")
	void InitializeFollow(USceneComponent* InFollowTarget);

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Control Menu")
	void SetMenuWidgetClass(TSubclassOf<UUserWidget> InWidgetClass);

	UFUNCTION(BlueprintPure, Category = "House of Atonal|Control Menu")
	UWidgetComponent* GetMenuWidgetComponent() const { return MenuWidgetComponent; }

protected:
	virtual void BeginPlay() override;

private:
	FTransform BuildHorizontalFollowFrame() const;
	void CaptureRelativeOffset();

	UPROPERTY(VisibleAnywhere, Category = "House of Atonal|Control Menu")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "House of Atonal|Control Menu")
	TObjectPtr<UWidgetComponent> MenuWidgetComponent;

	UPROPERTY(Transient)
	TObjectPtr<USceneComponent> FollowTarget;

	UPROPERTY(EditAnywhere, Category = "House of Atonal|Control Menu|Follow",
		meta = (ClampMin = "0.1"))
	float PositionFollowSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "House of Atonal|Control Menu|Follow",
		meta = (ClampMin = "0.1"))
	float VerticalFollowSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "House of Atonal|Control Menu|Follow",
		meta = (ClampMin = "0.1"))
	float RotationFollowSpeed = 7.0f;

	UPROPERTY(EditAnywhere, Category = "House of Atonal|Control Menu|Follow",
		meta = (ClampMin = "1.0", Units = "cm"))
	float MaximumComfortSeparation = 125.0f;

	UPROPERTY(EditAnywhere, Category = "House of Atonal|Control Menu|Follow",
		meta = (ClampMin = "1.0"))
	float CatchUpSpeedMultiplier = 3.0f;

	FVector RelativeOffset = FVector::ZeroVector;
	bool bFollowInitialized = false;
};
