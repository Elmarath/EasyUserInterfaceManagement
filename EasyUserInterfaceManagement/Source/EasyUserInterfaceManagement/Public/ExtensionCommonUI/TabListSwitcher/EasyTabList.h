// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "CommonActionWidget.h" // Include this for action widgets
#include "EasyTabList.generated.h"

class USizeBox;
class UCommonButtonStyle;
class UEasyCommonButtonExtended;

USTRUCT(BlueprintType)
struct FCommonTabListExtendedTabButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab")
	FName TabNameID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab")
	FText TabDisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab", meta = (AllowedClasses = "/Script/Engine.Texture2D,/Script/Engine.MaterialInterface"))
	TSoftObjectPtr<UObject> TabIconDisplayAsset;
};

USTRUCT(BlueprintType)
struct FCommonTabListExtendedContentInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab")
	TObjectPtr<UWidget> TabContent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab", meta = (TitleProperty = "TabDisplayName"))
	FCommonTabListExtendedTabButtonInfo TabButtonInfo;
};

/**
 * A tab list widget that extends functionality for managing tabs and their content on widget switchers.
 * Handles Input Actions for tab navigation and allows dynamic registration of tabs and their content.
 */
UCLASS()
class EASYUSERINTERFACEMANAGEMENT_API UEasyTabList : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	UEasyTabList(const FObjectInitializer& ObjectInitializer);

public:
	// Function to initialize action buttons
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	
public:
	/**
	 * Registers a tab dynamically with the specified content widget, tab index, and button info.
	 * @param ContentWidget - The widget to display when the tab is selected.
	 * @param TabIndex - The index at which to insert the tab.
	 * @param TabButtonInfo - Information about the tab button.
	 * @return True if the tab was registered successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tab")
	bool RegisterTabDynamic(UWidget* ContentWidget, int32 TabIndex, FCommonTabListExtendedTabButtonInfo TabButtonInfo);

	/**
	 * Registers a tab dynamically from a widget class with the specified tab index and button info.
	 * It creates an instance of the widget class and registers it as a tab.
	 * @param ContentWidgetClass - The class of the widget to create for the tab content.
	 * @param TabIndex - The index at which to insert the tab.
	 * @param TabButtonInfo - Information about the tab button.
	 * @return The created widget instance if registration was successful, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tab", meta = (DeterminesOutputType = "ContentWidgetClass"))
	UUserWidget* RegisterTabDynamicFromClass(TSubclassOf<UUserWidget> ContentWidgetClass, int32 TabIndex, FCommonTabListExtendedTabButtonInfo TabButtonInfo);

	/** Unsafe method, only safe if 0 and 1 indexes are valid */
	UFUNCTION(BlueprintCallable, Category = "Tab")
	void SetSelectionToZero();
	
#if WITH_EDITOR
	UFUNCTION()
	void DebugCreateTabsForDesigner();
#endif

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	int32 DebugTabCount = 3;
#endif
	
public:
	/* Tab button class to use for creating tab buttons */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tab")
	TSubclassOf<UEasyCommonButtonExtended> TabButtonClass;
	/* Tab button style to apply to created tab buttons */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab")
	TSubclassOf<UCommonButtonStyle> TabButtonStyle;
	/* The linked switcher that this tab list will control */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab")
	TObjectPtr<UCommonAnimatedSwitcher> LinkedSwitcherOnScene;
	/* List of tab content and their associated button info */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab", meta = (TitleProperty = "TabButtonInfo"))
	TArray<FCommonTabListExtendedContentInfo> TabContentList;

public:
	/** Handles the action to navigate to the next tab */
	UFUNCTION(BlueprintCallable, Category = "Tab")
	void BP_HandleNextTab();
	/** Handles the action to navigate to the previous tab */
	UFUNCTION(BlueprintCallable, Category = "Tab")
	void BP_HandlePreviousTab();
	/** Whether tab navigation input actions should be passed through to underlying widgets */
	UPROPERTY(EditDefaultsOnly, Category = "Tab")
	bool bIsPassThroughTabNavigation = true;
	/** Retrieves the content widget associated with the specified tab name ID */
	UFUNCTION(BlueprintPure, Category = "Tab")
	UWidget* GetContentWidgetByTabName(FName TabNameID) const;
	/** Retrieves the index of the tab associated with the specified tab name ID */
	UFUNCTION(BlueprintPure, Category = "Tab")
	int32 GetTabIndexByName(FName TabNameID) const;

protected:
	/** Optional size box wrappers for action buttons */
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<USizeBox> PreviousTabContainerActionWrapper;
	/** Optional size box wrappers for action buttons */
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<USizeBox> NextTabContainerActionWrapper;
	/** Input action data for navigating to the next tab (It resizes the SizeBox widgets) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab")
	FVector2D TabActionSize = FVector2D(48, 48);
	
	/** Optional container for tab buttons (Generally: Horizontal Box, Vertical box or StackBox) */
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<UPanelWidget> TabButtonContainer;
	/** Optional action button widgets to display next tab Input Action*/
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<UCommonActionWidget> NextTabActionWidget;
	/** Optional action button widgets to display previous tab Input Action*/
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<UCommonActionWidget> PreviousTabActionWidget;

public:
	/** Sets the tab button class used for creating tab buttons */
	UFUNCTION(BlueprintCallable, Category = "Tab")
	void SetTabButtonClass(TSubclassOf<UEasyCommonButtonExtended> InTabButtonClass);
	/** Sets the tab button style applied to created tab buttons */
	UFUNCTION(BlueprintCallable, Category = "Tab")
	void SetTabButtonStyle(TSubclassOf<UCommonButtonStyle> InTabButtonStyle);
	
protected:
	void HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	void HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;

private:
	/** Updates the visibility and size of action button wrappers based on the presence of action widgets and input action data */
	void UpdateActionWrappers();
	/** Updates the linked switcher based on the LinkedSwitcherOnScene property */
	void UpdateLinkedSwitcher();
};
