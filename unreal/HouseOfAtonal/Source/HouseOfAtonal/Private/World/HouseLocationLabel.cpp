#include "World/HouseLocationLabel.h"

#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"

AHouseLocationLabel::AHouseLocationLabel()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(Root);
	Text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Text->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Text->SetWorldSize(WorldSize);
	Text->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AHouseLocationLabel::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Text->SetText(FText::Format(
		FText::FromString(TEXT("{0}  {1}")), PlaceName, Distance));
	Text->SetTextRenderColor(LabelColor.ToFColor(true));
	Text->SetWorldSize(WorldSize);
}
