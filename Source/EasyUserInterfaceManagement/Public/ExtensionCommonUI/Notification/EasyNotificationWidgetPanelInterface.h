// Copyright Elmarath Studio 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EasyNotificationWidgetPanelInterface.generated.h"

struct FGameplayTag;
// This class does not need to be modified.
UINTERFACE()
class UEasyNotificationWidgetPanelInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EASYUSERINTERFACEMANAGEMENT_API IEasyNotificationWidgetPanelInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Notification Panel", meta = (Categories = "UI.NotificationType"))
	void OnNotificationWidgetAdded(const FGameplayTag& LayerTag, class UEasyNotificationWidget* NotificationWidget);
};
