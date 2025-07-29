// Copyright Elmarath Studio 2025


#include "ExtensionCommonUI/Notification/EasyNotificationWidget.h"
#include "TimerManager.h"
#include "ExtensionCommonUI/Foundation/EasyCommonButtonExtended.h"
#include "ExtensionCommonUI/Foundation/ButtonActions/CommonButtonClickActionBase.h"

UEasyNotificationWidget::UEasyNotificationWidget()
{
	NotificationInfo = FEasyNotificationWidgetInfo();
	NotificationInfo.NotificationDuration = 6.0f; // Default duration
	NotificationInfo.bCanBeManuallyDismissed = true;

	DismissButtonText = FText::FromString("Dismiss");
	DismissButtonImage = nullptr;
	bShouldAutoDismiss = true;
	bExecuteDismissActionsOnlyOnButtonPress = false;
	bHasNotificationDismissedByButton = false;

#if WITH_EDITOR
	DebugNotificationInfo = FEasyNotificationWidgetInfo();
	DebugNotificationInfo.NotificationHeader = FText::FromString("Debug Header");
	DebugNotificationInfo.NotificationBody = FText::FromString("Debug Body That will be used to test the notification widget in the editor.");
	DebugNotificationInfo.NotificationIconWidgetClass = nullptr; // Set to a valid class in the editor
	DebugNotificationInfo.NotificationDuration = 6.0f;
	DebugNotificationInfo.NotificationIcon = nullptr; // Set to a valid texture in the editor
	DebugNotificationInfo.bCanBeManuallyDismissed = true;
#endif
}

void UEasyNotificationWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

#if WITH_EDITOR
	if (IsDesignTime())
	{
		FEasyNotificationWidgetInfo TestNotificationInfo = DebugNotificationInfo;
		RefreshWidget(DebugNotificationInfo);
	}
#endif
}

void UEasyNotificationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RefreshWidget(NotificationInfo);

	if (DismissButtonWidget)
	{
		DismissButtonWidget->OnClicked().AddUObject(this, &UEasyNotificationWidget::OnDismissButtonClicked);
	}
}

void UEasyNotificationWidget::SetNotificationInfo(const FEasyNotificationWidgetInfo& InNotificationInfo)
{
	NotificationInfo = InNotificationInfo;
	RefreshWidget(NotificationInfo);
	SetNotificationLifeTime(NotificationInfo.NotificationDuration);
	
	OnNotificationInfoSet_BP(InNotificationInfo);
}

void UEasyNotificationWidget::SetNotificationHeader(const FText& InNotificationHeader)
{
	NotificationInfo.NotificationHeader = InNotificationHeader;
	RefreshWidget(NotificationInfo);
}

void UEasyNotificationWidget::SetNotificationBody(const FText& InNotificationBody)
{
	NotificationInfo.NotificationBody = InNotificationBody;
	RefreshWidget(NotificationInfo);
}

void UEasyNotificationWidget::SetNotificationIcon(const TSoftObjectPtr<UObject>& InNotificationIcon)
{
	NotificationInfo.NotificationIcon = InNotificationIcon;
	RefreshWidget(NotificationInfo);
}

void UEasyNotificationWidget::SetNotificationIconWidgetClass(
	const TSoftClassPtr<UEasyNotificationIconContent>& InNotificationIconWidgetClass)
{
	NotificationInfo.NotificationIconWidgetClass = InNotificationIconWidgetClass;
	RefreshWidget(NotificationInfo);
}

void UEasyNotificationWidget::SetNotificationLifeTime(float InNotificationDuration)
{
	NotificationInfo.NotificationDuration = InNotificationDuration;

	NotificationStartTime = GetWorld()->GetTimeSeconds();
	
	// reset timer
	NotificationTimerHandle.Invalidate();
	if (NotificationInfo.NotificationDuration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(NotificationTimerHandle, this, &UEasyNotificationWidget::DismissNotification, NotificationInfo.NotificationDuration, false);
	}
}

void UEasyNotificationWidget::RefreshWidget(FEasyNotificationWidgetInfo FromData)
{
	if (NotificationHeaderTextWidget)
	{
		NotificationHeaderTextWidget->SetText(FromData.NotificationHeader);
	}
	if (NotificationBodyTextWidget)
	{
		NotificationBodyTextWidget->SetText(FromData.NotificationBody);
	}
	if (NotificationIconWidget)
	{
		NotificationIconWidget->SetBrushFromLazyDisplayAsset(FromData.NotificationIcon);

		if (FromData.NotificationIcon.IsValid())
		{
			NotificationIconWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			NotificationIconWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	if (DismissButtonWidget)
	{
		DismissButtonWidget->SetButtonText(DismissButtonText);
		DismissButtonWidget->SetButtonImageBrushFromLazyDisplayAsset(DismissButtonImage, DismissButtonWidget->bMatchImageSizeByDefault);

		if (FromData.bCanBeManuallyDismissed)
		{
			DismissButtonWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			DismissButtonWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	if (NotificationIconContainerWidget)
	{
		NotificationIconContainerWidget->SetLazyContent(FromData.NotificationIconWidgetClass);

		if (FromData.NotificationIconWidgetClass.IsValid())
		{
			NotificationIconContainerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			NotificationIconContainerWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UEasyNotificationWidget::DismissNotification()
{
	// execute action if it is set and the notification is not dismissed by button
	if (!bExecuteDismissActionsOnlyOnButtonPress && !bHasNotificationDismissedByButton)
	{
		if (DismissButtonAction)
		{
			DismissButtonAction->ExecuteAction(this);
		}
	}

	OnPromptNotificationDismissed.Broadcast();
	OnNotificationDismissed();

	if (bShouldAutoDismiss)
	{
		RemoveFromParent();
	}
}

bool UEasyNotificationWidget::GetTimeRemaining(float& OutTimeRemaining, float& Ratio) const
{
	if (NotificationInfo.NotificationDuration <= 0.0f)
	{
		return false;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimeElapsed = CurrentTime - NotificationStartTime;
	const float TimeRemaining = FMath::Max(NotificationInfo.NotificationDuration - TimeElapsed, 0.0f);

	OutTimeRemaining = TimeRemaining;
	Ratio = FMath::Clamp(TimeElapsed / NotificationInfo.NotificationDuration, 0.0f, 1.0f);

	return true;
}

void UEasyNotificationWidget::OnDismissButtonClicked()
{
	if (!NotificationInfo.bCanBeManuallyDismissed)
	{
		return;
	}
	
	if (DismissButtonAction)
	{
		DismissButtonAction->ExecuteAction(this);
	}

	bHasNotificationDismissedByButton = true;
	DismissNotification();
}
