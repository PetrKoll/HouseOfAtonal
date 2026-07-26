#include "UI/HouseControlMenuComponent.h"

#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "UI/HouseControlMenuActor.h"
#include "UI/HouseControlMenuWidget.h"

UHouseControlMenuComponent::UHouseControlMenuComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MenuActorClass = AHouseControlMenuActor::StaticClass();
}

void UHouseControlMenuComponent::ToggleControlMenu(
	USceneComponent* FollowTarget,
	FVector PointerOrigin,
	FVector PointerDirection)
{
	if (IsControlMenuVisible())
	{
		HideControlMenu();
		return;
	}

	ShowControlMenu(FollowTarget, PointerOrigin, PointerDirection);
}

void UHouseControlMenuComponent::ShowControlMenu(
	USceneComponent* FollowTarget,
	FVector PointerOrigin,
	FVector PointerDirection)
{
	if (!IsValid(FollowTarget) || !GetWorld())
	{
		return;
	}

	HideControlMenu();

	const FVector Direction = PointerDirection.GetSafeNormal();
	if (Direction.IsNearlyZero())
	{
		return;
	}

	const FVector SpawnLocation = PointerOrigin + Direction * SpawnDistance;
	const FVector ToUser = FollowTarget->GetComponentLocation() - SpawnLocation;
	const FRotator SpawnRotation(0.0f, ToUser.Rotation().Yaw, 0.0f);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ActiveControlMenu = GetWorld()->SpawnActor<AHouseControlMenuActor>(
		MenuActorClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParameters);

	if (!IsValid(ActiveControlMenu))
	{
		return;
	}

	if (MenuWidgetClass)
	{
		ActiveControlMenu->SetMenuWidgetClass(MenuWidgetClass);
		UWidgetComponent* WidgetComponent =
			ActiveControlMenu->GetMenuWidgetComponent();
		WidgetComponent->InitWidget();

		if (UHouseControlMenuWidget* MenuWidget =
			Cast<UHouseControlMenuWidget>(WidgetComponent->GetUserWidgetObject()))
		{
			MenuWidget->OnDismissRequested.AddDynamic(
				this,
				&UHouseControlMenuComponent::HideControlMenu);
		}
	}

	ActiveControlMenu->InitializeFollow(FollowTarget);
}

void UHouseControlMenuComponent::HideControlMenu()
{
	if (IsValid(ActiveControlMenu))
	{
		ActiveControlMenu->Destroy();
	}

	ActiveControlMenu = nullptr;
}

bool UHouseControlMenuComponent::IsControlMenuVisible() const
{
	return IsValid(ActiveControlMenu);
}

void UHouseControlMenuComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	HideControlMenu();
	Super::EndPlay(EndPlayReason);
}
