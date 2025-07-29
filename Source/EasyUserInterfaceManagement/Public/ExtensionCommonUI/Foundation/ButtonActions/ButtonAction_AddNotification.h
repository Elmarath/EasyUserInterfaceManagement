// Copyright Elmarath Studio 2025

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonClickActionBase.h"
#include "GameplayTagContainer.h"
#include "ExtensionCommonUI/Notification/EasyNotificationWidget.h"
#include "ButtonAction_AddNotification.generated.h"

/**
 * 
 */
UCLASS(DisplayName= "Add Notification")
class EASYUSERINTERFACEMANAGEMENT_API UButtonAction_AddNotification : public UCommonButtonClickActionBase
{
	GENERATED_BODY()

public:
	UButtonAction_AddNotification(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Add Notification", meta = (Categories = "UI.NotificationType"))
	FGameplayTag NotificationType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Add Notification")
	TSoftClassPtr<UEasyNotificationWidget> NotificationWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Add Notification")
	FEasyNotificationWidgetInfo NotificationInfo;

public:
	virtual void ExecuteAction_Implementation(UWidget* InstigatorWidget) override;
};
