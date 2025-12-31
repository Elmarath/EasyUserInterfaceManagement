// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "ExtensionCommonUI/Foundation/ButtonActions/ButtonAction_AddNotification.h"

#include "EasyUserInterfaceManagement.h"
#include "EasyUserInterfaceManagement/EasyUserInterfaceManager.h"

UButtonAction_AddNotification::UButtonAction_AddNotification(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NotificationType = FGameplayTag::EmptyTag;
	NotificationWidgetClass = nullptr;
	NotificationInfo = FEasyNotificationWidgetInfo();
}

void UButtonAction_AddNotification::ExecuteAction_Implementation(UWidget* InstigatorWidget)
{
	Super::ExecuteAction_Implementation(InstigatorWidget);

	// Ensure the InstigatorWidget is valid and has a LocalPlayer
	ULocalPlayer* LocalPlayer = InstigatorWidget->GetOwningLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Invalid LocalPlayer in ButtonAction_AddNotification::ExecuteAction_Implementation"));
		return;
	}
	UEasyUserInterfaceManager* UserInterfaceManager = LocalPlayer->GetSubsystem<UEasyUserInterfaceManager>();

	if (IsValid(UserInterfaceManager))
	{
		FOnNotificationAddedToPanel OnNotificationAddedDelegate;
		UserInterfaceManager->AddNotificationToPanel(NotificationType, NotificationWidgetClass, NotificationInfo, OnNotificationAddedDelegate);
		return;
	}

	UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Invalid UserInterfaceManager in ButtonAction_AddNotification::ExecuteAction_Implementation"));
}
