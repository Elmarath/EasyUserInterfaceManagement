// Copyright Elmarath Studio 2025 All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EasyUserInterfaceHUDComponent.generated.h"

class UEasyUserInterfaceRootWidget;

USTRUCT(BlueprintType)
struct FRootWidgetInitializationParams
{
	GENERATED_BODY()
	/** The Root Widget Class to be created on HUD initialization */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	TSoftClassPtr<UEasyUserInterfaceRootWidget> RootWidgetClass;
	/** The Z-Order to be used when adding the Root Widget to the viewport */
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

/**
 * This component is used for defining HUD related properties for Easy User Interface Management system.
 * So that User Interface Manager Local Subsystem can automatically create and manage the Root Widget defined here.
 * Creating the User Interface happens on the "begin play" of this component.
 */
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
	/**
	 * Default Root Widget Initialization Parameters used by User Interface Manager Local Subsystem to create the
	 * Root Widget on HUD initialization.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Easy User Interface Management")
	FRootWidgetInitializationParams DefaultRootWidget;
	
public:
	/**
	 * Gets the Root Widget Initialization Parameters.
	 * @return The Root Widget Initialization Parameters.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Easy User Interface Management")
	FRootWidgetInitializationParams GetRootWidgetInitializationParams() const;

private:
	/** Registers the Root Widget to the User Interface Manager Local Subsystem */
	UFUNCTION()
	void RegisterRootWidgetToManager() const;
};
