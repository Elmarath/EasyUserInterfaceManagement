// Copyright Elmarath Studio 2025

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
	UFUNCTION()
	void AddRootWidgetToViewport(TSoftClassPtr<UEasyUserInterfaceRootWidget> InRootWidget, int32 ZOrder = 0);

public:
	UFUNCTION(BlueprintPure, Category = "Easy User Interface Management")
	UEasyUserInterfaceRootWidget* GetRootWidget();

public:
	UPROPERTY(BlueprintAssignable, Category = "Easy User Interface Management")
	FOnRootWidgetAddedToViewport OnRootWidgetAddedToViewport;

public:
	/**
	 * Pushes a widget to the specified stack while asynchronously loading it.
	 * If any widget is already being loaded for the specified LayerTag and has the same class, it will skip the request.
	 * @param LayerTag The tag representing the activatable widget stack.
	 * @param OnWidgetAdded Delegate to call when a widget is added to the stack.
	 */
	UFUNCTION(BlueprintCallable, Category = "Easy User Interface Management|Activatable Widget", meta = (Categories = "UI.Layer"))
	void PushWidgetToStackAsync(FGameplayTag LayerTag, TSoftClassPtr<UCommonActivatableWidget> InWidget, FOnWidgetUpdatedOnStack OnWidgetAdded) const;
	
	UFUNCTION(BlueprintPure, Category = "Easy User Interface Management|Activatable Widget", meta = (Categories = "UI.Layer"))
	UCommonActivatableWidgetStack* GetWidgetStackFromRoot(FGameplayTag LayerTag) const;

public:
	UFUNCTION(BlueprintCallable, Category = "Easy User Interface Management|Notification", meta = (Categories = "UI.NotificationType"))
	void AddNotificationToPanel(FGameplayTag LayerTag, TSoftClassPtr<UEasyNotificationWidget> NotificationWidgetClass, const FEasyNotificationWidgetInfo& NotificationInfo, FOnNotificationAddedToPanel OnNotificationAdded);

public:
	UFUNCTION(BlueprintCallable, Category = "Easy User Interface Management|Prompt Question", meta = (Categories = "UI.Layer"))
	UEasyPromptQuestion* PushQuestionPromptToLayer(TSubclassOf<UEasyPromptQuestion> PromptWidgetClass, FGameplayTag StackLayer, const FPromptQuestionBodyInfo& InQuestionBodyInfo, FPromtResultEventDelegate OnPromptResult);
	
public:
	UFUNCTION(BlueprintCallable, Category = "Easy User Interface Management|Input")
	void SetCommonInputMode(APlayerController* PlayerController, ECommonInputMode CommonInputMode, EMouseCaptureMode MouseCaptureMode, bool bHideMouse);

	UFUNCTION(BlueprintPure, Category = "Easy User Interface Management|Input")
	ECommonInputMode GetCurrentCommonInputMode(const APlayerController* PlayerController) const;

public:
	UPROPERTY(Transient)
	TObjectPtr<UEasyUserInterfaceRootWidget> RootWidget;
};
