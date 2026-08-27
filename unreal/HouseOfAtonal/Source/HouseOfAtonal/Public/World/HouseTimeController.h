#pragma once

#include "CoreMinimal.h"
#include "Experience/HouseExperienceTypes.h"
#include "GameFramework/Actor.h"
#include "HouseTimeController.generated.h"

class UMaterialInstanceDynamic;

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

	/** Actors authored inside the time_01 folder. Populated by the level setup utility. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Time Controller|State Content")
	TArray<TObjectPtr<AActor>> TimeState01Actors;

	/** Actors authored inside the time_02 folder. Populated by the level setup utility. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Time Controller|State Content")
	TArray<TObjectPtr<AActor>> TimeState02Actors;

	/** Actors authored inside the time_03 folder. Populated by the level setup utility. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Time Controller|State Content")
	TArray<TObjectPtr<AActor>> TimeState03Actors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Controller|Transition",
		meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "30.0", Units = "s"))
	float TransitionDuration = 5.0f;

	/** Existing rain Blueprint actors. They stay inactive for sunny and foggy weather. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weather Controller")
	TArray<TObjectPtr<AActor>> RainActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Cloud Coverage")
	float SunnyCloudCoverage = 3.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Cloud Coverage")
	float MorningRainCloudCoverage = 8.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Cloud Coverage")
	float AfternoonRainCloudCoverage = 7.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Cloud Coverage")
	float NightRainCloudCoverage = 9.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Cloud Coverage")
	float MorningFogCloudCoverage = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Cloud Coverage")
	float AfternoonFogCloudCoverage = 5.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Cloud Coverage")
	float NightFogCloudCoverage = 7.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Fog")
	float SunnyFog = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Fog")
	float MorningRainFog = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Fog")
	float AfternoonRainFog = 2.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Fog")
	float NightRainFog = 3.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Fog")
	float MorningFog = 8.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Fog")
	float AfternoonFog = 5.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Fog")
	float NightFog = 7.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Controller|Transition",
		meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "30.0", Units = "s"))
	float WeatherTransitionDuration = 4.0f;

	/** Copies the current UDS Time of Day into the selected state. */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Time Controller|Authoring")
	void CaptureCurrentUDSTime();

	/** Applies the selected state immediately while authoring the level. */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Time Controller|Authoring")
	void PreviewSelectedTimeState();

	/** Starts a smooth forward-only transition to one of the three states. */
	UFUNCTION(BlueprintCallable, Category = "Time Controller")
	void SetTimeState(EHouseTimeState NewState, bool bInstant = false);

	UFUNCTION(BlueprintCallable, Category = "Weather Controller")
	void SetWeather(EHouseWeatherPreset NewWeather, bool bInstant = false);

	UFUNCTION(BlueprintPure, Category = "Time Controller")
	EHouseTimeState GetCurrentTimeState() const { return CurrentTimeState; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time Controller|Runtime")
	EHouseTimeState CurrentTimeState = EHouseTimeState::State01;

private:
	UFUNCTION()
	void HandleExperienceTimeChanged(EHouseTimePreset PreviousTime, EHouseTimePreset NewTime);

	UFUNCTION()
	void HandleExperienceWeatherChanged(EHouseWeatherPreset PreviousWeather, EHouseWeatherPreset NewWeather);

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
	float GetRainCloudCoverage() const;
	float GetRainFog() const;
	float GetFogCloudCoverage() const;
	float GetFogAmount() const;
	void ResolveRainActors();
	void SetRainEnabled(bool bEnabled) const;
	void SetTimeStateContentVisibility(EHouseTimeState ActiveState) const;
	void InitializeLightBulbMaterials();
	void SetLightBulbIntensity(float Intensity) const;

	bool bTransitionActive = false;
	float TransitionElapsed = 0.0f;
	float TransitionStart = 0.0f;
	float TransitionForwardDistance = 0.0f;
	float TransitionTarget = 0.0f;
	bool bWeatherTransitionActive = false;
	float WeatherTransitionElapsed = 0.0f;
	float WeatherStartCloudCoverage = 0.0f;
	float WeatherTargetCloudCoverage = 0.0f;
	float WeatherStartFog = 0.0f;
	float WeatherTargetFog = 0.0f;
	EHouseWeatherPreset CurrentWeather = EHouseWeatherPreset::Clear;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMaterialInstanceDynamic>> LightBulbMaterials;
};
