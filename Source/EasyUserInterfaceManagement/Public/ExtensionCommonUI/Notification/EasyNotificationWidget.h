// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "CommonUserWidget.h"
#include "CommonLazyImage.h"
#include "CommonLazyWidget.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "EasyNotificationWidget.generated.h"

class UCommonButtonClickActionBase;
class UEasyCommonButtonExtended;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPromptNotificationDismissed);

/**
 * Base class for notification icon content, which can be used to display custom icons in the notification widget.
 */
UCLASS(Abstract)
class EASYUSERINTERFACEMANAGEMENT_API UEasyNotificationIconContent : public UCommonUserWidget
{
	GENERATED_BODY()
};


USTRUCT(BlueprintType)
struct FEasyNotificationWidgetInfo
{
	GENERATED_BODY()
	/** Header text of the notification */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification")
	FText NotificationHeader;
	/** Body text of the notification */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification", meta=(MultiLine=true))
	FText NotificationBody;
	/** Icon to be displayed in the notification */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification", meta=(AllowedClasses="/Script/Engine.Texture2D,/Script/Engine.MaterialInterface"))
	TSoftObjectPtr<UObject> NotificationIcon;

	/** Optional widget to be displayed instead of (or together) the icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification")
	TSoftClassPtr<UEasyNotificationIconContent> NotificationIconWidgetClass;
	
	/** If set <= 0, the notification will not be automatically dismissed, instead developer should handle it dismissal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification")
	float NotificationDuration;

	/** Determines button visibility (Visible or Collapsed) and it's action (will be nothing if set to false) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification")
	bool bCanBeManuallyDismissed;
	
	FEasyNotificationWidgetInfo()
	{
		NotificationHeader = FText::GetEmpty();
		NotificationBody = FText::GetEmpty();
		NotificationIconWidgetClass = nullptr;
		NotificationDuration = 6.0f;
		NotificationIcon = nullptr;
		bCanBeManuallyDismissed = true;
	}
};

/**
 * Base class for notification widgets, which can be used to display various notifications in the game.
 */
UCLASS(Abstract)
class EASYUSERINTERFACEMANAGEMENT_API UEasyNotificationWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UEasyNotificationWidget();
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	
public:
	/** Optional Header Text widget bindings **/
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification")
	TObjectPtr<UCommonTextBlock> NotificationHeaderTextWidget;
	/** Optional Notification Body widget bindings **/
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification")
	TObjectPtr<UCommonRichTextBlock> NotificationBodyTextWidget;
	/** Optional Notification Icon widget bindings **/
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification")
	TObjectPtr<UCommonLazyImage> NotificationIconWidget;
	/** Optional Notification Icon Container widget bindings **/
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification")
	TObjectPtr<UCommonLazyWidget> NotificationIconContainerWidget;
	/** Optional Dismiss Button widget bindings that will dismiss the notification if exists **/
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification|Dismiss")
	TObjectPtr<UEasyCommonButtonExtended> DismissButtonWidget;
	
public:
	/** Information about the notification to be displayed **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompt Notification")
	FEasyNotificationWidgetInfo NotificationInfo;

	/**
	 * If set to true, the notification will be dismissed automatically after NotificationDuration,
	 * if set to false, developer should handle the dismissal using:
	 * OnPromptNotificationDismissed delegate or overriding the OnNotificationDismissed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss")
	bool bShouldAutoDismiss = true;
	/**
	 * Text to display on the dismiss button (if any)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss Button")
	FText DismissButtonText;

	/**
	 * Icon to display on the dismiss button (if any)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss Button")
	TSoftObjectPtr<UObject> DismissButtonImage;
	/**
	 * Actions to execute when the dismiss button is pressed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss Button")
	TObjectPtr<UCommonButtonClickActionBase> DismissButtonAction;
	/**
	 * If set to true, the dismiss button actions will only be executed when the button is pressed,
	 * if set to false, the actions will be executed whenever the notification is dismissed (either automatically or manually)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss Button")
	bool bExecuteDismissActionsOnlyOnButtonPress = false;
	/**
	 * Delegate called when the notification is dismissed (either automatically or manually)
	 */
	UPROPERTY(BlueprintAssignable, Category = "Prompt Notification")
	FOnPromptNotificationDismissed OnPromptNotificationDismissed;
	
public:
	/**
	 * Sets the notification info to be displayed in the widget and Updates the widget accordingly (even the lifetime).
	 * @param InNotificationInfo The notification info struct containing details about the notification.
	 */
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationInfo(const FEasyNotificationWidgetInfo& InNotificationInfo);
	/**
	 * Event called when the notification info is set, can be overridden in Blueprints for custom behavior.
	 * @param InNotificationInfo The notification info struct containing details about the notification.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Prompt Notification")
	void OnNotificationInfoSet_BP(const FEasyNotificationWidgetInfo& InNotificationInfo);
	virtual void OnNotificationInfoSet_BP_Implementation(const FEasyNotificationWidgetInfo& InNotificationInfo) {}

	/**
	 * Sets the notification header text.
	 * @param InNotificationHeader The header text to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationHeader(const FText& InNotificationHeader);
	/**
	 * Sets the notification body text.
	 * @param InNotificationBody The body text to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationBody(const FText& InNotificationBody);
	/**
	 * Sets the notification icon.
	 * @param InNotificationIcon The icon asset to set (can be a Texture2D or MaterialInterface).
	 */
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification", meta=(AllowedClasses="Texture2D, MaterialInterface"))
	void SetNotificationIcon(const TSoftObjectPtr<UObject>& InNotificationIcon);
	/**
	 * Sets the notification icon widget class.
	 * @param InNotificationIconWidgetClass The widget class to set for the notification icon.
	 */
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationIconWidgetClass(const TSoftClassPtr<UEasyNotificationIconContent>& InNotificationIconWidgetClass);
	/**
	 * Sets the notification lifetime duration.
	 * @param InNotificationDuration The duration in seconds for which the notification should be displayed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationLifeTime(float InNotificationDuration);
	/**
	 * Refreshes the widget to reflect any changes made to the FromData struct.
	 * @param FromData The notification info struct containing details about the notification.
	 */
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void RefreshWidget(FEasyNotificationWidgetInfo FromData);

	/**
	 * Dismisses the notification, either automatically or manually.
	 */
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void DismissNotification();

public:
	/**
	 * Event called when the notification is dismissed, can be overridden in Blueprints for custom behavior.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Prompt Notification")
	void OnNotificationDismissed();
	virtual void OnNotificationDismissed_Implementation() {};
	/**
	 * Retrieves the remaining time before the notification is dismissed automatically. Generally for cosmetic purposes.
	 * @param OutTimeRemaining - The remaining time in seconds before the notification is dismissed.
	 * @param Ratio - The ratio of remaining time to total duration (0.0 to 1.0).
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category = "Prompt Notification")
	bool GetTimeRemaining(float& OutTimeRemaining, float& Ratio) const;
	
private:
	UFUNCTION()
	void OnDismissButtonClicked();
	
private:
	float NotificationStartTime = -1.0f;
	FTimerHandle NotificationTimerHandle;
	bool bHasNotificationDismissedByButton = false;

#if WITH_EDITORONLY_DATA
protected:
	/**
	 * This will be used to debug the notification widget in the editor at design time.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Debug")
	FEasyNotificationWidgetInfo DebugNotificationInfo;
#endif
};
