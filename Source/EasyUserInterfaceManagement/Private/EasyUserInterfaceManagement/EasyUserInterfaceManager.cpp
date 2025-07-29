// Copyright Elmarath Studio 2025


#include "EasyUserInterfaceManagement/EasyUserInterfaceManager.h"

#include "EasyUserInterfaceManagement.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "ExtensionCommonUI/EasyUserInterfaceRootWidget.h"
#include "Input/CommonUIActionRouterBase.h"


void UEasyUserInterfaceManager::AddRootWidgetToViewport(TSoftClassPtr<UEasyUserInterfaceRootWidget> InRootWidget, int32 ZOrder)
{
	if (!InRootWidget.ToSoftObjectPath().IsValid())
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Invalid root widget class provided."));
		return;
	}

	if (IsValid(RootWidget))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("Root widget is already registered. Removing the existing one before adding a new one."));
		RootWidget->RemoveFromParent();
		return;
	}
	
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	TSharedPtr<FStreamableHandle> Handle = Streamable.RequestAsyncLoad(
		InRootWidget.ToSoftObjectPath(),
		FStreamableDelegate::CreateWeakLambda(
			this,
			[this, InRootWidget, ZOrder]()
			{
				// At this point, the asset is loaded; get the loaded class via the soft pointer.
				UClass* LoadedClass = InRootWidget.Get();
				if (LoadedClass)
				{
					// Create the widget instance
					RootWidget = CreateWidget<UEasyUserInterfaceRootWidget>(GetLocalPlayer()->GetPlayerController(GetWorld()), LoadedClass);
					RootWidget->AddToViewport(ZOrder);
					OnRootWidgetAddedToViewport.Broadcast(RootWidget);
				}
				else
				{
					UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Failed to load root widget class."));
				}
			}
		)
	);

	if (!Handle.IsValid())
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Failed to start async load for root widget class."));
	}
}

UEasyUserInterfaceRootWidget* UEasyUserInterfaceManager::GetRootWidget()
{
	return RootWidget;
}

void UEasyUserInterfaceManager::PushWidgetToStackAsync(FGameplayTag LayerTag,
	TSoftClassPtr<UCommonActivatableWidget> InWidget, FOnWidgetUpdatedOnStack OnWidgetAdded) const
{
	if (!RootWidget)
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Root widget is not registered. Cannot push widget to stack."));
		return;
	}
	
	RootWidget->PushWidgetToStackAsync(LayerTag, InWidget, OnWidgetAdded);
}

UCommonActivatableWidgetStack* UEasyUserInterfaceManager::GetWidgetStackFromRoot(FGameplayTag LayerTag) const
{
	if (!RootWidget)
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Root widget is not registered. Cannot get widget stack."));
		return nullptr;
	}

	if (UCommonActivatableWidgetStack* FoundStackPtr = RootWidget->GetWidgetStack(LayerTag))
	{
		return FoundStackPtr;
	}

	UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("Widget stack not found for LayerTag: %s"), *LayerTag.ToString());
	return nullptr;
}

void UEasyUserInterfaceManager::AddNotificationToPanel(
	FGameplayTag LayerTag,
	TSoftClassPtr<UEasyNotificationWidget> NotificationWidgetClass,
	const FEasyNotificationWidgetInfo& NotificationInfo,
	FOnNotificationAddedToPanel OnNotificationAdded)
{
	if (!RootWidget)
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Root widget is not registered. Cannot add notification to panel."));
		return;
	}

	// Make a copy of the soft class pointer so we can resolve it inside the lambda
	TSoftClassPtr<UEasyNotificationWidget> ClassToLoad = NotificationWidgetClass;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(
		ClassToLoad.ToSoftObjectPath(),
		FStreamableDelegate::CreateWeakLambda(this, [this, ClassToLoad, LayerTag, NotificationInfo, OnNotificationAdded]()
		{
			if (!IsValid(RootWidget))
			{
				UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Root widget is not registered. Cannot add notification to panel."));
				return;
			}

			UClass* LoadedClass = ClassToLoad.Get(); // Loaded class now available

			if (IsValid(LoadedClass))
			{
				UEasyNotificationWidget* NotificationWidget = CreateWidget<UEasyNotificationWidget>(GetLocalPlayer()->GetPlayerController(GetWorld()), LoadedClass);
				RootWidget->AddNotificationWidgetToPanel(LayerTag, NotificationWidget, NotificationInfo);
				OnNotificationAdded.ExecuteIfBound(LayerTag, NotificationWidget);
			}
			else
			{
				UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("Failed to load notification widget class for tag %s"), *LayerTag.ToString());
			}
		})
	);
}

