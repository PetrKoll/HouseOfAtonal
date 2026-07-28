#include "UI/HouseControlMenuComponent.h"

#include "Components/SceneComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"
#include "UI/HouseControlMenuActor.h"
#include "UI/HouseRadialMenuV1.h"

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
		if (UWidgetInteractionComponent* WidgetInteraction =
			FindRightWidgetInteraction())
		{
			if (WidgetInteraction->IsOverInteractableWidget())
			{
				WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
				WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
				return;
			}
		}

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

	ActiveControlMenu->SetMenuWidgetClass(
		UHouseRadialMenuV1::StaticClass());
	UWidgetComponent* WidgetComponent =
		ActiveControlMenu->GetMenuWidgetComponent();
	WidgetComponent->SetRelativeScale3D(
		WidgetComponent->GetRelativeScale3D() *
		MenuWorldScaleMultiplier);
	WidgetComponent->InitWidget();

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

UWidgetInteractionComponent*
UHouseControlMenuComponent::FindRightWidgetInteraction() const
{
	if (!GetOwner())
	{
		return nullptr;
	}

	TArray<UWidgetInteractionComponent*> Interactions;
	GetOwner()->GetComponents<UWidgetInteractionComponent>(Interactions);

	for (UWidgetInteractionComponent* Interaction : Interactions)
	{
		if (IsValid(Interaction) &&
			Interaction->GetName().Contains(TEXT("Right")))
		{
			return Interaction;
		}
	}

	return Interactions.Num() > 0 ? Interactions[0] : nullptr;
}
