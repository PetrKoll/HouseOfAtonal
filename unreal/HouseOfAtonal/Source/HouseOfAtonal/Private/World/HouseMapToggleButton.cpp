#include "World/HouseMapToggleButton.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "World/HouseMapViewController.h"

AHouseMapToggleButton::AHouseMapToggleButton()
{
	PrimaryActorTick.bCanEverTick = true;
	Disc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disc"));
	SetRootComponent(Disc);
	Disc->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Disc->SetCollisionResponseToAllChannels(ECR_Ignore);
	Disc->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Disc->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.04f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(
		TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (Cylinder.Succeeded())
	{
		Disc->SetStaticMesh(Cylinder.Object);
	}

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(Disc);
	Text->SetRelativeLocation(FVector(0.0f, 0.0f, 55.0f));
	Text->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Text->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Text->SetWorldSize(18.0f);
	Text->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Text->SetText(MapLabel);
	OnClicked.AddDynamic(this, &AHouseMapToggleButton::HandleClicked);
}

void AHouseMapToggleButton::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetWorld())
	{
		if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (const APawn* Pawn = PlayerController->GetPawn())
			{
				SetActorLocation(Pawn->GetActorLocation() - FVector::UpVector * HeightBelowUser);
			}
		}
	}
}

void AHouseMapToggleButton::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(Controller))
	{
		Controller->OnMapViewModeChanged.AddUniqueDynamic(
			this, &AHouseMapToggleButton::HandleModeChanged);
		HandleModeChanged(Controller->IsInMapView());
	}
}

void AHouseMapToggleButton::Activate()
{
	if (IsValid(Controller))
	{
		Controller->ToggleMapView();
	}
}

void AHouseMapToggleButton::HandleClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	Activate();
}

void AHouseMapToggleButton::HandleModeChanged(const bool bIsMapView)
{
	Text->SetText(bIsMapView ? HomeLabel : MapLabel);
}
