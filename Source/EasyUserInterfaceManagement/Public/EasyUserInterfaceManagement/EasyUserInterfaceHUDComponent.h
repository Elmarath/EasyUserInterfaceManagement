// Copyright Elmarath Studio 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EasyUserInterfaceHUDComponent.generated.h"

class UEasyUserInterfaceRootWidget;

USTRUCT(BlueprintType)
struct FRootWidgetInitializationParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	TSoftClassPtr<UEasyUserInterfaceRootWidget> RootWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization", meta = (DisplayName = "Z-Order"))
	int32 ZOrder = 0;

	FRootWidgetInitializationParams()
	{
		RootWidgetClass = TSoftClassPtr<UEasyUserInterfaceRootWidget>();
		ZOrder = 0;
	}

	FRootWidgetInitializationParams(const TSoftClassPtr<UEasyUserInterfaceRootWidget>& InRootWidgetClass, const int32 InZOrder = 0)
	{
		RootWidgetClass = InRootWidgetClass;
		ZOrder = InZOrder;
	}
};

UCLASS(ClassGroup=("User Interface"), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable, DisplayName="Easy User Interface HUD")
class EASYUSERINTERFACEMANAGEMENT_API UEasyUserInterfaceHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEasyUserInterfaceHUDComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Easy User Interface Management")
	FRootWidgetInitializationParams DefaultRootWidget;
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Easy User Interface Management")
	FRootWidgetInitializationParams GetRootWidgetInitializationParams() const;

private:
	UFUNCTION()
	void RegisterRootWidgetToManager() const;
};
