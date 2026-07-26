#include "UI/HouseControlMenuWidget.h"

#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/PanelWidget.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetInteractionComponent.h"
#include "Engine/GameInstance.h"
#include "Experience/HouseExperienceSubsystem.h"
#include "Layout/WidgetPath.h"
#include "Blueprint/WidgetTree.h"

void UHouseControlMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindCategoryButtons();
	CreateOptionStrip();

	const FVector2D InitialScale(CollapsedScale, CollapsedScale);
	for (UButton* Button : {BTN_Destination, BTN_Weather, BTN_TimeOfDay, BTN_HouseState})
	{
		if (IsValid(Button))
		{
			Button->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
			Button->SetRenderScale(InitialScale);
		}
	}

	if (IsValid(BTN_HouseState))
	{
		BTN_HouseState->SetVisibility(ESlateVisibility::Collapsed);
	}

	RefreshOptionStrip();
}

void UHouseControlMenuWidget::NativeDestruct()
{
	UnbindCategoryButtons();
	Super::NativeDestruct();
}

void UHouseControlMenuWidget::NativeTick(
	const FGeometry& MyGeometry,
	float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SynchronizeHoveredCategory();
	UpdateButtonScale(BTN_Destination, EHouseControlMenuCategory::Destination, InDeltaTime);
	UpdateButtonScale(BTN_Weather, EHouseControlMenuCategory::Weather, InDeltaTime);
	UpdateButtonScale(BTN_TimeOfDay, EHouseControlMenuCategory::TimeOfDay, InDeltaTime);
	UpdateButtonScale(BTN_HouseState, EHouseControlMenuCategory::HouseState, InDeltaTime);
}

void UHouseControlMenuWidget::SetHoveredCategory(EHouseControlMenuCategory Category)
{
	// When a world-space pointer is assigned, NativeTick is the single
	// authoritative hover source. This prevents Blueprint hover delegates from
	// racing the right-controller Slate hit path.
	if (IsValid(PointerInteraction))
	{
		return;
	}

	const EHouseControlMenuCategory PreviousExpanded = GetExpandedCategory();
	HoveredCategory = Category;
	BroadcastExpandedCategoryChange(PreviousExpanded);
}

void UHouseControlMenuWidget::ClearHoveredCategory(EHouseControlMenuCategory Category)
{
	if (IsValid(PointerInteraction))
	{
		return;
	}

	if (HoveredCategory != Category)
	{
		return;
	}

	const EHouseControlMenuCategory PreviousExpanded = GetExpandedCategory();
	HoveredCategory = EHouseControlMenuCategory::None;
	BroadcastExpandedCategoryChange(PreviousExpanded);
}

void UHouseControlMenuWidget::SelectCategoryCenter(EHouseControlMenuCategory Category)
{
	const EHouseControlMenuCategory PreviousExpanded = GetExpandedCategory();
	PinnedCategory = Category;
	HoveredCategory = Category;
	BroadcastExpandedCategoryChange(PreviousExpanded);
}

void UHouseControlMenuWidget::HandleOutsideClick()
{
	const EHouseControlMenuCategory PreviousExpanded = GetExpandedCategory();
	HoveredCategory = EHouseControlMenuCategory::None;
	PinnedCategory = EHouseControlMenuCategory::None;
	BroadcastExpandedCategoryChange(PreviousExpanded);
	OnDismissRequested.Broadcast();
}

void UHouseControlMenuWidget::SetPointerInteraction(
	UWidgetInteractionComponent* InPointerInteraction,
	UWidgetComponent* InHostWidgetComponent)
{
	PointerInteraction = InPointerInteraction;
	HostWidgetComponent = InHostWidgetComponent;
}

EHouseControlMenuCategory UHouseControlMenuWidget::GetExpandedCategory() const
{
	return HoveredCategory != EHouseControlMenuCategory::None
		? HoveredCategory
		: PinnedCategory;
}

