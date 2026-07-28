#include "UI/HouseRadialMenuV1.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Styling/SlateBrush.h"
#include "UI/HouseFilledCircleWidget.h"

void UHouseRadialMenuV1::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BuildMenu();
}

void UHouseRadialMenuV1::NativeTick(
	const FGeometry& MyGeometry,
	float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const int32 ExpandedMain =
		HoveredMain != INDEX_NONE ? HoveredMain : PinnedMain;
	for (int32 Index = 0; Index < MainVisuals.Num(); ++Index)
	{
		const float TargetScale =
			Index == ExpandedMain ? MainHoverScale : 1.0f;
		const float NewScale = FMath::FInterpTo(
			MainVisuals[Index]->GetRenderTransform().Scale.X,
			TargetScale,
			InDeltaTime,
			AnimationSpeed);
		MainVisuals[Index]->SetRenderScale(FVector2D(NewScale));
	}

	HoveredOption = INDEX_NONE;
	for (int32 Index = 0; Index < OptionButtons.Num(); ++Index)
	{
		if (OptionButtons[Index]->GetVisibility() == ESlateVisibility::Visible &&
			OptionButtons[Index]->IsHovered())
		{
			HoveredOption = Index;
			break;
		}
	}

	for (int32 Index = 0; Index < OptionVisuals.Num(); ++Index)
	{
		const float TargetScale =
			Index == HoveredOption ? OptionHoverScale : 1.0f;
		const float NewScale = FMath::FInterpTo(
			OptionVisuals[Index]->GetRenderTransform().Scale.X,
			TargetScale,
			InDeltaTime,
			AnimationSpeed);
		OptionVisuals[Index]->SetRenderScale(FVector2D(NewScale));
	}

	UpdateOptionAnimation(InDeltaTime);
}

