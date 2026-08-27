#include "World/HouseTimeController.h"

#include "Experience/HouseExperienceSubsystem.h"
#include "Components/ActorComponent.h"
#include "EngineUtils.h"
#include "UObject/UnrealType.h"

namespace
{
	constexpr float UDSDayLength = 2400.0f;

	FString NormalizePropertyLabel(FString Label)
	{
		Label.ReplaceInline(TEXT("_"), TEXT(""));
		Label.ReplaceInline(TEXT(" "), TEXT(""));
		return Label.ToLower();
	}
}

AHouseTimeController::AHouseTimeController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	TimeState01.TimeOfDay = 800.0f;
	TimeState02.TimeOfDay = 1700.0f;
	TimeState03.TimeOfDay = 2200.0f;
}

void AHouseTimeController::BeginPlay()
{
	Super::BeginPlay();
	ResolveRainActors();

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UHouseExperienceSubsystem* Experience =
			GameInstance->GetSubsystem<UHouseExperienceSubsystem>())
		{
			Experience->OnTimeChanged.AddUniqueDynamic(
				this, &AHouseTimeController::HandleExperienceTimeChanged);
			Experience->OnWeatherChanged.AddUniqueDynamic(
				this, &AHouseTimeController::HandleExperienceWeatherChanged);
			CurrentTimeState = MapExperienceTime(Experience->GetTimeOfDay());
			SetTimeState(CurrentTimeState, true);
			SetWeather(Experience->GetWeather(), true);
			return;
		}
	}

	SetTimeState(CurrentTimeState, true);
}

void AHouseTimeController::ResolveRainActors()
{
	RainActors.RemoveAll([](const TObjectPtr<AActor>& Actor)
	{
		return !IsValid(Actor);
	});
	if (!RainActors.IsEmpty() || !GetWorld())
	{
		return;
	}

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->GetClass()->GetPathName().Contains(TEXT("/EasyRain/")))
		{
			RainActors.Add(*It);
		}
	}
}

void AHouseTimeController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bTransitionActive && !bWeatherTransitionActive)
	{
		SetActorTickEnabled(false);
		return;
	}

	if (bTransitionActive)
	{
		TransitionElapsed += DeltaSeconds;
		const float Alpha = TransitionDuration <= KINDA_SMALL_NUMBER
			? 1.0f
			: FMath::Clamp(TransitionElapsed / TransitionDuration, 0.0f, 1.0f);
		const float SmoothAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);
		WriteNumericUDSProperty(TEXT("Time of Day"), NormalizeUDSTime(
			TransitionStart + TransitionForwardDistance * SmoothAlpha));
		if (Alpha >= 1.0f)
		{
			WriteNumericUDSProperty(TEXT("Time of Day"), TransitionTarget);
			bTransitionActive = false;
		}
	}

	if (bWeatherTransitionActive)
	{
		WeatherTransitionElapsed += DeltaSeconds;
		const float Alpha = WeatherTransitionDuration <= KINDA_SMALL_NUMBER
			? 1.0f
			: FMath::Clamp(WeatherTransitionElapsed / WeatherTransitionDuration, 0.0f, 1.0f);
		const float SmoothAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);
		const double CloudCoverage = FMath::Lerp(
			WeatherStartCloudCoverage, WeatherTargetCloudCoverage, SmoothAlpha);
		const double Fog = FMath::Lerp(WeatherStartFog, WeatherTargetFog, SmoothAlpha);
		WriteNumericUDSProperty(TEXT("Cloud Coverage"), CloudCoverage);
		WriteNumericUDSProperty(TEXT("Weather Cloud Coverage"), CloudCoverage);
		WriteNumericUDSProperty(TEXT("Fog"), Fog);
		WriteNumericUDSProperty(TEXT("Weather Fog"), Fog);
		if (Alpha >= 1.0f)
		{
			bWeatherTransitionActive = false;
		}
	}

	if (!bTransitionActive && !bWeatherTransitionActive)
	{
		SetActorTickEnabled(false);
	}
}

