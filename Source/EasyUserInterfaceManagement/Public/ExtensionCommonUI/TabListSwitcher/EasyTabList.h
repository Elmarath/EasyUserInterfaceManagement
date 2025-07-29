// Copyright Elmarath Studio 2025

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab", meta = (AllowedClasses = "Texture2D,MaterialInterface"))
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
	UFUNCTION(BlueprintCallable, Category = "Tab")
	bool RegisterTabDynamic(UWidget* ContentWidget, int32 TabIndex, FCommonTabListExtendedTabButtonInfo TabButtonInfo);

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tab")
	TSubclassOf<UEasyCommonButtonExtended> TabButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab")
	TSubclassOf<UCommonButtonStyle> TabButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab")
	TObjectPtr<UCommonAnimatedSwitcher> LinkedSwitcherOnScene;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab", meta = (TitleProperty = "TabButtonInfo"))
	TArray<FCommonTabListExtendedContentInfo> TabContentList;

public:
	UFUNCTION(BlueprintCallable, Category = "Tab")
	void BP_HandleNextTab();

	UFUNCTION(BlueprintCallable, Category = "Tab")
	void BP_HandlePreviousTab();

	UPROPERTY(EditDefaultsOnly, Category = "Tab")
	bool bIsPassThroughTabNavigation = true;

	UFUNCTION(BlueprintPure, Category = "Tab")
	UWidget* GetContentWidgetByTabName(FName TabNameID) const;

	UFUNCTION(BlueprintPure, Category = "Tab")
	int32 GetTabIndexByName(FName TabNameID) const;

protected:
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<USizeBox> PreviousTabContainerActionWrapper;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<USizeBox> NextTabContainerActionWrapper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab")
	FVector2D TabActionSize = FVector2D(48, 48);
	
	// Optional action button widgets
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<UPanelWidget> TabButtonContainer;
	
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<UCommonActionWidget> NextTabActionWidget;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "Tab")
	TObjectPtr<UCommonActionWidget> PreviousTabActionWidget;

public:
	UFUNCTION(BlueprintCallable, Category = "Tab")
	void SetTabButtonClass(TSubclassOf<UEasyCommonButtonExtended> InTabButtonClass);
	UFUNCTION(BlueprintCallable, Category = "Tab")
	void SetTabButtonStyle(TSubclassOf<UCommonButtonStyle> InTabButtonStyle);

	
protected:
	void HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	void HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;

private:
	void UpdateActionWrappers();
	void UpdateLinkedSwitcher();
};