void UHouseControlMenuWidget::BroadcastExpandedCategoryChange(
	EHouseControlMenuCategory PreviousExpanded)
{
	const EHouseControlMenuCategory NewExpanded = GetExpandedCategory();
	if (PreviousExpanded != NewExpanded)
	{
		OnExpandedCategoryChanged.Broadcast(PreviousExpanded, NewExpanded);
		RefreshOptionStrip();
	}
}

void UHouseControlMenuWidget::CreateOptionStrip()
{
	if (!WidgetTree || IsValid(OptionStrip))
	{
		return;
	}

	UPanelWidget* RootPanel = Cast<UPanelWidget>(WidgetTree->RootWidget);
	if (!IsValid(RootPanel))
	{
		return;
	}

	OptionStrip = WidgetTree->ConstructWidget<UHorizontalBox>(
		UHorizontalBox::StaticClass(),
		TEXT("GeneratedOptionStrip"));
	RootPanel->AddChild(OptionStrip);

	if (UCanvasPanelSlot* CanvasSlot =
		Cast<UCanvasPanelSlot>(OptionStrip->Slot))
	{
		CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		CanvasSlot->SetPosition(FVector2D(0.0f, 145.0f));
		CanvasSlot->SetAutoSize(true);
	}

	OptionButtons.Reset();
	for (int32 Index = 0; Index < 3; ++Index)
	{
		USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>();
		SizeBox->SetWidthOverride(130.0f);
		SizeBox->SetHeightOverride(56.0f);

		UButton* Button = WidgetTree->ConstructWidget<UButton>();
		UTextBlock* Label = WidgetTree->ConstructWidget<UTextBlock>();
		Label->SetJustification(ETextJustify::Center);
		Button->AddChild(Label);
		SizeBox->AddChild(Button);

		UHorizontalBoxSlot* StripSlot = OptionStrip->AddChildToHorizontalBox(SizeBox);
		StripSlot->SetPadding(FMargin(8.0f, 0.0f));
		StripSlot->SetVerticalAlignment(VAlign_Center);

		OptionButtons.Add(Button);
	}

	OptionButtons[0]->OnClicked.AddUniqueDynamic(
		this, &UHouseControlMenuWidget::HandleOptionZeroClicked);
	OptionButtons[1]->OnClicked.AddUniqueDynamic(
		this, &UHouseControlMenuWidget::HandleOptionOneClicked);
	OptionButtons[2]->OnClicked.AddUniqueDynamic(
		this, &UHouseControlMenuWidget::HandleOptionTwoClicked);
}