void AHouseTimeController::CaptureCurrentUDSTime()
{
	double CurrentUDSTime = 0.0;
	if (!ReadNumericUDSProperty(TEXT("Time of Day"), CurrentUDSTime))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("HouseTimeController: assign an Ultra Dynamic Sky actor exposing 'Time of Day'."));
		return;
	}

	Modify();
	FHouseTimeProfile& Profile = GetProfile(EditingTimeState);
	Profile.TimeOfDay = NormalizeUDSTime(static_cast<float>(CurrentUDSTime));
	CaptureUDSSnapshot(Profile);

	MarkPackageDirty();
	UE_LOG(LogTemp, Display,
		TEXT("HouseTimeController: captured time %.2f and %d editable UDS properties."),
		Profile.TimeOfDay, Profile.Snapshot.Num());
}

void AHouseTimeController::PreviewSelectedTimeState()
{
	bTransitionActive = false;
	SetActorTickEnabled(false);
	CurrentTimeState = EditingTimeState;
	const FHouseTimeProfile& Profile = GetProfile(EditingTimeState);
	if (!WriteNumericUDSProperty(TEXT("Time of Day"), Profile.TimeOfDay))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("HouseTimeController: assign an Ultra Dynamic Sky actor exposing 'Time of Day'."));
	}
	ApplyDiscreteSnapshotValues(Profile);
}

void AHouseTimeController::SetTimeState(const EHouseTimeState NewState, const bool bInstant)
{
	CurrentTimeState = NewState;
	const FHouseTimeProfile& TargetProfile = GetProfile(NewState);
	TransitionTarget = NormalizeUDSTime(TargetProfile.TimeOfDay);

	double CurrentUDSTime = TransitionTarget;
	if (!ReadNumericUDSProperty(TEXT("Time of Day"), CurrentUDSTime))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("HouseTimeController: assign an Ultra Dynamic Sky actor exposing 'Time of Day'."));
		return;
	}

	TransitionStart = NormalizeUDSTime(static_cast<float>(CurrentUDSTime));
	TransitionForwardDistance = TransitionTarget - TransitionStart;
	if (TransitionForwardDistance < 0.0f)
	{
		TransitionForwardDistance += UDSDayLength;
	}
	if (bInstant || TransitionDuration <= KINDA_SMALL_NUMBER ||
		TransitionForwardDistance <= KINDA_SMALL_NUMBER)
	{
		bTransitionActive = false;
		WriteNumericUDSProperty(TEXT("Time of Day"), TransitionTarget);
		if (!bWeatherTransitionActive)
		{
			SetActorTickEnabled(false);
		}
		return;
	}

	TransitionElapsed = 0.0f;
	bTransitionActive = true;
	SetActorTickEnabled(true);
}

void AHouseTimeController::SetWeather(const EHouseWeatherPreset NewWeather, const bool bInstant)
{
	switch (NewWeather)
	{
	case EHouseWeatherPreset::Rain:
		WeatherTargetCloudCoverage = RainCloudCoverage;
		WeatherTargetFog = RainFog;
		SetRainEnabled(true);
		break;
	case EHouseWeatherPreset::Fog:
		WeatherTargetCloudCoverage = FogCloudCoverage;
		WeatherTargetFog = FoggyFog;
		SetRainEnabled(false);
		break;
	default:
		WeatherTargetCloudCoverage = SunnyCloudCoverage;
		WeatherTargetFog = SunnyFog;
		SetRainEnabled(false);
		break;
	}

	double CurrentCloudCoverage = WeatherTargetCloudCoverage;
	double CurrentFog = WeatherTargetFog;
	ReadNumericUDSProperty(TEXT("Cloud Coverage"), CurrentCloudCoverage);
	ReadNumericUDSProperty(TEXT("Fog"), CurrentFog);
	WeatherStartCloudCoverage = static_cast<float>(CurrentCloudCoverage);
	WeatherStartFog = static_cast<float>(CurrentFog);

	if (bInstant || WeatherTransitionDuration <= KINDA_SMALL_NUMBER)
	{
		bWeatherTransitionActive = false;
		WriteNumericUDSProperty(TEXT("Cloud Coverage"), WeatherTargetCloudCoverage);
		WriteNumericUDSProperty(TEXT("Weather Cloud Coverage"), WeatherTargetCloudCoverage);
		WriteNumericUDSProperty(TEXT("Fog"), WeatherTargetFog);
		WriteNumericUDSProperty(TEXT("Weather Fog"), WeatherTargetFog);
		if (!bTransitionActive)
		{
			SetActorTickEnabled(false);
		}
		return;
	}

	WeatherTransitionElapsed = 0.0f;
	bWeatherTransitionActive = true;
	SetActorTickEnabled(true);
}

