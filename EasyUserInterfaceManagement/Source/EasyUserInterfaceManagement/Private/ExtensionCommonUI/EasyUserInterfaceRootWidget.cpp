// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "ExtensionCommonUI/EasyUserInterfaceRootWidget.h"

#include "EasyUserInterfaceManagement.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "ExtensionCommonUI/Notification/EasyNotificationWidget.h"
#include "ExtensionCommonUI/Notification/EasyNotificationWidgetPanelInterface.h"

UEasyUserInterfaceRootWidget::UEasyUserInterfaceRootWidget()
{
	WidgetStacks = TMap<FGameplayTag, UCommonActivatableWidgetStack*>();
	LoadingWidgetParams = FPushWidgetToStackAsyncParams();
}

void UEasyUserInterfaceRootWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEasyUserInterfaceRootWidget::RegisterWidgetStack(FGameplayTag LayerTag,
                                                       UCommonActivatableWidgetStack* WidgetClass)
{
	WidgetStacks.Add(LayerTag, WidgetClass);
}

void UEasyUserInterfaceRootWidget::PushWidgetToStackAsync(FGameplayTag LayerTag,
                                                          TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
                                                          FOnWidgetUpdatedOnStack OnWidgetAdded)
{
	if (!WidgetClass.IsValid() && !WidgetClass.ToSoftObjectPath().IsValid())
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("[%s] WidgetClass is invalid!"), *GetName());
		return;
	}

	UCommonActivatableWidgetStack** FoundStackPtr = WidgetStacks.Find(LayerTag);
	if (!FoundStackPtr || !(*FoundStackPtr))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("[%s] WidgetStack not found for LayerTag: %s"), *GetName(), *LayerTag.ToString());
		return;
	}

	if (LoadingWidgetParams == FPushWidgetToStackAsyncParams(LayerTag, WidgetClass))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Display, TEXT("[%s] Already loading widget for LayerTag: %s, skipping this request."), *GetName(), *LayerTag.ToString());
		return;
	}

	UCommonActivatableWidgetStack* WidgetStack = *FoundStackPtr;

	LoadingWidgetParams = FPushWidgetToStackAsyncParams(LayerTag, WidgetClass);
	
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(
		WidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateWeakLambda(
			this,
			[this, WidgetClass, WidgetStack, LayerTag, OnWidgetAdded]()
			{
				UClass* LoadedClass = WidgetClass.Get();
				if (!LoadedClass)
				{
					LoadingWidgetParams.Reset();
					UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("[%s] Widget class failed to load."), *GetName());
					return;
				}
				UCommonActivatableWidget* WidgetAdded = WidgetStack->AddWidget(LoadedClass);
				OnWidgetAdded.ExecuteIfBound(LayerTag, WidgetAdded);
				LoadingWidgetParams.Reset();
			}
		)
	);
}

UCommonActivatableWidgetStack* UEasyUserInterfaceRootWidget::GetWidgetStack(const FGameplayTag& LayerTag) const
{
	if (!WidgetStacks.Contains(LayerTag))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("GetWidgetStack: Layer not found"));
		return nullptr;
	}

	UCommonActivatableWidgetStack* WidgetStack = WidgetStacks[LayerTag];
	return WidgetStack;
}

void UEasyUserInterfaceRootWidget::RegisterNotificationPanel(FGameplayTag LayerTag, UWidget* ContainerWidget)
{
	if (!ContainerWidget)
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Registering Notification Panel: [%s] ContainerWidget is null!"), *GetName());
		return;
	}

	UWidget** FoundPanelPtr = NotificationPanels.Find(LayerTag);
	if (FoundPanelPtr && *FoundPanelPtr)
	{
		UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("[%s] Notification panel already registered for LayerTag: %s, Replacing the registered panel."), *GetName(), *LayerTag.ToString());
	}

	NotificationPanels.Add(LayerTag, ContainerWidget);
}

bool UEasyUserInterfaceRootWidget::UnRegisterNotificationPanel(const FGameplayTag& LayerTag)
{
	UWidget** FoundPanelPtr = NotificationPanels.Find(LayerTag);
	if (FoundPanelPtr && *FoundPanelPtr)
	{
		NotificationPanels.Remove(LayerTag);
		return true;
	}
	UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("[%s] No notification panel found for LayerTag: %s"), *GetName(), *LayerTag.ToString());
	return false;
}

UWidget* UEasyUserInterfaceRootWidget::GetNotificationPanel(const FGameplayTag& LayerTag) const
{
	if (!NotificationPanels.Contains(LayerTag))
	{
		return nullptr;
	}
	return NotificationPanels[LayerTag];
}

void UEasyUserInterfaceRootWidget::AddNotificationWidgetToPanel(FGameplayTag LayerTag,
	UEasyNotificationWidget* NotificationWidget, const FEasyNotificationWidgetInfo& InNotificationInfo)
{
	UWidget** FoundPanelPtr = NotificationPanels.Find(LayerTag);
	if (!FoundPanelPtr || !(*FoundPanelPtr))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("[%s] Notification panel not found for LayerTag: %s"), *GetName(), *LayerTag.ToString());
		return;
	}

	if (UWidget* NotificationPanel = *FoundPanelPtr)
	{
		if (NotificationPanel->GetClass()->ImplementsInterface(UEasyNotificationWidgetPanelInterface::StaticClass()))
		{
			IEasyNotificationWidgetPanelInterface::Execute_OnNotificationWidgetAdded(NotificationPanel, LayerTag, NotificationWidget);
			NotificationWidget->SetNotificationInfo(InNotificationInfo);
		}
		else
		{
			UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("[%s] Notification panel does not implement IEasyNotificationWidgetPanelInterface for LayerTag: %s"), *GetName(), *LayerTag.ToString());
		}
	}
	else
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("[%s] Notification panel is null for LayerTag: %s"), *GetName(), *LayerTag.ToString());
	}
}