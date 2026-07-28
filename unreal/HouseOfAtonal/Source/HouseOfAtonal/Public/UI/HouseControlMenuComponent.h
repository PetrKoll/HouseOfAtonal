#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "HouseControlMenuComponent.generated.h"

class AHouseControlMenuActor;
class USceneComponent;
class UWidgetInteractionComponent;

/**
 * Add this component to the XR pawn and call ToggleControlMenu from the right
 * controller trigger input.
 */
UCLASS(ClassGroup = (HouseOfAtonal), BlueprintType, Blueprintable,
	meta = (BlueprintSpawnableComponent))
class HOUSEOFATONAL_API UHouseControlMenuComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHouseControlMenuComponent();

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Control Menu")
	void ToggleControlMenu(
		USceneComponent* FollowTarget,
		FVector PointerOrigin,
		FVector PointerDirection);

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Control Menu")
	void ShowControlMenu(
		USceneComponent* FollowTarget,
		FVector PointerOrigin,
		FVector PointerDirection);

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Control Menu")
	void HideControlMenu();

	UFUNCTION(BlueprintPure, Category = "House of Atonal|Control Menu")
	bool IsControlMenuVisible() const;

	UFUNCTION(BlueprintPure, Category = "House of Atonal|Control Menu")
	AHouseControlMenuActor* GetActiveControlMenu() const { return ActiveControlMenu; }

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UWidgetInteractionComponent* FindRightWidgetInteraction() const;

	UPROPERTY(EditAnywhere, Category = "House of Atonal|Control Menu")
	TSubclassOf<AHouseControlMenuActor> MenuActorClass;

	UPROPERTY(EditAnywhere, Category = "House of Atonal|Control Menu")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "House of Atonal|Control Menu",
		meta = (ClampMin = "25.0", ClampMax = "500.0", Units = "cm"))
	float SpawnDistance = 125.0f;

	UPROPERTY(EditAnywhere, Category = "House of Atonal|Control Menu",
		meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float MenuWorldScaleMultiplier = 2.0f;

	UPROPERTY(Transient)
	TObjectPtr<AHouseControlMenuActor> ActiveControlMenu;
};