void AHouseTimeController::SetRainEnabled(const bool bEnabled) const
{
	for (AActor* RainActor : RainActors)
	{
		if (!IsValid(RainActor))
		{
			continue;
		}
		RainActor->SetActorHiddenInGame(!bEnabled);
		RainActor->SetActorEnableCollision(false);
		TInlineComponentArray<UActorComponent*> Components(RainActor);
		for (UActorComponent* Component : Components)
		{
			if (bEnabled)
			{
				Component->Activate(true);
			}
			else
			{
				Component->Deactivate();
			}
		}
	}
}

void AHouseTimeController::HandleExperienceTimeChanged(
	const EHouseTimePreset PreviousTime,
	const EHouseTimePreset NewTime)
{
	SetTimeState(MapExperienceTime(NewTime));
}

void AHouseTimeController::HandleExperienceWeatherChanged(
	const EHouseWeatherPreset PreviousWeather,
	const EHouseWeatherPreset NewWeather)
{
	SetWeather(NewWeather);
}

FHouseTimeProfile& AHouseTimeController::GetProfile(const EHouseTimeState State)
{
	switch (State)
	{
	case EHouseTimeState::State01:
		return TimeState01;
	case EHouseTimeState::State02:
		return TimeState02;
	case EHouseTimeState::State03:
		return TimeState03;
	default:
		return TimeState01;
	}
}

const FHouseTimeProfile& AHouseTimeController::GetProfile(const EHouseTimeState State) const
{
	switch (State)
	{
	case EHouseTimeState::State01:
		return TimeState01;
	case EHouseTimeState::State02:
		return TimeState02;
	case EHouseTimeState::State03:
		return TimeState03;
	default:
		return TimeState01;
	}
}

void AHouseTimeController::CaptureUDSSnapshot(FHouseTimeProfile& Profile) const
{
	Profile.Snapshot.Reset();
	if (!IsValid(UltraDynamicSky))
	{
		return;
	}

	for (TFieldIterator<FProperty> PropertyIt(UltraDynamicSky->GetClass()); PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;
		if (!IsCapturableUDSProperty(Property))
		{
			continue;
		}

		FHouseCapturedUDSProperty Captured;
		Captured.PropertyName = Property->GetFName();
		const void* ValueAddress = Property->ContainerPtrToValuePtr<void>(UltraDynamicSky);
		Property->ExportTextItem_Direct(
			Captured.ExportedValue, ValueAddress, nullptr, UltraDynamicSky, PPF_None);
		Profile.Snapshot.Add(MoveTemp(Captured));
	}
}

