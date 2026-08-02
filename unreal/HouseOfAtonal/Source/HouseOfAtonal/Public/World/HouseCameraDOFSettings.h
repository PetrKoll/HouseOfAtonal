#pragma once

#include "GameFramework/Actor.h"
#include "HouseCameraDOFSettings.generated.h"

class UPostProcessComponent;
class USceneComponent;

/** Level-editable depth-of-field controller shared by the editor and XR view. */
UCLASS()
class HOUSEOFATONAL_API AHouseCameraDOFSettings : public AActor
{
	GENERATED_BODY()

public:
	AHouseCameraDOFSettings();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera DOF")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Camera DOF")
	TObjectPtr<UPostProcessComponent> PostProcess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera DOF")
	bool bEnableDepthOfField = false;

	/** Distance to the sharp focus plane in centimeters. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera DOF", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10000.0", EditCondition = "bEnableDepthOfField"))
	float FocusDistance = 1500.0f;

	/** Lower values create stronger blur. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera DOF", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "22.0", EditCondition = "bEnableDepthOfField"))
	float Aperture = 5.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera DOF", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "5000.0", EditCondition = "bEnableDepthOfField"))
	float FocalRegion = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera DOF", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "5000.0", EditCondition = "bEnableDepthOfField"))
	float NearTransition = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera DOF", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "5000.0", EditCondition = "bEnableDepthOfField"))
	float FarTransition = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera DOF", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "bEnableDepthOfField"))
	float BlendWeight = 1.0f;

private:
	void ApplySettings();
};
