// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonActivatableWidget.h"
#include "EasyActivatableWidgetWithInputModes.generated.h"

UENUM(BlueprintType)
enum class ECommonActivatableWidgetInputMode : uint8
{
	Default UMETA(DisplayName = "Default"), // No input mode set, will use the default input mode of the game
	GameAndMenu UMETA(DisplayName = "Game and Menu"), // Both the Game And The Menu will receive input
	Game UMETA(DisplayName = "Game"), // Game will receive input, menu will not
	Menu UMETA(DisplayName = "Menu"), // Menu will receive input, game will not
};

/**
 * Object that simplifies the configuration of input modes for UCommonActivatableWidget derived widgets.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, ClassGroup = ("User Interface"), meta = (BlueprintSpawnableComponent), CollapseCategories)
class UUserInterfaceCommonInputModeSet : public UObject
{
	GENERATED_BODY()

public:
	UUserInterfaceCommonInputModeSet()
	{
		InputMode = ECommonActivatableWidgetInputMode::Menu;
		GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
		GameMouseLockMode = EMouseLockMode::LockAlways;
		bHideCursorDuringViewportCapture = false;
	}

protected:
	/** The input mode to be applied when the widget is activated */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ECommonActivatableWidgetInputMode InputMode = ECommonActivatableWidgetInputMode::Default;
	/** The mouse lock mode to be applied when the widget is activated (only applies if InputMode is not Default) */
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (EditCondition = "InputMode != ECommonActivatableWidgetInputMode::Default"))
	EMouseLockMode GameMouseLockMode = EMouseLockMode::DoNotLock;
	/** The mouse capture mode to be applied when the widget is activated (only applies if InputMode is Game or GameAndMenu) */
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (EditCondition = "InputMode != ECommonActivatableWidgetInputMode::Default || InputMode != ECommonActivatableWidgetInputMode::Menu"))
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
	/** Whether to hide the cursor during viewport capture (only applies if InputMode is Game or GameAndMenu) */
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (EditCondition = "InputMode != ECommonActivatableWidgetInputMode::Default || InputMode != ECommonActivatableWidgetInputMode::Menu"))
	bool bHideCursorDuringViewportCapture = false;

public:
	UFUNCTION()
	TOptional<FUIInputConfig> GetInputConfig() const
	{
		switch (InputMode)
		{
		case ECommonActivatableWidgetInputMode::GameAndMenu:
			return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode, GameMouseLockMode, bHideCursorDuringViewportCapture);
		case ECommonActivatableWidgetInputMode::Game:
			return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode, GameMouseLockMode, bHideCursorDuringViewportCapture);
		case ECommonActivatableWidgetInputMode::Menu:
			return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture, GameMouseLockMode, false);
		case ECommonActivatableWidgetInputMode::Default:
		default:
			return TOptional<FUIInputConfig>();
		}
	}
};

/**
 * An activatable widget that simplifies the management of input modes by its settings.
 */
UCLASS()
class EASYUSERINTERFACEMANAGEMENT_API UEasyActivatableWidgetWithInputModes : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UEasyActivatableWidgetWithInputModes(FObjectInitializer const& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, Category = "Input", Instanced)
	TObjectPtr<UUserInterfaceCommonInputModeSet> InputModeSet;

public:
	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

#if WITH_EDITORONLY_DATA
	/**
	 * Enables/Disables compilation of input debug messages (compilation) in the editor.
	 */
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bShowInputDebugMessages = true;
#endif 
	
#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif

};
