#pragma once

#include "Blueprint/UserWidget.h"
#include "UI/HouseControlMenuTypes.h"
#include "HouseControlMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FHouseMenuCategoryChanged,
	EHouseControlMenuCategory,
	PreviousCategory,
	EHouseControlMenuCategory,
	NewCategory);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHouseMenuDismissRequested);

/**
 * State model for the radial Control Menu.
 *
 * A Blueprint subclass owns presentation and animation. It reports hover,
 * center selection and outside clicks through this class so interaction rules
 * stay consistent between controller rays and direct touch.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class HOUSEOFATONAL_API UHouseControlMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Control Menu")
	void SetHoveredCategory(EHouseControlMenuCategory Category);

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Control Menu")
	void ClearHoveredCategory(EHouseControlMenuCategory Category);

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Control Menu")
	void SelectCategoryCenter(EHouseControlMenuCategory Category);

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Control Menu")
	void HandleOutsideClick();

	UFUNCTION(BlueprintPure, Category = "House of Atonal|Control Menu")
	EHouseControlMenuCategory GetExpandedCategory() const;

	UFUNCTION(BlueprintPure, Category = "House of Atonal|Control Menu")
	EHouseControlMenuCategory GetPinnedCategory() const { return PinnedCategory; }

	UPROPERTY(BlueprintAssignable, Category = "House of Atonal|Control Menu")
	FHouseMenuCategoryChanged OnExpandedCategoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "House of Atonal|Control Menu")
	FHouseMenuDismissRequested OnDismissRequested;

private:
	void BroadcastExpandedCategoryChange(EHouseControlMenuCategory PreviousExpanded);

	UPROPERTY(Transient)
	EHouseControlMenuCategory HoveredCategory = EHouseControlMenuCategory::None;

	UPROPERTY(Transient)
	EHouseControlMenuCategory PinnedCategory = EHouseControlMenuCategory::None;
};
