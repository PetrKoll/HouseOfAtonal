#pragma once

#include "CoreMinimal.h"
#include "Experience/HouseExperienceTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HouseExperienceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FHouseLocationChanged,
	EHouseLocation,
	PreviousLocation,
	EHouseLocation,
	NewLocation);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FHouseWeatherChanged,
	EHouseWeatherPreset,
	PreviousWeather,
	EHouseWeatherPreset,
	NewWeather);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FHouseTimeChanged,
	EHouseTimePreset,
	PreviousTime,
	EHouseTimePreset,
	NewTime);

/**
 * Project-owned state boundary for the architectural experience.
 *
 * UI, level transitions and weather integrations request state changes here.
 * Concrete map loading and Ultra Dynamic Sky bindings are intentionally kept
 * outside this class until their functional specification is approved.
 */
UCLASS()
class HOUSEOFATONAL_API UHouseExperienceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "House of Atonal|Experience")
	EHouseLocation GetLocation() const { return Location; }

	UFUNCTION(BlueprintPure, Category = "House of Atonal|Experience")
	EHouseWeatherPreset GetWeather() const { return Weather; }

	UFUNCTION(BlueprintPure, Category = "House of Atonal|Experience")
	EHouseTimePreset GetTimeOfDay() const { return TimeOfDay; }

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Experience")
	void SetLocation(EHouseLocation NewLocation);

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Experience")
	void SetWeather(EHouseWeatherPreset NewWeather);

	UFUNCTION(BlueprintCallable, Category = "House of Atonal|Experience")
	void SetTimeOfDay(EHouseTimePreset NewTime);

	UPROPERTY(BlueprintAssignable, Category = "House of Atonal|Experience")
	FHouseLocationChanged OnLocationChanged;

	UPROPERTY(BlueprintAssignable, Category = "House of Atonal|Experience")
	FHouseWeatherChanged OnWeatherChanged;

	UPROPERTY(BlueprintAssignable, Category = "House of Atonal|Experience")
	FHouseTimeChanged OnTimeChanged;

private:
	UPROPERTY(VisibleAnywhere, Category = "House of Atonal|Experience")
	EHouseLocation Location = EHouseLocation::Menu;

	UPROPERTY(VisibleAnywhere, Category = "House of Atonal|Experience")
	EHouseWeatherPreset Weather = EHouseWeatherPreset::Clear;

	UPROPERTY(VisibleAnywhere, Category = "House of Atonal|Experience")
	EHouseTimePreset TimeOfDay = EHouseTimePreset::Noon;
};
