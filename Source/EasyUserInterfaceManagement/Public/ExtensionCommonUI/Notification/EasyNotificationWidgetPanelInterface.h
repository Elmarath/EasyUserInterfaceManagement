// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EasyNotificationWidgetPanelInterface.generated.h"

struct FGameplayTag;
// 
UINTERFACE()
class UEasyNotificationWidgetPanelInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for notification widget panels to handle added notification widgets.
 * So they have a way of knowing when a new notification is added and their information.
 */
class EASYUSERINTERFACEMANAGEMENT_API IEasyNotificationWidgetPanelInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Notification Panel", meta = (Categories = "UI.NotificationType"))
	void OnNotificationWidgetAdded(const FGameplayTag& LayerTag, class UEasyNotificationWidget* NotificationWidget);
};