void UHouseRadialMenuV1::BuildMenu()
{
	if (!WidgetTree || IsValid(RootCanvas) || MainPositions.Num() < 3)
	{
		return;
	}

	RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(
		UCanvasPanel::StaticClass(), TEXT("RadialMenuV1Root"));
	WidgetTree->RootWidget = RootCanvas;

	MainButtons.SetNum(3);
	MainVisuals.SetNum(3);
	OptionButtons.SetNum(9);
	OptionVisuals.SetNum(9);
	OptionLabels.SetNum(9);

	for (int32 Index = 0; Index < 3; ++Index)
	{
		UButton* MainButton = nullptr;
		USizeBox* MainVisual = nullptr;
		UTextBlock* MainLabel = nullptr;
		CreateCircleButton(
			*FString::Printf(TEXT("MainCircle%d"), Index),
			MainPositions[Index],
			MainDiameter,
			FText::FromString(FString::Printf(TEXT("%02d"), Index + 1)),
			MainFontSize,
			MainButton,
			MainVisual,
			MainLabel);
		MainButtons[Index] = MainButton;
		MainVisuals[Index] = MainVisual;
		if (UCanvasPanelSlot* MainSlot =
			Cast<UCanvasPanelSlot>(MainButton->Slot))
		{
			MainSlot->SetZOrder(10);
		}

	}

	for (int32 GroupIndex = 0; GroupIndex < 3; ++GroupIndex)
	{
		for (int32 OptionIndex = 0; OptionIndex < 3; ++OptionIndex)
		{
			const int32 FlatIndex = GroupIndex * 3 + OptionIndex;
			UButton* OptionButton = nullptr;
			USizeBox* OptionVisual = nullptr;
			UTextBlock* OptionLabel = nullptr;
			CreateCircleButton(
				*FString::Printf(
					TEXT("OptionCircle%d%d"),
					GroupIndex + 1,
					OptionIndex + 1),
				MainPositions[GroupIndex],
				OptionDiameter,
				FText::FromString(FString::Printf(
					TEXT("%d%d"),
					GroupIndex + 1,
					OptionIndex + 1)),
				OptionFontSize,
				OptionButton,
				OptionVisual,
				OptionLabel);
			OptionButtons[FlatIndex] = OptionButton;
			OptionVisuals[FlatIndex] = OptionVisual;
			OptionLabels[FlatIndex] = OptionLabel;
			if (UCanvasPanelSlot* OptionSlot =
				Cast<UCanvasPanelSlot>(OptionButton->Slot))
			{
				OptionSlot->SetZOrder(0);
			}
			OptionButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	MainButtons[0]->OnHovered.AddUniqueDynamic(this, &UHouseRadialMenuV1::HoverMain0);
	MainButtons[1]->OnHovered.AddUniqueDynamic(this, &UHouseRadialMenuV1::HoverMain1);
	MainButtons[2]->OnHovered.AddUniqueDynamic(this, &UHouseRadialMenuV1::HoverMain2);
	MainButtons[0]->OnUnhovered.AddUniqueDynamic(this, &UHouseRadialMenuV1::UnhoverMain0);
	MainButtons[1]->OnUnhovered.AddUniqueDynamic(this, &UHouseRadialMenuV1::UnhoverMain1);
	MainButtons[2]->OnUnhovered.AddUniqueDynamic(this, &UHouseRadialMenuV1::UnhoverMain2);
	MainButtons[0]->OnClicked.AddUniqueDynamic(this, &UHouseRadialMenuV1::ClickMain0);
	MainButtons[1]->OnClicked.AddUniqueDynamic(this, &UHouseRadialMenuV1::ClickMain1);
	MainButtons[2]->OnClicked.AddUniqueDynamic(this, &UHouseRadialMenuV1::ClickMain2);

}

void UHouseRadialMenuV1::CreateCircleButton(
	FName Name,
	const FVector2D& Position,
	float Diameter,
	const FText& Label,
	int32 FontSize,
	UButton*& OutButton,
	USizeBox*& OutVisual,
	UTextBlock*& OutLabel)
{
	OutButton = WidgetTree->ConstructWidget<UButton>(
		UButton::StaticClass(), Name);
	FButtonStyle ButtonStyle = OutButton->GetStyle();
	FSlateBrush InvisibleBrush;
	InvisibleBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
	ButtonStyle.SetNormal(InvisibleBrush);
	ButtonStyle.SetHovered(InvisibleBrush);
	ButtonStyle.SetPressed(InvisibleBrush);
	OutButton->SetStyle(ButtonStyle);

	OutVisual = WidgetTree->ConstructWidget<USizeBox>();
	OutVisual->SetWidthOverride(Diameter);
	OutVisual->SetHeightOverride(Diameter);
	OutVisual->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));

	UHouseFilledCircleWidget* Circle =
		WidgetTree->ConstructWidget<UHouseFilledCircleWidget>();
	Circle->SetFillColor(FillColor);
	Circle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UOverlay* VisualStack = WidgetTree->ConstructWidget<UOverlay>();
	UOverlaySlot* CircleSlot = VisualStack->AddChildToOverlay(Circle);
	CircleSlot->SetHorizontalAlignment(HAlign_Fill);
	CircleSlot->SetVerticalAlignment(VAlign_Fill);

	OutLabel = WidgetTree->ConstructWidget<UTextBlock>();
	FSlateFontInfo Font = OutLabel->GetFont();
	Font.Size = FontSize;
	OutLabel->SetFont(Font);
	OutLabel->SetText(Label);
	OutLabel->SetColorAndOpacity(FSlateColor(TextColor));
	OutLabel->SetJustification(ETextJustify::Center);
	OutLabel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UOverlaySlot* LabelSlot = VisualStack->AddChildToOverlay(OutLabel);
	LabelSlot->SetHorizontalAlignment(HAlign_Center);
	LabelSlot->SetVerticalAlignment(VAlign_Center);

	OutVisual->AddChild(VisualStack);
	OutButton->AddChild(OutVisual);
	if (UButtonSlot* ContentSlot = Cast<UButtonSlot>(OutVisual->Slot))
	{
		ContentSlot->SetPadding(FMargin(0.0f));
		ContentSlot->SetHorizontalAlignment(HAlign_Center);
		ContentSlot->SetVerticalAlignment(VAlign_Center);
	}

	UCanvasPanelSlot* CanvasSlot = RootCanvas->AddChildToCanvas(OutButton);
	CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	CanvasSlot->SetPosition(Position);
	CanvasSlot->SetSize(FVector2D(Diameter));
	CanvasSlot->SetAutoSize(false);
}

void UHouseRadialMenuV1::SetMainHovered(int32 Index)
{
	HoveredMain = Index;
	RefreshOptionVisibility();
}

void UHouseRadialMenuV1::ClearMainHovered(int32 Index)
{
	if (HoveredMain == Index)
	{
		HoveredMain = INDEX_NONE;
		RefreshOptionVisibility();
	}
}

void UHouseRadialMenuV1::PinMain(int32 Index)
{
	PinnedMain = Index;
	RefreshOptionVisibility();
}

void UHouseRadialMenuV1::SetOptionHovered(int32 Index)
{
	HoveredOption = Index;
}

void UHouseRadialMenuV1::ClearOptionHovered(int32 Index)
{
	if (HoveredOption == Index)
	{
		HoveredOption = INDEX_NONE;
	}
}

