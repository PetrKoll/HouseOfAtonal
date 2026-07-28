#pragma once

#include "Blueprint/UserWidget.h"
#include "HouseRadialMenuV1.generated.h"

class UButton;
class UCanvasPanel;
class USizeBox;
class UTextBlock;

/**
 * Minimal radial-menu V1: three main filled circles and three contextual
 * filled circles above the active main circle.
 */
UCLASS(BlueprintType)
class HOUSEOFATONAL_API UHouseRadialMenuV1 : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(
		const FGeometry& MyGeometry,
		float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1")
	FLinearColor FillColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "32.0", ClampMax = "256.0"))
	float MainDiameter = 116.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "16.0", ClampMax = "160.0"))
	float OptionDiameter = 87.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1")
	TArray<FVector2D> MainPositions = {
		FVector2D(0.0f, -72.0f),
		FVector2D(-76.0f, 58.0f),
		FVector2D(76.0f, 58.0f)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1")
	TArray<FVector2D> TopOptionOffsets = {
		FVector2D(-108.0f, -112.0f),
		FVector2D(0.0f, -158.0f),
		FVector2D(108.0f, -112.0f)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1")
	TArray<FVector2D> LeftOptionOffsets = {
		FVector2D(-150.0f, -48.0f),
		FVector2D(-136.0f, 78.0f),
		FVector2D(-38.0f, 154.0f)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1")
	TArray<FVector2D> RightOptionOffsets = {
		FVector2D(150.0f, -48.0f),
		FVector2D(136.0f, 78.0f),
		FVector2D(38.0f, 154.0f)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "1.0", ClampMax = "3.0"))
	float MainHoverScale = 1.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "1.0", ClampMax = "3.0"))
	float OptionHoverScale = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "1.0", ClampMax = "40.0"))
	float AnimationSpeed = 14.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float OptionGroupDelay = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "1.0", ClampMax = "200.0"))
	float OptionSpringStiffness = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "1.0", ClampMax = "40.0"))
	float OptionSpringDamping = 11.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "8", ClampMax = "96"))
	int32 MainFontSize = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1",
		meta = (ClampMin = "8", ClampMax = "96"))
	int32 OptionFontSize = 24;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu V1")
	FLinearColor TextColor = FLinearColor::Black;

private:
	void BuildMenu();
	void CreateCircleButton(
		FName Name,
		const FVector2D& Position,
		float Diameter,
		const FText& Label,
		int32 FontSize,
		UButton*& OutButton,
		USizeBox*& OutVisual,
		UTextBlock*& OutLabel);
	void SetMainHovered(int32 Index);
	void ClearMainHovered(int32 Index);
	void PinMain(int32 Index);
	void SetOptionHovered(int32 Index);
	void ClearOptionHovered(int32 Index);
	void RefreshOptionVisibility();
	void UpdateOptionAnimation(float DeltaTime);
	const TArray<FVector2D>& GetOptionOffsetsForMain(int32 MainIndex) const;

	UFUNCTION() void HoverMain0();
	UFUNCTION() void HoverMain1();
	UFUNCTION() void HoverMain2();
	UFUNCTION() void UnhoverMain0();
	UFUNCTION() void UnhoverMain1();
	UFUNCTION() void UnhoverMain2();
	UFUNCTION() void ClickMain0();
	UFUNCTION() void ClickMain1();
	UFUNCTION() void ClickMain2();
	UFUNCTION() void HoverOption0();
	UFUNCTION() void HoverOption1();
	UFUNCTION() void HoverOption2();
	UFUNCTION() void UnhoverOption0();
	UFUNCTION() void UnhoverOption1();
	UFUNCTION() void UnhoverOption2();

	UPROPERTY(Transient)
	TObjectPtr<UCanvasPanel> RootCanvas;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UButton>> MainButtons;

	UPROPERTY(Transient)
	TArray<TObjectPtr<USizeBox>> MainVisuals;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UButton>> OptionButtons;

	UPROPERTY(Transient)
	TArray<TObjectPtr<USizeBox>> OptionVisuals;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UTextBlock>> OptionLabels;

	int32 HoveredMain = INDEX_NONE;
	int32 PinnedMain = INDEX_NONE;
	int32 HoveredOption = INDEX_NONE;
	int32 RequestedMain = INDEX_NONE;
	TArray<float> GroupAnimationAlpha = {0.0f, 0.0f, 0.0f};
	TArray<float> GroupAnimationVelocity = {0.0f, 0.0f, 0.0f};
	TArray<float> GroupDelayRemaining = {0.0f, 0.0f, 0.0f};
};
