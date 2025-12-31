// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "ExtensionCommonUI/ActivatableWidget/EasyActivatableWidgetBase.h"

#include "EasyUserInterfaceManagement/EasyCommonUIActionRouter.h"
#include "Editor/WidgetCompilerLog.h"
#include "Input/CommonUIInputTypes.h"

#define LOCTEXT_NAMESPACE "EasyActivatableWidgetBase"

UEasyActivatableWidgetBase::UEasyActivatableWidgetBase(const FObjectInitializer& ObjectInitializer) : UEasyActivatableWidgetWithInputModes(ObjectInitializer)
{

}

#if WITH_EDITOR
void UEasyActivatableWidgetBase::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
	class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);
}
#endif

void UEasyActivatableWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEasyActivatableWidgetBase::NativeDestruct()
{
	for (FUIActionBindingHandle Handle : BindingHandles)
	{
		if (Handle.IsValid())
		{
			Handle.Unregister();
		}
	}
	BindingHandles.Empty();
	
	Super::NativeDestruct();
}

void UEasyActivatableWidgetBase::NativeOnActivated()
{
	Super::NativeOnActivated();
	RegisterDefaultInputs();

	if (IsValid(GetOwningLocalPlayer()))
	{
		UEasyCommonUIActionRouter* CommonUIActionRouter = GetOwningLocalPlayer()->GetSubsystem<UEasyCommonUIActionRouter>();
		CommonUIActionRouter->SetAnalogMovementEnabled(bEnableAnalogCursor);
	}
}

void UEasyActivatableWidgetBase::NativeOnDeactivated()
{
	UnregisterAllBindings();
	if (IsValid(GetOwningLocalPlayer()) && GetOwningLocalPlayer()->GetSubsystem<UEasyCommonUIActionRouter>())
	{
		UEasyCommonUIActionRouter* CommonUIActionRouter = GetOwningLocalPlayer()->GetSubsystem<UEasyCommonUIActionRouter>();
		CommonUIActionRouter->SetAnalogMovementEnabled(false);
	}
	
	Super::NativeOnDeactivated();
}

void UEasyActivatableWidgetBase::RegisterBinding(FUserInterfaceInputActionToRegister InputAction,
												 const FInputActionExecutedDelegate& Callback,
												 FInputActionBindingHandle& BindingHandle)
{
	// Build the binding arguments using the full struct
	FBindUIActionArgs BindArgs(InputAction.InputAction, FSimpleDelegate::CreateLambda([InputAction, Callback]()
	{
		Callback.ExecuteIfBound(InputAction.InputAction.RowName);
	}));
	
	BindArgs.bDisplayInActionBar = InputAction.bDisplayInActionBarByDefault;
	BindArgs.bForceHold = InputAction.bForceHold;
	BindArgs.bConsumeInput = InputAction.bConsumeInput;
	BindArgs.OverrideDisplayName = InputAction.OverrideDisplayName;
	BindArgs.KeyEvent = InputAction.KeyEvent;
	// Register and store the handle
	BindingHandle.Handle = RegisterUIActionBinding(BindArgs);
	BindingHandles.Add(BindingHandle.Handle);
}

void UEasyActivatableWidgetBase::UnregisterBinding(FInputActionBindingHandle BindingHandle)
{
	if (BindingHandle.Handle.IsValid())
	{
		BindingHandle.Handle.Unregister();
		BindingHandles.Remove(BindingHandle.Handle);
		
	}
}

void UEasyActivatableWidgetBase::UnregisterAllBindings()
{
	for (FUIActionBindingHandle Handle : BindingHandles)
	{
		Handle.Unregister();
	}
	BindingHandles.Empty();
}

void UEasyActivatableWidgetBase::OnDefaultInputExecuted_Implementation(FName ActionName)
{
	for (const FUserInterfaceInputActionToRegister& InputAction : DefaultInputs)
	{
		// Ensure the input action is valid before registering
		if (!InputAction.InputAction.IsNull())
		{
			FInputActionBindingHandle BindingHandle;

			// Create a delegate for the default input execution
			FInputActionExecutedDelegate Callback;
			Callback.BindDynamic(this, &UEasyActivatableWidgetBase::OnDefaultInputExecuted);

			// Register the binding
			RegisterBinding(InputAction, Callback, BindingHandle);
		}
	}
}

void UEasyActivatableWidgetBase::RegisterDefaultInputs()
{
	for (const FUserInterfaceInputActionToRegister& InputAction : DefaultInputs)
	{
		// Ensure the input action is valid before registering
		if (!InputAction.InputAction.IsNull())
		{
			FInputActionBindingHandle BindingHandle;

			// Create a delegate for the default input execution
			FInputActionExecutedDelegate Callback;
			Callback.BindDynamic(this, &UEasyActivatableWidgetBase::OnDefaultInputExecuted);

			// Register the binding
			RegisterBinding(InputAction, Callback, BindingHandle);
		}
	}
}

#undef LOCTEXT_NAMESPACE