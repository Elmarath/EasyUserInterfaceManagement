// Copyright Elmarath Studio 2025 All Rights Reserved.

#include "ExtensionCommonUI/Foundation/ButtonActions/ButtonAction_ExecuteConsoleCommand.h"

#include "EasyUserInterfaceManagement.h"
#include "Components/Widget.h"

UButtonAction_ExecuteConsoleCommand::UButtonAction_ExecuteConsoleCommand(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConsoleCommand = TEXT("");
	bWriteToLog = false;
}

void UButtonAction_ExecuteConsoleCommand::ExecuteAction_Implementation(UWidget* InstigatorWidget)
{
	Super::ExecuteAction_Implementation(InstigatorWidget);

	if (InstigatorWidget == nullptr || ConsoleCommand.IsEmpty())
	{
		UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("InstigatorWidget is null or ConsoleCommand is empty!"));
		return;
	}
	if (APlayerController* PlayerController = Cast<APlayerController>(InstigatorWidget->GetOwningPlayer()))
	{
		PlayerController->ConsoleCommand(ConsoleCommand, bWriteToLog);
	}
	else
	{
		UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("InstigatorWidget does not have a valid PlayerController!"));
	}
}