UEasyPromptQuestion* UEasyUserInterfaceManager::PushQuestionPromptToLayer(
	TSubclassOf<UEasyPromptQuestion> PromptWidgetClass, FGameplayTag StackLayer,
	const FPromptQuestionBodyInfo& InQuestionBodyInfo, FPromtResultEventDelegate OnPromptResult)
{
	if (!IsValid(PromptWidgetClass))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Invalid PromptWidgetClass provided while pushing question prompt to layer."));
		return nullptr;
	}
	
	if (UEasyUserInterfaceRootWidget* CurrentRootWidget = GetRootWidget())
	{
		if (UCommonActivatableWidgetStack* Stack = CurrentRootWidget->GetWidgetStack(StackLayer))
		{
			if (IsValid(PromptWidgetClass))
			{
				UEasyPromptQuestion* PromptWidget = CreateWidget<UEasyPromptQuestion>(GetLocalPlayer()->GetPlayerController(GetWorld()), PromptWidgetClass);
				if (!PromptWidget)
				{
					UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Failed to create PromptWidget of class %s"), *PromptWidgetClass->GetName());
					return nullptr;
				}
				PromptWidget->QuestionBodyInfo = InQuestionBodyInfo;
				PromptWidget->RefreshWidget();

				PromptWidget->OnPromptQuestionResult.Add(OnPromptResult);

				Stack->AddWidgetInstance(*PromptWidget);
				
				return PromptWidget;
			}
		}
	}

	UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Failed to push question prompt to layer: %s. Stack or Root Widget not found."), *StackLayer.ToString());
	return nullptr;
}


void UEasyUserInterfaceManager::SetCommonInputMode(APlayerController* PlayerController, ECommonInputMode CommonInputMode, EMouseCaptureMode MouseCaptureMode, bool bHideMouse)
{
	check(PlayerController);

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot SetInputMode for non-local player [%s]"), *PlayerController->GetName());
		return;
	}

	UCommonUIActionRouterBase* ActionRouter = LocalPlayer->GetSubsystem<UCommonUIActionRouterBase>();
	if (!ActionRouter)
	{
		UE_LOG(LogTemp, Error, TEXT("CommonUIActionRouter is not available, cannot SetInputMode"));
		return;
	}

	FUIInputConfig InputConfig;
	if (CommonInputMode == ECommonInputMode::Game)
	{
		// Game mode means invisible mouse, permanently captured
		bool bHideCursorDuringViewportCapture = true;
		InputConfig = FUIInputConfig(CommonInputMode, EMouseCaptureMode::CapturePermanently, bHideCursorDuringViewportCapture);
	}
	else if (CommonInputMode == ECommonInputMode::All)
	{
		// Menu or All modes mean visible mouse, not permanently captured
		bool bHideCursorDuringViewportCapture = bHideMouse;
		InputConfig = FUIInputConfig(CommonInputMode, MouseCaptureMode, bHideCursorDuringViewportCapture);
	}
	else
	{
		// means we are in the MENU
		bool bHideCursorDuringViewportCapture = false;
		InputConfig = FUIInputConfig(CommonInputMode, EMouseCaptureMode::NoCapture, bHideCursorDuringViewportCapture);
	}

	ActionRouter->SetActiveUIInputConfig(InputConfig);
}

ECommonInputMode UEasyUserInterfaceManager::GetCurrentCommonInputMode(const APlayerController* PlayerController) const
{
	check(PlayerController);

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot GetInputMode for non-local player [%s]"), *PlayerController->GetName());
		return ECommonInputMode::All;
	}

	const UCommonUIActionRouterBase* ActionRouter = LocalPlayer->GetSubsystem<UCommonUIActionRouterBase>();
	if (!ActionRouter)
	{
		UE_LOG(LogTemp, Error, TEXT("CommonUIActionRouter is not available, cannot GetInputMode"));
		return ECommonInputMode::All;
	}

	return ActionRouter->GetActiveInputMode();
}
