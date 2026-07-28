#include "World/HouseMenuLevelPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputCoreTypes.h"
#include "InputMappingContext.h"
#include "MotionControllerComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UI/HouseControlMenuComponent.h"
#include "UI/HouseMenuLevelSelector.h"
#include "UI/HouseRadialMenuV1.h"
#include "UObject/ConstructorHelpers.h"

AHouseMenuLevelPawn::AHouseMenuLevelPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	TrackingOrigin = CreateDefaultSubobject<USceneComponent>(
		TEXT("TrackingOrigin"));
	SetRootComponent(TrackingOrigin);

	HeadCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HeadCamera"));
	HeadCamera->SetupAttachment(TrackingOrigin);
	HeadCamera->bLockToHmd = true;

	RightControllerAim = CreateDefaultSubobject<UMotionControllerComponent>(
		TEXT("MotionControllerRightAim"));
	RightControllerAim->SetupAttachment(TrackingOrigin);
	RightControllerAim->MotionSource = FName(TEXT("RightAim"));

	WidgetInteractionRight =
		CreateDefaultSubobject<UWidgetInteractionComponent>(
			TEXT("WidgetInteractionRight"));
	WidgetInteractionRight->SetupAttachment(RightControllerAim);
	WidgetInteractionRight->InteractionSource =
		EWidgetInteractionSource::World;
	WidgetInteractionRight->InteractionDistance = 500.0f;
	WidgetInteractionRight->TraceChannel = ECC_GameTraceChannel1;
	WidgetInteractionRight->PointerIndex = 1;
	WidgetInteractionRight->bShowDebug = false;
	WidgetInteractionRight->DebugColor =
		FLinearColor(0.96f, 0.72f, 0.18f, 1.0f);
	WidgetInteractionRight->DebugLineThickness = 0.35f;
	WidgetInteractionRight->DebugSphereLineThickness = 0.35f;

	ControlMenu = CreateDefaultSubobject<UHouseControlMenuComponent>(
		TEXT("HouseControlMenu"));

	ViewMovement = CreateDefaultSubobject<UFloatingPawnMovement>(
		TEXT("ViewMovement"));
	ViewMovement->MaxSpeed = 180.0f;
	ViewMovement->Acceleration = 600.0f;
	ViewMovement->Deceleration = 900.0f;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputContext(
		TEXT("/Game/XRFramework/Input/IMC_Default.IMC_Default"));
	if (InputContext.Succeeded())
	{
		DefaultInputContext = InputContext.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MenuAction(
		TEXT("/Game/HouseOfAtonal/Input/IA_ControlMenu_Open.IA_ControlMenu_Open"));
	if (MenuAction.Succeeded())
	{
		ControlMenuAction = MenuAction.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveAction(
		TEXT("/Game/XRFramework/Input/Actions/IA_Move.IA_Move"));
	if (MoveAction.Succeeded())
	{
		ViewMoveAction = MoveAction.Object;
	}
}

void AHouseMenuLevelPawn::BeginPlay()
{
	Super::BeginPlay();

	if (ControlMenu && GetWorld())
	{
		const bool bIsInitialMenuLevel =
			GetWorld()->GetMapName().Contains(TEXT("L_MenuLevel_Editable"));
		bIsViewLevel =
			GetWorld()->GetMapName().Contains(TEXT("L_ViewLevel"));
		ControlMenu->SetMenuWidgetClass(
			bIsInitialMenuLevel
				? UHouseMenuLevelSelector::StaticClass()
				: UHouseRadialMenuV1::StaticClass());
	}

	if (APlayerController* PlayerController =
		Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PlayerController->GetLocalPlayer()))
		{
			if (bIsViewLevel)
			{
				ViewInputContext = NewObject<UInputMappingContext>(this);
				if (ControlMenuAction)
				{
					ViewInputContext->MapKey(
						ControlMenuAction,
						EKeys::OculusTouch_Right_Trigger_Click);
				}
				if (ViewMoveAction)
				{
					ViewInputContext->MapKey(
						ViewMoveAction,
						EKeys::OculusTouch_Right_Thumbstick_2D);
					ViewInputContext->MapKey(
						ViewMoveAction,
						EKeys::Gamepad_Right2D);
				}
				InputSubsystem->AddMappingContext(ViewInputContext, 1);
			}
			else if (DefaultInputContext)
			{
				InputSubsystem->AddMappingContext(DefaultInputContext, 0);
			}
		}
	}
}

void AHouseMenuLevelPawn::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ControlMenuAction)
		{
			EnhancedInput->BindAction(
				ControlMenuAction,
				ETriggerEvent::Started,
				this,
				&AHouseMenuLevelPawn::HandleControlMenuInput);
		}
		if (ViewMoveAction)
		{
			EnhancedInput->BindAction(
				ViewMoveAction,
				ETriggerEvent::Triggered,
				this,
				&AHouseMenuLevelPawn::HandleViewMoveInput);
		}
	}
}

void AHouseMenuLevelPawn::HandleControlMenuInput(
	const FInputActionValue& Value)
{
	if (!ControlMenu || !RightControllerAim || !HeadCamera)
	{
		return;
	}

	ControlMenu->ToggleControlMenu(
		HeadCamera,
		RightControllerAim->GetComponentLocation(),
		RightControllerAim->GetForwardVector());
	WidgetInteractionRight->bShowDebug =
		ControlMenu->IsControlMenuVisible();
}

void AHouseMenuLevelPawn::HandleViewMoveInput(
	const FInputActionValue& Value)
{
	if (!bIsViewLevel || !HeadCamera)
	{
		return;
	}

	const FVector2D Axis = Value.Get<FVector2D>();

	FVector Forward = HeadCamera->GetForwardVector();
	Forward.Z = 0.0f;
	Forward.Normalize();
	FVector Right = HeadCamera->GetRightVector();
	Right.Z = 0.0f;
	Right.Normalize();

	// OpenXR's paired Oculus right-stick key arrives transposed relative to
	// the template IA_Move convention: X is physical forward, Y is strafe.
	AddMovementInput(Forward, Axis.X);
	AddMovementInput(Right, Axis.Y);
}
