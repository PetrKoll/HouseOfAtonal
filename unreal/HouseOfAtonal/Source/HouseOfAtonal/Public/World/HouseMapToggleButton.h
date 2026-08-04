#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseMapToggleButton.generated.h"

class AHouseMapViewController;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseMapToggleButton : public AActor
{
	GENERATED_BODY()

public:
	AHouseMapToggleButton();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Map Toggle")
	TObjectPtr<AHouseMapViewController> Controller;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Toggle")
	FText MapLabel = FText::FromString(TEXT("MAP"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Toggle")
	FText HomeLabel = FText::FromString(TEXT("HOME"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Toggle",
		meta = (ClampMin = "20.0", UIMin = "20.0", UIMax = "250.0", Units = "cm"))
	float HeightBelowUser = 110.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map Toggle")
	TObjectPtr<UStaticMeshComponent> Disc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map Toggle")
	TObjectPtr<UTextRenderComponent> Text;

	UFUNCTION(BlueprintCallable, Category = "Map Toggle")
	void Activate();

private:
	UFUNCTION()
	void HandleClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION()
	void HandleModeChanged(bool bIsMapView);
};