void UHouseRadialMenuV1::RefreshOptionVisibility()
{
	const int32 ExpandedMain =
		HoveredMain != INDEX_NONE ? HoveredMain : PinnedMain;
	if (RequestedMain == ExpandedMain)
	{
		return;
	}

	RequestedMain = ExpandedMain;
	HoveredOption = INDEX_NONE;
	if (RequestedMain != INDEX_NONE &&
		MainPositions.IsValidIndex(RequestedMain))
	{
		GroupDelayRemaining[RequestedMain] = OptionGroupDelay;
		for (int32 OptionIndex = 0; OptionIndex < 3; ++OptionIndex)
		{
			const int32 FlatIndex = RequestedMain * 3 + OptionIndex;
			if (UCanvasPanelSlot* CanvasSlot =
				Cast<UCanvasPanelSlot>(OptionButtons[FlatIndex]->Slot))
			{
				CanvasSlot->SetPosition(MainPositions[RequestedMain]);
			}
			OptionButtons[FlatIndex]->SetVisibility(
				ESlateVisibility::HitTestInvisible);
		}
	}
}

const TArray<FVector2D>& UHouseRadialMenuV1::GetOptionOffsetsForMain(
	int32 MainIndex) const
{
	if (MainIndex == 1)
	{
		return LeftOptionOffsets;
	}
	if (MainIndex == 2)
	{
		return RightOptionOffsets;
	}
	return TopOptionOffsets;
}

void UHouseRadialMenuV1::UpdateOptionAnimation(float DeltaTime)
{
	for (int32 GroupIndex = 0; GroupIndex < 3; ++GroupIndex)
	{
		const bool bIsRequested = GroupIndex == RequestedMain;
		if (bIsRequested && GroupDelayRemaining[GroupIndex] > 0.0f)
		{
			GroupDelayRemaining[GroupIndex] = FMath::Max(
				0.0f,
				GroupDelayRemaining[GroupIndex] - DeltaTime);
		}

		const float TargetAlpha =
			bIsRequested &&
			GroupDelayRemaining[GroupIndex] <= 0.0f
				? 1.0f
				: 0.0f;
		float& Alpha = GroupAnimationAlpha[GroupIndex];
		float& Velocity = GroupAnimationVelocity[GroupIndex];

		const float Acceleration =
			(TargetAlpha - Alpha) * OptionSpringStiffness;
		Velocity += Acceleration * DeltaTime;
		Velocity *= FMath::Exp(-OptionSpringDamping * DeltaTime);
		Alpha += Velocity * DeltaTime;
		Alpha = FMath::Clamp(Alpha, -0.04f, 1.08f);

		const float NormalizedAlpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
		const float SmoothAlpha =
			NormalizedAlpha * NormalizedAlpha * NormalizedAlpha *
			(NormalizedAlpha *
				(NormalizedAlpha * 6.0f - 15.0f) +
				10.0f);
		const float Overshoot = Alpha - NormalizedAlpha;
		const float TravelAlpha = SmoothAlpha + Overshoot * 0.35f;
		const TArray<FVector2D>& Offsets =
			GetOptionOffsetsForMain(GroupIndex);

		for (int32 OptionIndex = 0; OptionIndex < 3; ++OptionIndex)
		{
			const int32 FlatIndex = GroupIndex * 3 + OptionIndex;
			UButton* Button = OptionButtons[FlatIndex];
			const FVector2D Offset = Offsets.IsValidIndex(OptionIndex)
				? Offsets[OptionIndex]
				: FVector2D::ZeroVector;
			if (UCanvasPanelSlot* CanvasSlot =
				Cast<UCanvasPanelSlot>(Button->Slot))
			{
				CanvasSlot->SetPosition(
					MainPositions[GroupIndex] +
					Offset * TravelAlpha);
			}

			if (NormalizedAlpha > 0.01f || bIsRequested)
			{
				Button->SetVisibility(
					bIsRequested && NormalizedAlpha > 0.72f
						? ESlateVisibility::Visible
						: ESlateVisibility::HitTestInvisible);
			}
			else
			{
				Button->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UHouseRadialMenuV1::HoverMain0() { SetMainHovered(0); }
void UHouseRadialMenuV1::HoverMain1() { SetMainHovered(1); }
void UHouseRadialMenuV1::HoverMain2() { SetMainHovered(2); }
void UHouseRadialMenuV1::UnhoverMain0() { ClearMainHovered(0); }
void UHouseRadialMenuV1::UnhoverMain1() { ClearMainHovered(1); }
void UHouseRadialMenuV1::UnhoverMain2() { ClearMainHovered(2); }
void UHouseRadialMenuV1::ClickMain0() { PinMain(0); }
void UHouseRadialMenuV1::ClickMain1() { PinMain(1); }
void UHouseRadialMenuV1::ClickMain2() { PinMain(2); }
void UHouseRadialMenuV1::HoverOption0() { SetOptionHovered(0); }
void UHouseRadialMenuV1::HoverOption1() { SetOptionHovered(1); }
void UHouseRadialMenuV1::HoverOption2() { SetOptionHovered(2); }
void UHouseRadialMenuV1::UnhoverOption0() { ClearOptionHovered(0); }
void UHouseRadialMenuV1::UnhoverOption1() { ClearOptionHovered(1); }
void UHouseRadialMenuV1::UnhoverOption2() { ClearOptionHovered(2); }