void UHouseControlMenuWidget::RefreshOptionStrip()
{
	if (!IsValid(OptionStrip))
	{
		return;
	}

	const TArray<FText>* Options = nullptr;
	switch (GetExpandedCategory())
	{
	case EHouseControlMenuCategory::Destination:
		Options = &DestinationOptions;
		break;
	case EHouseControlMenuCategory::Weather:
		Options = &WeatherOptions;
		break;
	case EHouseControlMenuCategory::TimeOfDay:
		Options = &TimeOptions;
		break;
	default:
		OptionStrip->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	OptionStrip->SetVisibility(ESlateVisibility::Visible);
	for (int32 Index = 0; Index < OptionButtons.Num(); ++Index)
	{
		UButton* Button = OptionButtons[Index];
		const bool bHasOption = Options->IsValidIndex(Index);
		Button->SetVisibility(
			bHasOption ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		if (bHasOption)
		{
			if (UTextBlock* Label = Cast<UTextBlock>(Button->GetChildAt(0)))
			{
				Label->SetText((*Options)[Index]);
			}
		}
	}
}

void UHouseControlMenuWidget::SelectOption(int32 OptionIndex)
{
	const EHouseControlMenuCategory Category = GetExpandedCategory();
	if (Category == EHouseControlMenuCategory::None ||
		Category == EHouseControlMenuCategory::HouseState)
	{
		return;
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UHouseExperienceSubsystem* Experience =
			GameInstance->GetSubsystem<UHouseExperienceSubsystem>())
		{
			switch (Category)
			{
			case EHouseControlMenuCategory::Destination:
			{
				static constexpr EHouseLocation Values[] = {
					EHouseLocation::Exterior,
					EHouseLocation::Interior,
					EHouseLocation::Atrium};
				if (OptionIndex >= 0 && OptionIndex < UE_ARRAY_COUNT(Values))
				{
					Experience->SetLocation(Values[OptionIndex]);
				}
				break;
			}
			case EHouseControlMenuCategory::Weather:
			{
				static constexpr EHouseWeatherPreset Values[] = {
					EHouseWeatherPreset::Clear,
					EHouseWeatherPreset::Rain,
					EHouseWeatherPreset::Snow};
				if (OptionIndex >= 0 && OptionIndex < UE_ARRAY_COUNT(Values))
				{
					Experience->SetWeather(Values[OptionIndex]);
				}
				break;
			}
			case EHouseControlMenuCategory::TimeOfDay:
			{
				static constexpr EHouseTimePreset Values[] = {
					EHouseTimePreset::Morning,
					EHouseTimePreset::GoldenHour,
					EHouseTimePreset::Night};
				if (OptionIndex >= 0 && OptionIndex < UE_ARRAY_COUNT(Values))
				{
					Experience->SetTimeOfDay(Values[OptionIndex]);
				}
				break;
			}
			default:
				break;
			}
		}
	}

	OnOptionSelected.Broadcast(Category, OptionIndex);
}

void UHouseControlMenuWidget::BindCategoryButtons()
{
	if (IsValid(BTN_Destination))
	{
		BTN_Destination->OnClicked.AddUniqueDynamic(
			this, &UHouseControlMenuWidget::HandleDestinationClicked);
	}

	if (IsValid(BTN_Weather))
	{
		BTN_Weather->OnClicked.AddUniqueDynamic(
			this, &UHouseControlMenuWidget::HandleWeatherClicked);
	}

	if (IsValid(BTN_TimeOfDay))
	{
		BTN_TimeOfDay->OnClicked.AddUniqueDynamic(
			this, &UHouseControlMenuWidget::HandleTimeOfDayClicked);
	}

	if (IsValid(BTN_HouseState))
	{
		BTN_HouseState->OnClicked.AddUniqueDynamic(
			this, &UHouseControlMenuWidget::HandleHouseStateClicked);
	}
}

void UHouseControlMenuWidget::UnbindCategoryButtons()
{
	if (IsValid(BTN_Destination))
	{
		BTN_Destination->OnClicked.RemoveDynamic(
			this, &UHouseControlMenuWidget::HandleDestinationClicked);
	}

	if (IsValid(BTN_Weather))
	{
		BTN_Weather->OnClicked.RemoveDynamic(
			this, &UHouseControlMenuWidget::HandleWeatherClicked);
	}

	if (IsValid(BTN_TimeOfDay))
	{
		BTN_TimeOfDay->OnClicked.RemoveDynamic(
			this, &UHouseControlMenuWidget::HandleTimeOfDayClicked);
	}

	if (IsValid(BTN_HouseState))
	{
		BTN_HouseState->OnClicked.RemoveDynamic(
			this, &UHouseControlMenuWidget::HandleHouseStateClicked);
	}
}

void UHouseControlMenuWidget::UpdateButtonScale(
	UButton* Button,
	EHouseControlMenuCategory Category,
	float DeltaTime) const
{
	if (!IsValid(Button))
	{
		return;
	}

	const float TargetScale = GetExpandedCategory() == Category
		? ExpandedScale
		: CollapsedScale;
	const float CurrentScale = Button->GetRenderTransform().Scale.X;
	const float NewScale = FMath::FInterpTo(
		CurrentScale,
		TargetScale,
		DeltaTime,
		ScaleAnimationSpeed);
	Button->SetRenderScale(FVector2D(NewScale, NewScale));
}

void UHouseControlMenuWidget::SynchronizeHoveredCategory()
{
	EHouseControlMenuCategory ActualHoveredCategory =
		EHouseControlMenuCategory::None;

	if (IsButtonHoveredByPointer(BTN_Destination))
	{
		ActualHoveredCategory = EHouseControlMenuCategory::Destination;
	}
	else if (IsButtonHoveredByPointer(BTN_Weather))
	{
		ActualHoveredCategory = EHouseControlMenuCategory::Weather;
	}
	else if (IsButtonHoveredByPointer(BTN_TimeOfDay))
	{
		ActualHoveredCategory = EHouseControlMenuCategory::TimeOfDay;
	}
	else if (IsButtonHoveredByPointer(BTN_HouseState))
	{
		ActualHoveredCategory = EHouseControlMenuCategory::HouseState;
	}

	if (HoveredCategory == ActualHoveredCategory)
	{
		return;
	}

	const EHouseControlMenuCategory PreviousExpanded = GetExpandedCategory();
	HoveredCategory = ActualHoveredCategory;
	BroadcastExpandedCategoryChange(PreviousExpanded);
}

bool UHouseControlMenuWidget::IsButtonHoveredByPointer(
	const UButton* Button) const
{
	if (!IsValid(Button) || !IsValid(PointerInteraction))
	{
		return false;
	}

	const FHitResult& PointerHit = PointerInteraction->GetLastHitResult();
	if (!PointerHit.bBlockingHit ||
		PointerInteraction->GetHoveredWidgetComponent() != HostWidgetComponent)
	{
		return false;
	}

	const FWeakWidgetPath& HoveredPath =
		PointerInteraction->GetHoveredWidgetPath();
	if (!HoveredPath.IsValid())
	{
		return false;
	}

	const TSharedPtr<SWidget> CachedButtonWidget = Button->GetCachedWidget();
	return CachedButtonWidget.IsValid() &&
		HoveredPath.ContainsWidget(CachedButtonWidget.Get());
}

void UHouseControlMenuWidget::HandleDestinationHovered()
{
	SetHoveredCategory(EHouseControlMenuCategory::Destination);
}

void UHouseControlMenuWidget::HandleDestinationUnhovered()
{
	ClearHoveredCategory(EHouseControlMenuCategory::Destination);
}

void UHouseControlMenuWidget::HandleDestinationClicked()
{
	SelectCategoryCenter(EHouseControlMenuCategory::Destination);
}

void UHouseControlMenuWidget::HandleWeatherHovered()
{
	SetHoveredCategory(EHouseControlMenuCategory::Weather);
}

void UHouseControlMenuWidget::HandleWeatherUnhovered()
{
	ClearHoveredCategory(EHouseControlMenuCategory::Weather);
}

void UHouseControlMenuWidget::HandleWeatherClicked()
{
	SelectCategoryCenter(EHouseControlMenuCategory::Weather);
}

void UHouseControlMenuWidget::HandleTimeOfDayHovered()
{
	SetHoveredCategory(EHouseControlMenuCategory::TimeOfDay);
}

void UHouseControlMenuWidget::HandleTimeOfDayUnhovered()
{
	ClearHoveredCategory(EHouseControlMenuCategory::TimeOfDay);
}

void UHouseControlMenuWidget::HandleTimeOfDayClicked()
{
	SelectCategoryCenter(EHouseControlMenuCategory::TimeOfDay);
}

void UHouseControlMenuWidget::HandleHouseStateHovered()
{
	SetHoveredCategory(EHouseControlMenuCategory::HouseState);
}

void UHouseControlMenuWidget::HandleHouseStateUnhovered()
{
	ClearHoveredCategory(EHouseControlMenuCategory::HouseState);
}

void UHouseControlMenuWidget::HandleHouseStateClicked()
{
	SelectCategoryCenter(EHouseControlMenuCategory::HouseState);
}

void UHouseControlMenuWidget::HandleOptionZeroClicked()
{
	SelectOption(0);
}

void UHouseControlMenuWidget::HandleOptionOneClicked()
{
	SelectOption(1);
}

void UHouseControlMenuWidget::HandleOptionTwoClicked()
{
	SelectOption(2);
}
