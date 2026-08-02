#pragma once

#include "CoreMinimal.h"
#include "Experience/HouseExperienceTypes.h"
#include "GameFramework/Actor.h"
#include "HouseTimeController.generated.h"

UENUM(BlueprintType)
enum class EHouseTimeState : uint8
{
	State01 UMETA(DisplayName = "Time State 01"),
	State02 UMETA(DisplayName = "Time State 02"),
	State03 UMETA(DisplayName = "Time State 03")
};

USTRUCT(BlueprintType)
struct HOUSEOFATONAL_API FHouseCapturedUDSProperty
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Profile")
	FName PropertyName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Profile")
	FString ExportedValue;
};

USTRUCT(BlueprintType)
struct HOUSEOFATONAL_API FHouseTimeProfile
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Profile")
	float TimeOfDay = 800.0f;

	/** Complete snapshot of editable Ultra Dynamic Sky instance properties. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Profile")
	TArray<FHouseCapturedUDSProperty> Snapshot;
};

/**
 * Project-owned three-state time adapter for Ultra Dynamic Sky.
 *
 * The vendor actor stays unmodified. This actor reads and writes only its
 * exposed "Time of Day" value and always transitions forward around the day.
 */
UCLASS(BlueprintType, Blueprintable)
class HOUSEOFATONAL_API AHouseTimeController : public AActor
{
	GENERATED_BODY()

public:
	AHouseTimeController();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	/** Ultra Dynamic Sky actor controlled by this adapter. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Time Controller")
	TObjectPtr<AActor> UltraDynamicSky;

	/** Selecting a state in the editor previews its stored time immediately. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Controller|Authoring")
	EHouseTimeState EditingTimeState = EHouseTimeState::State01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Controller|States")
	FHouseTimeProfile TimeState01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Controller|States")
	FHouseTimeProfile TimeState02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Controller|States")
	FHouseTimeProfile TimeState03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Controller|Transition",
		meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "30.0", Units = "s"))
	float TransitionDuration = 5.0f;

	/** Copies the current UDS Time of Day into the selected state. */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Time Controller|Authoring")
	void CaptureCurrentUDSTime();

	/** Applies the selected state immediately while authoring the level. */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Time Controller|Authoring")
	void PreviewSelectedTimeState();

	/** Starts a smooth forward-only transition to one of the three states. */
	UFUNCTION(BlueprintCallable, Category = "Time Controller")
	void SetTimeState(EHouseTimeState NewState, bool bInstant = false);

	UFUNCTION(BlueprintPure, Category = "Time Controller")
	EHouseTimeState GetCurrentTimeState() const { return CurrentTimeState; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Controller|Runtime")
	EHouseTimeState CurrentTimeState = EHouseTimeState::State01;

private:
	UFUNCTION()
	void HandleExperienceTimeChanged(EHouseTimePreset PreviousTime, EHouseTimePreset NewTime);

	FHouseTimeProfile& GetProfile(EHouseTimeState State);
	const FHouseTimeProfile& GetProfile(EHouseTimeState State) const;
	void CaptureUDSSnapshot(FHouseTimeProfile& Profile) const;
	void ApplyDiscreteSnapshotValues(const FHouseTimeProfile& Profile) const;
	bool ReadNumericUDSProperty(FName PropertyLabel, double& OutValue) const;
	bool WriteNumericUDSProperty(FName PropertyLabel, double NewValue) const;
	FProperty* FindUDSProperty(FName PropertyName) const;
	bool IsCapturableUDSProperty(const FProperty* Property) const;
	static float NormalizeUDSTime(float Time);
	static EHouseTimeState MapExperienceTime(EHouseTimePreset TimePreset);

	bool bTransitionActive = false;
	float TransitionElapsed = 0.0f;
	float TransitionStart = 0.0f;
	float TransitionForwardDistance = 0.0f;
	float TransitionTarget = 0.0f;
	TMap<FName, double> TransitionStartParameters;
	TMap<FName, double> TransitionTargetParameters;
	TMap<FName, FLinearColor> TransitionStartColors;
	TMap<FName, FLinearColor> TransitionTargetColors;
};
