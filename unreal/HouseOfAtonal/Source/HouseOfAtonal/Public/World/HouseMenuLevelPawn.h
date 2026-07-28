#pragma once

#include "GameFramework/Pawn.h"
#include "HouseMenuLevelPawn.generated.h"

class UCameraComponent;
class UHouseControlMenuComponent;
class UInputAction;
class UInputMappingContext;
class UMotionControllerComponent;
class UFloatingPawnMovement;
class USceneComponent;
class UWidgetInteractionComponent;
struct FInputActionValue;

/**
 * Stationary XR viewpoint for the tabletop Menu Level.
 *
 * The pawn intentionally binds no locomotion or teleport input. OpenXR still
 * drives the camera inside the tracking origin, giving the diorama real
 * head-motion parallax.
 */
UCLASS()
class HOUSEOFATONAL_API AHouseMenuLevelPawn : public APawn
{
	GENERATED_BODY()

public:
	AHouseMenuLevelPawn();
	virtual void SetupPlayerInputComponent(
		UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level")
	TObjectPtr<USceneComponent> TrackingOrigin;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level")
	TObjectPtr<UCameraComponent> HeadCamera;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Control Menu")
	TObjectPtr<UMotionControllerComponent> RightControllerAim;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Control Menu")
	TObjectPtr<UWidgetInteractionComponent> WidgetInteractionRight;

	UPROPERTY(VisibleAnywhere, Category = "Menu Level|Control Menu")
	TObjectPtr<UHouseControlMenuComponent> ControlMenu;

	UPROPERTY(VisibleAnywhere, Category = "View Level|Movement")
	TObjectPtr<UFloatingPawnMovement> ViewMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Menu Level|Input")
	TObjectPtr<UInputMappingContext> DefaultInputContext;

	UPROPERTY(EditDefaultsOnly, Category = "Menu Level|Input")
	TObjectPtr<UInputAction> ControlMenuAction;

	UPROPERTY(EditDefaultsOnly, Category = "View Level|Input")
	TObjectPtr<UInputAction> ViewMoveAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputMappingContext> ViewInputContext;

private:
	void HandleControlMenuInput(const FInputActionValue& Value);
	void HandleViewMoveInput(const FInputActionValue& Value);
	bool bIsViewLevel = false;
};
