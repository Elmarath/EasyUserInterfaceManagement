// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "EasyActivatableWidgetWithInputModes.h"
#include "EasyActivatableWidgetBase.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FInputActionExecutedDelegate, FName, ActionName);

USTRUCT(BlueprintType)
struct FInputActionBindingHandle
{
	GENERATED_BODY()
	
public:
	/** The handle to the input binding. */
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	FUIActionBindingHandle Handle;

public:
	// define == operator for FInputActionBindingHandle
	bool operator==(const FInputActionBindingHandle& Other) const
	{
		return Handle == Other.Handle;
	}
};

USTRUCT(BlueprintType)
struct FUserInterfaceInputActionToRegister
{
	GENERATED_BODY()

	/**
	 * The input action to register.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle InputAction;

	/**
	 * Should the input action be displayed in the action bar by default?
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bDisplayInActionBarByDefault = true;

	/**
	 * True implies we will add default hold times if the current action is not a hold action
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bForceHold = false;

	/**
	 * The key event to bind to.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TEnumAsByte<EInputEvent> KeyEvent = IE_Pressed;

	/**
	 * True to have this binding consume the triggering key input.
	 * Persistent bindings that consume will prevent the key reaching non-persistent bindings and game agents.
	 * Non-persistent bindings that consume will prevent the key reaching game agents.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bConsumeInput = true;

	/**
	 * Optional display name to associate with this binding instead of the default.
	 * Note: Empty display names will use the default display name from the input action data table.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FText OverrideDisplayName;
};

/**
 * Base class for activatable widgets that can register and unregister input actions and setup easy input modes when activated.
 * The main responsibility of this widget is to manage input bindings and modes when the widget is activated or deactivated,
 * and create/destroy the input bindings at runtime as needed.
 */
UCLASS()
class EASYUSERINTERFACEMANAGEMENT_API UEasyActivatableWidgetBase : public UEasyActivatableWidgetWithInputModes
{
	GENERATED_BODY()

public:
	UEasyActivatableWidgetBase(const FObjectInitializer& ObjectInitializer);

public:
	/**
	 * The inputs to bind by default when the widget is activated.
	 * Note: This actions will be unregistered when the widget is destroyed.
	 *
	 * To use the inputs, use the "OnDefaultInputExecuted" event to handle the input action execution via input names.
	 * Note: The input data table should be using "CommonInputActionDataBase" as the row structure.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FUserInterfaceInputActionToRegister> DefaultInputs;

	/**
	 * If true, the widget will enable analog cursor support from the CommonUIActionRouterBase subsystem.
	 *
	 * Note: If true this will enable the analog cursor when activated.
	 * But in any case analog cursor will be disabled when the widget is deactivated.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bEnableAnalogCursor = false;
	
#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	/**
	 * Registers a binding for an input action at runtime.
	 * @param InputAction The input action to bind.
	 * @param Callback The delegate to call when the input action is executed.
	 * @param BindingHandle The handle to the binding that can be used to unregister it later.
	 */
	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void RegisterBinding(FUserInterfaceInputActionToRegister InputAction, const FInputActionExecutedDelegate& Callback, FInputActionBindingHandle& BindingHandle);

	/**
	 * Unregisters a binding for an input action at runtime.
	 * @param BindingHandle The handle to the binding that was registered.
	 */
	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void UnregisterBinding(FInputActionBindingHandle BindingHandle);
	/**
	 * Unregisters all bindings that were registered by this widget.
	 */
	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void UnregisterAllBindings();
	/**
	 * Checks if a binding is already registered.
	 * @param BindingHandle The handle to the binding to check.
	 * @param bIsBindingValid Output parameter that indicates if the binding handle is valid.
	 * @return True if the binding is already registered, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = ExtendedActivatableWidget)
	bool IsBindingAlreadyRegistered(const FInputActionBindingHandle& BindingHandle, bool& bIsBindingValid) const
	{
		bIsBindingValid = BindingHandle.Handle.IsValid();
		for (const FUIActionBindingHandle& PreviouslyBoundHandle : BindingHandles)
		{
			if (PreviouslyBoundHandle == BindingHandle.Handle)
			{
				return true;
			}
		}
		return false;
	}
	
	UFUNCTION(BlueprintImplementableEvent, Category = ExtendedActivatableWidget)
	void OnDefaultInputExecuted(FName ActionName);
	virtual void OnDefaultInputExecuted_Implementation(FName ActionName);
	
private:
	TArray<FUIActionBindingHandle> BindingHandles;

private:
	UFUNCTION()
	void RegisterDefaultInputs();

};
