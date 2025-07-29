// Copyright Elmarath Studio 2025


#include "ExtensionCommonUI/Foundation/ButtonActions/ButtonAction_Back.h"
#include "CommonUILibrary.h"
#include "CommonActivatableWidget.h"
#include "EasyUserInterfaceManagement.h"

UButtonAction_Back::UButtonAction_Back(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FindParentToDeactivate = UCommonActivatableWidget::StaticClass();
}

void UButtonAction_Back::ExecuteAction_Implementation(UWidget* InstigatorWidget)
{
	Super::ExecuteAction_Implementation(InstigatorWidget);

	if (UCommonActivatableWidget* ParentActivatableWidget =
	Cast<UCommonActivatableWidget>(UCommonUILibrary::FindParentWidgetOfType(InstigatorWidget, FindParentToDeactivate)))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Log, TEXT("UButtonAction_Back: Deactivating widget %s"), *ParentActivatableWidget->GetName());
		ParentActivatableWidget->DeactivateWidget();
	}
	else
	{
		UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("UButtonAction_Back: No parent UCommonActivatableWidget found for %s"), *InstigatorWidget->GetName());
	}
}
