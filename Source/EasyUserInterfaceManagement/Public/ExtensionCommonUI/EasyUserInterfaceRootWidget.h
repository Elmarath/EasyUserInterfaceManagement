// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "CommonActivatableWidget.h"
#include "Notification/EasyNotificationWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "EasyUserInterfaceRootWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnWidgetUpdatedOnStack, FGameplayTag, LayerTag, UCommonActivatableWidget*, ActivatableWidget);

USTRUCT()
struct FPushWidgetToStackAsyncParams
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag LayerTag;

	UPROPERTY()
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass;

	bool operator==(const FPushWidgetToStackAsyncParams& Other) const
	{
		return LayerTag == Other.LayerTag && WidgetClass == Other.WidgetClass;
	}

	void Reset()
	{
		LayerTag = FGameplayTag();
		WidgetClass.Reset();
	}

	FPushWidgetToStackAsyncParams()
	{
		LayerTag = FGameplayTag();
		WidgetClass = TSoftClassPtr<UCommonActivatableWidget>();
	}

	FPushWidgetToStackAsyncParams(const FGameplayTag& InLayerTag, const TSoftClassPtr<UCommonActivatableWidget>& InWidgetClass)
	{
		LayerTag = InLayerTag;
		WidgetClass = InWidgetClass;
	}
};


/**
 * Root widget that is the parent of all user interface widgets in the Easy User Interface Management system for global management.
 */
UCLASS(Abstract)
class EASYUSERINTERFACEMANAGEMENT_API UEasyUserInterfaceRootWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UEasyUserInterfaceRootWidget();

public:
	virtual void NativeConstruct() override;

private:
	/** Internal map of registered widget stacks by their layer tags */
	UPROPERTY()
	TMap<FGameplayTag, UCommonActivatableWidgetStack*> WidgetStacks;

public:
	/**
	 * Registers a widget stack for a specific layer tag to manage activatable widgets.
	 * @param LayerTag The tag representing the activatable widget stack. Note: Simple View: "UI.Layer.SimpleView"
	 * @param WidgetClass The class of the widget stack to register. This should be a subclass of UCommonActivatableWidgetStack.
	 */
	UFUNCTION(BlueprintCallable, BlueprintType, Category = "Root Widget|Activatable Widget Stack", meta = (Categories = "UI.Layer"))
	void RegisterWidgetStack(FGameplayTag LayerTag, UCommonActivatableWidgetStack* WidgetClass);

	/**
	 * Pushes a widget to the specified stack while asynchronously loading it.
	 * @param LayerTag The tag representing the activatable widget stack. Note: Simple View: "UI.Layer.SimpleView"
	 * @param WidgetClass The class of the widget to push onto the stack. This should be a subclass of UCommonActivatableWidget.
	 * @param OnWidgetAdded Delegate to call when a widget is added to the stack. This will be called with the LayerTag and the newly added widget.
	 */
	UFUNCTION(BlueprintCallable, BlueprintType, Category = "Root Widget|Activatable Widget Stack", meta = (Categories = "UI.Layer"))
	void PushWidgetToStackAsync(FGameplayTag LayerTag, TSoftClassPtr<UCommonActivatableWidget> WidgetClass, FOnWidgetUpdatedOnStack OnWidgetAdded);

	UFUNCTION(BlueprintPure, BlueprintType, Category = "Root Widget|Activatable Widget Stack", meta = (Categories = "UI.Layer"))
	UCommonActivatableWidgetStack* GetWidgetStack(const FGameplayTag& LayerTag) const;

public:
	/**
	 * Registers a notification panel for a specific layer tag to manage notifications.
	 * @param LayerTag The tag representing the notification panel layer. Note: Simple View: "UI.NotificationType.SimpleView"
	 * @param ContainerWidget The panel widget that contain the notifications. Note: Should implement IEasyNotificationWidgetPanelInterface.
	 */
	UFUNCTION(BlueprintCallable, BlueprintType, Category = "Root Widget|Notification", meta = (Categories = "UI.NotificationType"))
	void RegisterNotificationPanel(FGameplayTag LayerTag, UWidget* ContainerWidget);
	/**
	 * Unregisters a notification panel for a specific layer tag.
	 * @param LayerTag The tag representing the notification panel layer to unregister. Note: Simple View: "UI.NotificationType.SimpleView"
	 * @return Returns true if the panel was successfully unregistered, false if no panel was found for the given layer tag.
	 */
	UFUNCTION(BlueprintCallable, BlueprintType, Category = "Root Widget|Notification", meta = (Categories = "UI.NotificationType"))
	bool UnRegisterNotificationPanel(const FGameplayTag& LayerTag);

	/**
	 * Retrieves the notification panel associated with the specified layer tag.
	 * @param LayerTag The tag representing the notification panel layer. Note: Simple View: "UI.NotificationType.SimpleView"
	 * @return The widget representing the notification panel for the specified layer tag, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, BlueprintType, Category = "Root Widget|Notification", meta = (Categories = "UI.NotificationType"))
	UWidget* GetNotificationPanel(const FGameplayTag& LayerTag) const;

public:
	/**
	 * Adds a notification widget to the specified notification panel with given info.
	 * @param LayerTag The tag representing the notification panel layer. Note: Simple View: "UI.NotificationType.SimpleView"
	 * @param NotificationWidget The notification widget to add to the panel.
	 * @param InNotificationInfo The information struct containing details about the notification.
	 */
	UFUNCTION(BlueprintCallable, BlueprintType, Category = "Root Widget")
	void AddNotificationWidgetToPanel(FGameplayTag LayerTag, UEasyNotificationWidget* NotificationWidget, const FEasyNotificationWidgetInfo& InNotificationInfo);
	
private:
	/** Internal Loading Widget Param */
	UPROPERTY()
	FPushWidgetToStackAsyncParams LoadingWidgetParams;
	/** Internal map of Registered Notification panels to display notifications inside */
	UPROPERTY()
	TMap<FGameplayTag, UWidget*> NotificationPanels;
};