void AHouseTimeController::ApplyDiscreteSnapshotValues(const FHouseTimeProfile& Profile) const
{
	if (!IsValid(UltraDynamicSky))
	{
		return;
	}

	for (const FHouseCapturedUDSProperty& Captured : Profile.Snapshot)
	{
		FProperty* Property = FindUDSProperty(Captured.PropertyName);
		const FStructProperty* StructProperty = CastField<FStructProperty>(Property);
		const bool bIsLinearColor = StructProperty &&
			StructProperty->Struct == TBaseStructure<FLinearColor>::Get();
		if (!Property || Property->IsA<FNumericProperty>() || bIsLinearColor)
		{
			continue;
		}

		void* ValueAddress = Property->ContainerPtrToValuePtr<void>(UltraDynamicSky);
		Property->ImportText_Direct(
			*Captured.ExportedValue, ValueAddress, UltraDynamicSky, PPF_None);
	}
}

bool AHouseTimeController::ReadNumericUDSProperty(
	const FName PropertyLabel, double& OutValue) const
{
	if (!IsValid(UltraDynamicSky))
	{
		return false;
	}

	if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(FindUDSProperty(PropertyLabel)))
	{
		const void* ValueAddress = NumericProperty->ContainerPtrToValuePtr<void>(UltraDynamicSky);
		OutValue = NumericProperty->IsFloatingPoint()
			? NumericProperty->GetFloatingPointPropertyValue(ValueAddress)
			: static_cast<double>(NumericProperty->GetSignedIntPropertyValue(ValueAddress));
		return true;
	}

	return false;
}

bool AHouseTimeController::WriteNumericUDSProperty(
	const FName PropertyLabel, const double NewValue) const
{
	if (!IsValid(UltraDynamicSky))
	{
		return false;
	}

	if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(FindUDSProperty(PropertyLabel)))
	{
		void* ValueAddress = NumericProperty->ContainerPtrToValuePtr<void>(UltraDynamicSky);
		if (NumericProperty->IsFloatingPoint())
		{
			NumericProperty->SetFloatingPointPropertyValue(ValueAddress, NewValue);
		}
		else
		{
			NumericProperty->SetIntPropertyValue(ValueAddress, static_cast<int64>(FMath::RoundToDouble(NewValue)));
		}
		return true;
	}

	return false;
}

FProperty* AHouseTimeController::FindUDSProperty(const FName PropertyLabel) const
{
	if (!IsValid(UltraDynamicSky))
	{
		return nullptr;
	}

	const FString RequestedLabel = NormalizePropertyLabel(PropertyLabel.ToString());
	for (TFieldIterator<FProperty> PropertyIt(UltraDynamicSky->GetClass()); PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;
		if (!Property->IsA<FNumericProperty>())
		{
			continue;
		}

		const FString InternalName = NormalizePropertyLabel(Property->GetName());
		const FString DisplayName = NormalizePropertyLabel(Property->GetDisplayNameText().ToString());
		if (InternalName == RequestedLabel || DisplayName == RequestedLabel)
		{
			return Property;
		}
	}

	return nullptr;
}

bool AHouseTimeController::IsCapturableUDSProperty(const FProperty* Property) const
{
	if (!Property || !Property->HasAnyPropertyFlags(CPF_Edit) ||
		Property->HasAnyPropertyFlags(CPF_Transient | CPF_DuplicateTransient | CPF_DisableEditOnInstance))
	{
		return false;
	}

	const UClass* OwnerClass = Property->GetOwnerClass();
	return OwnerClass && !OwnerClass->HasAnyClassFlags(CLASS_Native);
}

float AHouseTimeController::NormalizeUDSTime(const float Time)
{
	float Normalized = FMath::Fmod(Time, UDSDayLength);
	if (Normalized < 0.0f)
	{
		Normalized += UDSDayLength;
	}
	return Normalized;
}

EHouseTimeState AHouseTimeController::MapExperienceTime(const EHouseTimePreset TimePreset)
{
	switch (TimePreset)
	{
	case EHouseTimePreset::GoldenHour:
	case EHouseTimePreset::Evening:
		return EHouseTimeState::State02;
	case EHouseTimePreset::Night:
		return EHouseTimeState::State03;
	default:
		return EHouseTimeState::State01;
	}
}
