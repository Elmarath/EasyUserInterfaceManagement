// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "ExtensionCommonUI/TabListSwitcher/EasyTabList.h"
#include "CommonAnimatedSwitcher.h"
#include "Components/SizeBox.h"
#include "ExtensionCommonUI/Foundation/EasyCommonButtonExtended.h"

UEasyTabList::UEasyTabList(const FObjectInitializer& ObjectInitializer)
{
	TabActionSize = FVector2D(48.0f, 48.0f);
}

void UEasyTabList::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateActionWrappers();
	UpdateLinkedSwitcher();

	// Bind the optional action widgets if they exist
	if (NextTabActionWidget)
	{
		NextTabActionWidget->SetInputAction(NextTabInputActionData); // Set input action if using a data table
	}

	if (PreviousTabActionWidget)
	{
		PreviousTabActionWidget->SetInputAction(PreviousTabInputActionData); // Set input action if using a data table
	}
}

void UEasyTabList::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsDesignTime())
	{
		UpdateActionWrappers();
#if WITH_EDITOR
		DebugCreateTabsForDesigner();
#endif
	}
}

bool UEasyTabList::RegisterTabDynamic(UWidget* ContentWidget, int32 TabIndex,
                                      FCommonTabListExtendedTabButtonInfo TabButtonInfo)
{
	if (!TabButtonClass)
	{
		return false;
	}

	if (!ContentWidget || TabIndex < 0 || TabIndex > GetTabCount())
	{
		return false;
	}


	if (RegisterTab(TabButtonInfo.TabNameID, TabButtonClass, ContentWidget, TabIndex))
	{
		if (UEasyCommonButtonExtended* Button =
			Cast<UEasyCommonButtonExtended>(GetTabButtonBaseByID(TabButtonInfo.TabNameID)))
		{
			Button->SetButtonText(TabButtonInfo.TabDisplayName);
			Button->SetButtonImageBrushFromLazyDisplayAsset(TabButtonInfo.TabIconDisplayAsset,
			                                                Button->bMatchImageSizeByDefault);
			Button->SetStyle(TabButtonStyle);
			return true;
		}
	}
	return false;
}

UUserWidget* UEasyTabList::RegisterTabDynamicFromClass(TSubclassOf<UUserWidget> ContentWidgetClass, int32 TabIndex,
                                                       FCommonTabListExtendedTabButtonInfo TabButtonInfo)
{
	if (ContentWidgetClass)
	{
		UUserWidget* ContentWidget = CreateWidget<UUserWidget>(this, ContentWidgetClass);
		if (RegisterTabDynamic(ContentWidget, TabIndex, TabButtonInfo))
		{
			return ContentWidget;
		}
	}
	return nullptr;
}

void UEasyTabList::SetSelectionToZero()
{
	if (TabContentList.Num() < 2)
	{
		return;
	}

	SelectTabByID(TabContentList[1].TabButtonInfo.TabNameID);
	SelectTabByID(TabContentList[0].TabButtonInfo.TabNameID);
}
# if WITH_EDITOR
void UEasyTabList::DebugCreateTabsForDesigner()
{
	if (TabButtonContainer && TabButtonClass && TabButtonStyle)
	{
		TabButtonContainer->ClearChildren();

		for (int i = 0; i < DebugTabCount; i++)
		{
			FCommonTabListExtendedTabButtonInfo TabButtonInfo;
			TabButtonInfo.TabNameID = FName(*FString::Printf(TEXT("Tab%d"), i));
			TabButtonInfo.TabDisplayName = FText::FromString(FString::Printf(TEXT("Tab %d"), i));
			TabButtonInfo.TabIconDisplayAsset = nullptr;

			UEasyCommonButtonExtended* TabButton = CreateWidget<UEasyCommonButtonExtended>(this, TabButtonClass);
			TabButton->SetStyle(TabButtonStyle);
			TabButton->SetButtonText(TabButtonInfo.TabDisplayName);
			TabButton->SetButtonImageBrushFromLazyDisplayAsset(TabButtonInfo.TabIconDisplayAsset,
			                                                   TabButton->bMatchImageSizeByDefault);
			TabButton->SetStyle(TabButtonStyle);
			TabButtonContainer->AddChild(TabButton);
		}
	}
}
#endif

void UEasyTabList::BP_HandleNextTab()
{
	HandleNextTabInputAction(bIsPassThroughTabNavigation);
}

void UEasyTabList::BP_HandlePreviousTab()
{
	HandlePreviousTabInputAction(bIsPassThroughTabNavigation);
}

