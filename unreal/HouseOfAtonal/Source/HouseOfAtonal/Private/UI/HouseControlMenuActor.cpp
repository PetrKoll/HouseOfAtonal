#include "UI/HouseControlMenuActor.h"

#include "Blueprint/UserWidget.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"

AHouseControlMenuActor::AHouseControlMenuActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	MenuWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MenuWidget"));
	MenuWidgetComponent->SetupAttachment(SceneRoot);
	MenuWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	MenuWidgetComponent->SetDrawSize(FIntPoint(1024, 1024));
	MenuWidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));
	MenuWidgetComponent->SetTwoSided(true);
	MenuWidgetComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MenuWidgetComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MenuWidgetComponent->SetCollisionResponseToChannel(
		ECC_GameTraceChannel1, ECR_Block);
}

void AHouseControlMenuActor::BeginPlay()
{
	Super::BeginPlay();

	if (FollowTarget && !bFollowInitialized)
	{
		CaptureRelativeOffset();
	}
}

void AHouseControlMenuActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bFollowInitialized || !IsValid(FollowTarget))
	{
		return;
	}

	const FTransform FollowFrame = BuildHorizontalFollowFrame();
	const FVector DesiredLocation = FollowFrame.TransformPosition(RelativeOffset);
	const float Separation = FVector::Distance(GetActorLocation(), DesiredLocation);
	const float EffectivePositionSpeed = Separation > MaximumComfortSeparation
		? PositionFollowSpeed * CatchUpSpeedMultiplier
		: PositionFollowSpeed;

	FVector NewLocation = FMath::VInterpTo(
		GetActorLocation(),
		DesiredLocation,
		DeltaSeconds,
		EffectivePositionSpeed);
	NewLocation.Z = FMath::FInterpTo(
		GetActorLocation().Z,
		DesiredLocation.Z,
		DeltaSeconds,
		VerticalFollowSpeed);

	const FVector ToUser = FollowTarget->GetComponentLocation() - NewLocation;
	const FRotator LookAtRotation = ToUser.Rotation();
	const FRotator DesiredRotation(
		LookAtRotation.Pitch,
		LookAtRotation.Yaw,
		0.0f);
	const FRotator NewRotation = FMath::RInterpTo(
		GetActorRotation(),
		DesiredRotation,
		DeltaSeconds,
		RotationFollowSpeed);

	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AHouseControlMenuActor::InitializeFollow(USceneComponent* InFollowTarget)
{
	FollowTarget = InFollowTarget;
	CaptureRelativeOffset();
}

void AHouseControlMenuActor::SetMenuWidgetClass(TSubclassOf<UUserWidget> InWidgetClass)
{
	MenuWidgetComponent->SetWidgetClass(InWidgetClass);
}

FTransform AHouseControlMenuActor::BuildHorizontalFollowFrame() const
{
	if (!IsValid(FollowTarget))
	{
		return FTransform::Identity;
	}

	const FRotator TargetRotation = FollowTarget->GetComponentRotation();
	const FRotator HorizontalRotation(0.0f, TargetRotation.Yaw, 0.0f);
	return FTransform(HorizontalRotation, FollowTarget->GetComponentLocation());
}

void AHouseControlMenuActor::CaptureRelativeOffset()
{
	if (!IsValid(FollowTarget))
	{
		bFollowInitialized = false;
		return;
	}

	RelativeOffset = BuildHorizontalFollowFrame().InverseTransformPosition(GetActorLocation());
	bFollowInitialized = true;
}
