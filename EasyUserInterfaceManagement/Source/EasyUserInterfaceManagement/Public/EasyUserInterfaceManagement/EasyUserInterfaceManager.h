// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputModeTypes.h"
#include "GameplayTagContainer.h"
#include "ExtensionCommonUI/EasyUserInterfaceRootWidget.h"
#include "ExtensionCommonUI/Notification/EasyNotificationWidget.h"
#include "ExtensionCommonUI/PromptQuestion/EasyPromptQuestion.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "EasyUserInterfaceManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRootWidgetAddedToViewport, UEasyUserInterfaceRootWidget*, RootWidget);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnNotificationAddedToPanel, FGameplayTag, LayerTag, UEasyNotificationWidget*, NotificationWidget);

DECLARE_DYNAMIC_DELEGATE_OneParam(FPromtResultEventDelegate, EPromptQuestionResult, Result);

/**
 * Manager for the Easy User Interface system, responsible for managing root widgets and their stacks.
 */
UCLASS()
class EASYUSERINTERFACEMANAGEMENT_API UEasyUserInterfaceManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	/** Adds the specified root widget to the viewport.
	 * @param InRootWidget The root widget class to be added to the viewport.
	 * @param ZOrder The Z-order for the widget in the viewport.
	 */
	UFUNCTION()
	void AddRootWidgetToViewport(TSoftClassPtr<UEasyUserInterfaceRootWidget> InRootWidget, int32 ZOrder = 0);

public:
	/** Retrieves the current root widget.
	 * @return The current root widget.
	 */
	UFUNCTION(BlueprintPure, Category = "Easy User Interface Management")
	UEasyUserInterfaceRootWidget* GetRootWidget();

public:
	/** Event triggered when the root widget is added to the viewport. */
	UPROPERTY(BlueprintAssignable, Category = "Easy User Interface Management")
	FOnRootWidgetAddedToViewport OnRootWidgetAddedToViewport;

public:
	/**
	 * Pushes a widget to the specified stack while asynchronously loading it.
	 * If any widget is already being loaded for the specified LayerTag and has the same class, it will skip the request.
	 * @param LayerTag The tag representing the activatable widget stack.
	 * @param OnWidgetAdded Delegate to call when a widget is added to the stack.
	 * @return True if the request to load and push the widget was initiated, false otherwise. (May fail if the root widget not yet registered or invalid class provided)
	 */
	UFUNCTION(BlueprintCallable, Category = "Easy User Interface Management|Activatable Widget", meta = (Categories = "UI.Layer"))
	bool PushWidgetToStackAsync(FGameplayTag LayerTag, TSoftClassPtr<UCommonActivatableWidget> InWidget, FOnWidgetUpdatedOnStack OnWidgetAdded);

	/**
	 * Retrieves the widget stack associated with the specified layer tag.
	 * @param LayerTag The tag representing the activatable widget stack.
	 * @return The widget stack associated with the specified layer tag, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category = "Easy User Interface Management|Activatable Widget", meta = (Categories = "UI.Layer"))
	UCommonActivatableWidgetStack* GetWidgetStackFromRoot(FGameplayTag LayerTag) const;

public:
	/**
	 * Adds a notification widget to the specified panel layer asynchronously.
	 * @param LayerTag The tag representing the notification panel layer.
	 * @param NotificationWidgetClass The class of the notification widget to be added.
	 * @param NotificationInfo The information to be displayed in the notification.
	 * @param OnNotificationAdded Delegate to call when the notification is added to the panel.
	 * @return True if the request to load and add the notification was initiated, false otherwise. (May fail if the root widget not yet registered or invalid class provided)
	 */
	UFUNCTION(BlueprintCallable, Category = "Easy User Interface Management|Notification", meta = (Categories = "UI.NotificationType"))
	bool AddNotificationToPanel(FGameplayTag LayerTag, TSoftClassPtr<UEasyNotificationWidget> NotificationWidgetClass, const FEasyNotificationWidgetInfo& NotificationInfo, FOnNotificationAddedToPanel OnNotificationAdded);

public:
	/**
	 * Pushes a question prompt widget to the specified stack layer, in the root widget.
	 * @param PromptWidgetClass The class of the prompt question widget to be pushed.
	 * @param StackLayer The tag representing the activatable widget stack layer.
	 * @param InQuestionBodyInfo The information about the prompt question body.
	 * @param OnPromptResult Delegate to call when the prompt result is available.
	 * @return The created prompt question widget, or nullptr if the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Easy User Interface Management|Prompt Question", meta = (Categories = "UI.Layer"))
	UEasyPromptQuestion* PushQuestionPromptToLayer(TSubclassOf<UEasyPromptQuestion> PromptWidgetClass, FGameplayTag StackLayer, const FPromptQuestionBodyInfo& InQuestionBodyInfo, FPromtResultEventDelegate OnPromptResult);
	
public:
	/**
	 * Sets a common input mode for the specified player controller.
	 * @param PlayerController The player controller to set the input mode for.
	 * @param CommonInputMode The common input mode to be set.
	 * @param MouseCaptureMode The mouse capture mode to be set.
	 * @param bHideMouse Whether to hide the mouse cursor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Easy User Interface Management|Input")
	void SetCommonInputMode(APlayerController* PlayerController, ECommonInputMode CommonInputMode, EMouseCaptureMode MouseCaptureMode, bool bHideMouse);
	/**
	 * Retrieves the current common input mode for the specified player controller.
	 * @param PlayerController The player controller to get the input mode from.
	 * @return The current common input mode.
	 */
	UFUNCTION(BlueprintPure, Category = "Easy User Interface Management|Input")
	ECommonInputMode GetCurrentCommonInputMode(const APlayerController* PlayerController) const;

public:
	/**
	 * Globally accessible root widget managed by this User Interface Manager.
	 */
	UPROPERTY(Transient)
	TObjectPtr<UEasyUserInterfaceRootWidget> RootWidget;
};
