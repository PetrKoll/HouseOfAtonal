#include "Experience/HouseExperienceSubsystem.h"

void UHouseExperienceSubsystem::SetLocation(EHouseLocation NewLocation)
{
	if (Location == NewLocation)
	{
		return;
	}

	const EHouseLocation PreviousLocation = Location;
	Location = NewLocation;
	OnLocationChanged.Broadcast(PreviousLocation, Location);
}

void UHouseExperienceSubsystem::SetWeather(EHouseWeatherPreset NewWeather)
{
	if (Weather == NewWeather)
	{
		return;
	}

	const EHouseWeatherPreset PreviousWeather = Weather;
	Weather = NewWeather;
	OnWeatherChanged.Broadcast(PreviousWeather, Weather);
}

void UHouseExperienceSubsystem::SetTimeOfDay(EHouseTimePreset NewTime)
{
	if (TimeOfDay == NewTime)
	{
		return;
	}

	const EHouseTimePreset PreviousTime = TimeOfDay;
	TimeOfDay = NewTime;
	OnTimeChanged.Broadcast(PreviousTime, TimeOfDay);
}
