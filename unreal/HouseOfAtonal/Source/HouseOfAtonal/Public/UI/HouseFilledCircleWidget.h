#pragma once

#include "Components/Widget.h"
#include "HouseFilledCircleWidget.generated.h"

/**
 * Texture-free filled circle rendered as colored Slate geometry.
 */
UCLASS(BlueprintType)
class HOUSEOFATONAL_API UHouseFilledCircleWidget : public UWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Circle")
	void SetFillColor(FLinearColor NewColor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FLinearColor FillColor = FLinearColor::White;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	TSharedPtr<class SHouseFilledCircle> CircleWidget;
};
