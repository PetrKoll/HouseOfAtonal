#include "UI/HouseControlMenuWidget.h"

void UHouseControlMenuWidget::SetHoveredCategory(EHouseControlMenuCategory Category)
{
	const EHouseControlMenuCategory PreviousExpanded = GetExpandedCategory();
	HoveredCategory = Category;
	BroadcastExpandedCategoryChange(PreviousExpanded);
}

void UHouseControlMenuWidget::ClearHoveredCategory(EHouseControlMenuCategory Category)
{
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

EHouseControlMenuCategory UHouseControlMenuWidget::GetExpandedCategory() const
{
	return PinnedCategory != EHouseControlMenuCategory::None
		? PinnedCategory
		: HoveredCategory;
}

void UHouseControlMenuWidget::BroadcastExpandedCategoryChange(
	EHouseControlMenuCategory PreviousExpanded)
{
	const EHouseControlMenuCategory NewExpanded = GetExpandedCategory();
	if (PreviousExpanded != NewExpanded)
	{
		OnExpandedCategoryChanged.Broadcast(PreviousExpanded, NewExpanded);
	}
}
