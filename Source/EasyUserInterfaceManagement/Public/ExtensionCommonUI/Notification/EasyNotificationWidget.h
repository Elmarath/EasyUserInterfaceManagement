// Copyright Elmarath Studio 2025

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification")
	FText NotificationHeader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification", meta=(MultiLine=true))
	FText NotificationBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification", meta=(AllowedClasses="Texture2D, MaterialInterface"))
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
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification")
	TObjectPtr<UCommonTextBlock> NotificationHeaderTextWidget;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification")
	TObjectPtr<UCommonRichTextBlock> NotificationBodyTextWidget;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification")
	TObjectPtr<UCommonLazyImage> NotificationIconWidget;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification")
	TObjectPtr<UCommonLazyWidget> NotificationIconContainerWidget;
	
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Notification|Dismiss")
	TObjectPtr<UEasyCommonButtonExtended> DismissButtonWidget;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompt Notification")
	FEasyNotificationWidgetInfo NotificationInfo;

	/**
	 * If set to true, the notification will be dismissed automatically after NotificationDuration,
	 * if set to false, developer should handle the dismissal using:
	 * OnPromptNotificationDismissed delegate or overriding the OnNotificationDismissed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss")
	bool bShouldAutoDismiss = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss Button")
	FText DismissButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss Button")
	TSoftObjectPtr<UObject> DismissButtonImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss Button")
	TObjectPtr<UCommonButtonClickActionBase> DismissButtonAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt Notification|Dismiss Button")
	bool bExecuteDismissActionsOnlyOnButtonPress = false;
	
	UPROPERTY(BlueprintAssignable, Category = "Prompt Notification")
	FOnPromptNotificationDismissed OnPromptNotificationDismissed;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationInfo(const FEasyNotificationWidgetInfo& InNotificationInfo);

	UFUNCTION(BlueprintNativeEvent, Category = "Prompt Notification")
	void OnNotificationInfoSet_BP(const FEasyNotificationWidgetInfo& InNotificationInfo);
	virtual void OnNotificationInfoSet_BP_Implementation(const FEasyNotificationWidgetInfo& InNotificationInfo) {}
	
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationHeader(const FText& InNotificationHeader);

	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationBody(const FText& InNotificationBody);

	UFUNCTION(BlueprintCallable, Category = "Prompt Notification", meta=(AllowedClasses="Texture2D, MaterialInterface"))
	void SetNotificationIcon(const TSoftObjectPtr<UObject>& InNotificationIcon);

	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationIconWidgetClass(const TSoftClassPtr<UEasyNotificationIconContent>& InNotificationIconWidgetClass);
	
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void SetNotificationLifeTime(float InNotificationDuration);
	
	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void RefreshWidget(FEasyNotificationWidgetInfo FromData);

	UFUNCTION(BlueprintCallable, Category = "Prompt Notification")
	void DismissNotification();

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Prompt Notification")
	void OnNotificationDismissed();
	virtual void OnNotificationDismissed_Implementation() {};

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
