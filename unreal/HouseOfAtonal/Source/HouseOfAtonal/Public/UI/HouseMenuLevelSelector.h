#pragma once

#include "Blueprint/UserWidget.h"
#include "Experience/HouseExperienceTypes.h"
#include "HouseMenuLevelSelector.generated.h"

class UButton;
class UCanvasPanel;
class USizeBox;

/** Three-destination selector used only in the initial Menu Level. */
UCLASS(BlueprintType)
class HOUSEOFATONAL_API UHouseMenuLevelSelector : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(
		const FGeometry& MyGeometry,
		float InDeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Menu Level Selector")
	float CircleDiameter = 116.0f;

	UPROPERTY(EditAnywhere, Category = "Menu Level Selector")
	float HoverScale = 1.3f;

	UPROPERTY(EditAnywhere, Category = "Menu Level Selector")
	float AnimationSpeed = 14.0f;

	UPROPERTY(EditAnywhere, Category = "Menu Level Selector")
	FLinearColor FillColor = FLinearColor::White;

private:
	void BuildSelector();
	void CreateDestination(
		int32 Index,
		const FVector2D& Position,
		const FText& Label);
	void TravelTo(EHouseLocation Location);

	UFUNCTION() void SelectAtrium();
	UFUNCTION() void SelectRoomOne();
	UFUNCTION() void SelectRoomTwo();

	UPROPERTY(Transient)
	TObjectPtr<UCanvasPanel> RootCanvas;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UButton>> Buttons;

	UPROPERTY(Transient)
	TArray<TObjectPtr<USizeBox>> Visuals;
};
