#include "UI/HouseMenuLevelSelector.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Experience/HouseExperienceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Styling/SlateBrush.h"
#include "UI/HouseFilledCircleWidget.h"

void UHouseMenuLevelSelector::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BuildSelector();
}

void UHouseMenuLevelSelector::NativeTick(
	const FGeometry& MyGeometry,
	const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	for (int32 Index = 0; Index < Buttons.Num(); ++Index)
	{
		const float Target = Buttons[Index]->IsHovered() ? HoverScale : 1.0f;
		const float Current = Visuals[Index]->GetRenderTransform().Scale.X;
		Visuals[Index]->SetRenderScale(FVector2D(FMath::FInterpTo(
			Current, Target, InDeltaTime, AnimationSpeed)));
	}
}

void UHouseMenuLevelSelector::BuildSelector()
{
	RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>();
	WidgetTree->RootWidget = RootCanvas;
	Buttons.SetNum(3);
	Visuals.SetNum(3);
	CreateDestination(0, FVector2D(0.0f, -72.0f), FText::FromString(TEXT("A")));
	CreateDestination(1, FVector2D(-76.0f, 58.0f), FText::FromString(TEXT("1")));
	CreateDestination(2, FVector2D(76.0f, 58.0f), FText::FromString(TEXT("2")));
	Buttons[0]->OnClicked.AddUniqueDynamic(
		this, &UHouseMenuLevelSelector::SelectAtrium);
	Buttons[1]->OnClicked.AddUniqueDynamic(
		this, &UHouseMenuLevelSelector::SelectRoomOne);
	Buttons[2]->OnClicked.AddUniqueDynamic(
		this, &UHouseMenuLevelSelector::SelectRoomTwo);
}

void UHouseMenuLevelSelector::CreateDestination(
	const int32 Index,
	const FVector2D& Position,
	const FText& Label)
{
	UButton* Button = WidgetTree->ConstructWidget<UButton>();
	FButtonStyle Style = Button->GetStyle();
	FSlateBrush Invisible;
	Invisible.DrawAs = ESlateBrushDrawType::NoDrawType;
	Style.SetNormal(Invisible);
	Style.SetHovered(Invisible);
	Style.SetPressed(Invisible);
	Button->SetStyle(Style);

	USizeBox* Visual = WidgetTree->ConstructWidget<USizeBox>();
	Visual->SetWidthOverride(CircleDiameter);
	Visual->SetHeightOverride(CircleDiameter);
	Visual->SetRenderTransformPivot(FVector2D(0.5f));

	UOverlay* Overlay = WidgetTree->ConstructWidget<UOverlay>();
	UHouseFilledCircleWidget* Circle =
		WidgetTree->ConstructWidget<UHouseFilledCircleWidget>();
	Circle->SetFillColor(FillColor);
	Circle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UOverlaySlot* CircleSlot = Overlay->AddChildToOverlay(Circle);
	CircleSlot->SetHorizontalAlignment(HAlign_Fill);
	CircleSlot->SetVerticalAlignment(VAlign_Fill);

	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>();
	Text->SetText(Label);
	Text->SetJustification(ETextJustify::Center);
	Text->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
	Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FSlateFontInfo Font = Text->GetFont();
	Font.Size = 30;
	Text->SetFont(Font);
	UOverlaySlot* TextSlot = Overlay->AddChildToOverlay(Text);
	TextSlot->SetHorizontalAlignment(HAlign_Center);
	TextSlot->SetVerticalAlignment(VAlign_Center);

	Visual->AddChild(Overlay);
	Button->AddChild(Visual);
	if (UButtonSlot* ContentSlot = Cast<UButtonSlot>(Visual->Slot))
	{
		ContentSlot->SetHorizontalAlignment(HAlign_Center);
		ContentSlot->SetVerticalAlignment(VAlign_Center);
	}
	UCanvasPanelSlot* CanvasSlot = RootCanvas->AddChildToCanvas(Button);
	CanvasSlot->SetAnchors(FAnchors(0.5f));
	CanvasSlot->SetAlignment(FVector2D(0.5f));
	CanvasSlot->SetPosition(Position);
	CanvasSlot->SetSize(FVector2D(CircleDiameter));

	Buttons[Index] = Button;
	Visuals[Index] = Visual;
}

void UHouseMenuLevelSelector::TravelTo(const EHouseLocation Location)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UHouseExperienceSubsystem* Experience =
			GameInstance->GetSubsystem<UHouseExperienceSubsystem>())
		{
			Experience->SetLocation(Location);
		}
	}
	UGameplayStatics::OpenLevel(this, FName(TEXT("L_ViewLevel")));
}

void UHouseMenuLevelSelector::SelectAtrium()
{
	TravelTo(EHouseLocation::Atrium);
}

void UHouseMenuLevelSelector::SelectRoomOne()
{
	TravelTo(EHouseLocation::RoomOne);
}

void UHouseMenuLevelSelector::SelectRoomTwo()
{
	TravelTo(EHouseLocation::RoomTwo);
}
