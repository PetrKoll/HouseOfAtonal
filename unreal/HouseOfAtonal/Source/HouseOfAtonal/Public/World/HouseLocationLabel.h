#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseLocationLabel.generated.h"

class USceneComponent;
class UTextRenderComponent;

UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseLocationLabel : public AActor
{
	GENERATED_BODY()

public:
	AHouseLocationLabel();
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Label")
	FText PlaceName = FText::FromString(TEXT("School"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Label")
	FText Distance = FText::FromString(TEXT("340 m"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Label")
	FLinearColor LabelColor = FLinearColor(0.92f, 0.95f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Label",
		meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "10.0"))
	float WorldSize = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Location Label")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Location Label")
	TObjectPtr<UTextRenderComponent> Text;
};
