// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "ExtensionCommonUI/ActivatableWidget/EasyActivatableWidgetWithInputModes.h"

#include "Editor/WidgetCompilerLog.h"

#define LOCTEXT_NAMESPACE "EasyActivatableWidgetWithInputModes"

UEasyActivatableWidgetWithInputModes::UEasyActivatableWidgetWithInputModes(FObjectInitializer const& ObjectInitializer)
{
	InputModeSet = ObjectInitializer.CreateDefaultSubobject<UUserInterfaceCommonInputModeSet>(this, TEXT("InputModeSet"));
}

TOptional<FUIInputConfig> UEasyActivatableWidgetWithInputModes::GetDesiredInputConfig() const
{
	if (InputModeSet)
	{
		return InputModeSet->GetInputConfig();
	}
	
	return Super::GetDesiredInputConfig();
}

#if WITH_EDITOR
void UEasyActivatableWidgetWithInputModes::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
	class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);

	if (!bShowInputDebugMessages)
	{
		return;
	}
	
	if (!GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UEasyActivatableWidgetWithInputModes, BP_GetDesiredFocusTarget)))
	{
		CompileLog.Warning(LOCTEXT("ValidateGetDesiredFocusTarget_Warning", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen."));

		//TODO - Note for now, because we can't guarantee it isn't implemented in a native subclass of this one.
		CompileLog.Note(LOCTEXT("ValidateGetDesiredFocusTarget_Note", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen.  If it was implemented in the native base class you can ignore this message."));
	}
}
#endif

#undef LOCTEXT_NAMESPACE