UWidget* UEasyTabList::GetContentWidgetByTabName(FName TabNameID) const
{
	// Check if the tab exists in the registered tabs map
	if (const FCommonRegisteredTabInfo* TabInfo = GetRegisteredTabsByID().Find(TabNameID))
	{
		return TabInfo->ContentInstance; // Return the associated widget
	}

	return nullptr; // Tab not found
}

int32 UEasyTabList::GetTabIndexByName(FName TabNameID) const
{
	// Check if the tab exists in the registered tabs map
	if (const FCommonRegisteredTabInfo* TabInfo = GetRegisteredTabsByID().Find(TabNameID))
	{
		return TabInfo->TabIndex; // Return the associated widget
	}

	return INDEX_NONE; // Tab not found
}

void UEasyTabList::SetTabButtonClass(TSubclassOf<UEasyCommonButtonExtended> InTabButtonClass)
{
	if (InTabButtonClass && InTabButtonClass->IsChildOf<UEasyCommonButtonExtended>())
	{
		TabButtonClass = InTabButtonClass;
	}

	if (TabButtonContainer)
	{
		TabButtonContainer->ClearChildren();
	}
}

void UEasyTabList::SetTabButtonStyle(TSubclassOf<UCommonButtonStyle> InTabButtonStyle)
{
	if (InTabButtonStyle && InTabButtonStyle->IsChildOf<UCommonButtonStyle>())
	{
		TabButtonStyle = InTabButtonStyle;
	}
}

void UEasyTabList::HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabCreation_Implementation(TabNameID, TabButton);

	if (TabButtonContainer)
	{
		TabButtonContainer->AddChild(TabButton);
	}

	// Ensure LinkedSwitcher is set
	if (!GetLinkedSwitcher())
	{
		return;
	}

	UWidget* ContentWidget = GetContentWidgetByTabName(TabNameID);
	if (!ContentWidget)
	{
		return;
	}

	// Check if the content widget is already in the switcher
	const TArray<UWidget*> LinkedSwitcherContents = GetLinkedSwitcher()->GetAllChildren();
	if (!LinkedSwitcherContents.Contains(ContentWidget))
	{
		GetLinkedSwitcher()->AddChild(ContentWidget);
	}
}

void UEasyTabList::HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabRemoval_Implementation(TabNameID, TabButton);

	if (TabButtonContainer)
	{
		TabButtonContainer->RemoveChild(TabButton);
	}
}

void UEasyTabList::UpdateActionWrappers()
{
	if (PreviousTabActionWidget && !PreviousTabInputActionData.IsNull() && PreviousTabContainerActionWrapper)
	{
		PreviousTabContainerActionWrapper->SetWidthOverride(TabActionSize.X);
		PreviousTabContainerActionWrapper->SetHeightOverride(TabActionSize.Y);
		PreviousTabContainerActionWrapper->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		if (IsValid(PreviousTabContainerActionWrapper))
		{
			PreviousTabContainerActionWrapper->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	if (NextTabActionWidget && !NextTabInputActionData.IsNull() && NextTabContainerActionWrapper)
	{
		NextTabContainerActionWrapper->SetWidthOverride(TabActionSize.X);
		NextTabContainerActionWrapper->SetHeightOverride(TabActionSize.Y);
		NextTabContainerActionWrapper->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		if (IsValid(NextTabContainerActionWrapper))
		{
			NextTabContainerActionWrapper->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UEasyTabList::UpdateLinkedSwitcher()
{
	if (LinkedSwitcherOnScene)
	{
		SetLinkedSwitcher(LinkedSwitcherOnScene);
		// get the children of the switcher
		TArray<UWidget*> SwitcherChildren = LinkedSwitcherOnScene->GetAllChildren();
		TArray<FCommonTabListExtendedContentInfo> WidgetsToBeAdded = TArray<FCommonTabListExtendedContentInfo>();

		for (FCommonTabListExtendedContentInfo TabContent : TabContentList)
		{
			if (SwitcherChildren.Contains(TabContent.TabContent))
			{
				WidgetsToBeAdded.Add(TabContent);
			}
		}

		LinkedSwitcherOnScene->ClearChildren();
		TabButtonContainer->ClearChildren();

		for (int i = 0; i < WidgetsToBeAdded.Num(); i++)
		{
			LinkedSwitcherOnScene->AddChild(TabContentList[i].TabContent);
			RegisterTabDynamic(TabContentList[i].TabContent, i, TabContentList[i].TabButtonInfo);
		}
	}
}
