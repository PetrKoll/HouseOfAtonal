#pragma once

#include "Blueprint/UserWidget.h"
#include "UI/HouseControlMenuTypes.h"
#include "HouseControlMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UWidgetComponent;
class UWidgetInteractionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FHouseMenuCategoryChanged,
	EHouseControlMenuCategory,
	PreviousCategory,
	EHouseControlMenuCategory,
	NewCategory);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHouseMenuDismissRequested);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FHouseMenuOptionSelected,
	EHouseControlMenuCategory,
	Category,
	int32,
	OptionIndex);

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

	void SetPointerInteraction(
		UWidgetInteractionComponent* InPointerInteraction,
		UWidgetComponent* InHostWidgetComponent);

	UPROPERTY(BlueprintAssignable, Category = "House of Atonal|Control Menu")
	FHouseMenuCategoryChanged OnExpandedCategoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "House of Atonal|Control Menu")
	FHouseMenuDismissRequested OnDismissRequested;

	UPROPERTY(BlueprintAssignable, Category = "House of Atonal|Control Menu")
	FHouseMenuOptionSelected OnOptionSelected;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> BTN_Destination;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> BTN_Weather;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> BTN_TimeOfDay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> BTN_HouseState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "House of Atonal|Control Menu|Visuals",
		meta = (ClampMin = "0.1", ClampMax = "3.0"))
	float CollapsedScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "House of Atonal|Control Menu|Visuals",
		meta = (ClampMin = "0.1", ClampMax = "3.0"))
	float ExpandedScale = 1.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "House of Atonal|Control Menu|Visuals",
		meta = (ClampMin = "0.1", ClampMax = "50.0"))
	float ScaleAnimationSpeed = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "House of Atonal|Control Menu|Options")
	TArray<FText> DestinationOptions = {
		NSLOCTEXT("HouseControlMenu", "Exterior", "Exterior"),
		NSLOCTEXT("HouseControlMenu", "Interior", "Interior"),
		NSLOCTEXT("HouseControlMenu", "Atrium", "Atrium")};

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "House of Atonal|Control Menu|Options")
	TArray<FText> WeatherOptions = {
		NSLOCTEXT("HouseControlMenu", "Clear", "Clear"),
		NSLOCTEXT("HouseControlMenu", "Rain", "Rain"),
		NSLOCTEXT("HouseControlMenu", "Snow", "Snow")};

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "House of Atonal|Control Menu|Options")
	TArray<FText> TimeOptions = {
		NSLOCTEXT("HouseControlMenu", "Morning", "Morning"),
		NSLOCTEXT("HouseControlMenu", "GoldenHour", "Golden Hour"),
		NSLOCTEXT("HouseControlMenu", "Night", "Night")};

private:
	void BroadcastExpandedCategoryChange(EHouseControlMenuCategory PreviousExpanded);
	void BindCategoryButtons();
	void UnbindCategoryButtons();
	void CreateOptionStrip();
	void RefreshOptionStrip();
	void SelectOption(int32 OptionIndex);
	void UpdateButtonScale(
		UButton* Button,
		EHouseControlMenuCategory Category,
		float DeltaTime) const;
	void SynchronizeHoveredCategory();
	bool IsButtonHoveredByPointer(const UButton* Button) const;

	UFUNCTION()
	void HandleDestinationHovered();

	UFUNCTION()
	void HandleDestinationUnhovered();

	UFUNCTION()
	void HandleDestinationClicked();

	UFUNCTION()
	void HandleWeatherHovered();

	UFUNCTION()
	void HandleWeatherUnhovered();

	UFUNCTION()
	void HandleWeatherClicked();

	UFUNCTION()
	void HandleTimeOfDayHovered();

	UFUNCTION()
	void HandleTimeOfDayUnhovered();

	UFUNCTION()
	void HandleTimeOfDayClicked();

	UFUNCTION()
	void HandleHouseStateHovered();

	UFUNCTION()
	void HandleHouseStateUnhovered();

	UFUNCTION()
	void HandleHouseStateClicked();

	UFUNCTION()
	void HandleOptionZeroClicked();

	UFUNCTION()
	void HandleOptionOneClicked();

	UFUNCTION()
	void HandleOptionTwoClicked();

	UPROPERTY(Transient)
	EHouseControlMenuCategory HoveredCategory = EHouseControlMenuCategory::None;

	UPROPERTY(Transient)
	EHouseControlMenuCategory PinnedCategory = EHouseControlMenuCategory::None;

	UPROPERTY(Transient)
	TObjectPtr<UWidgetInteractionComponent> PointerInteraction;

	UPROPERTY(Transient)
	TObjectPtr<UWidgetComponent> HostWidgetComponent;

	UPROPERTY(Transient)
	TObjectPtr<UHorizontalBox> OptionStrip;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UButton>> OptionButtons;
};
