// Copyright Elmarath Studio 2025 All Rights Reserved.

#include "ExtensionCommonUI/Foundation/ButtonActions/ButtonAction_PushWidgetToStack.h"

#include "EasyUserInterfaceManagement.h"
#include "EasyUserInterfaceManagement/EasyUserInterfaceManager.h"

UButtonAction_PushWidgetToStack::UButtonAction_PushWidgetToStack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetToPush = TSoftClassPtr<UCommonActivatableWidget>();
	LayerTag = FGameplayTag();
}

void UButtonAction_PushWidgetToStack::ExecuteAction_Implementation(UWidget* InstigatorWidget)
{
	Super::ExecuteAction_Implementation(InstigatorWidget);

	if (!InstigatorWidget)
	{
		UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("InstigatorWidget is null. Cannot push widget to stack."));
		return;
	}

	// Get Local Player Subsystem
	UEasyUserInterfaceManager* Manager = InstigatorWidget->GetOwningLocalPlayer()->GetSubsystem<UEasyUserInterfaceManager>();
	const FOnWidgetUpdatedOnStack WidgetAddedDelegate;
	Manager->PushWidgetToStackAsync(LayerTag, WidgetToPush, WidgetAddedDelegate);
}
