// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonButtonClickActionBase.h"
#include "ButtonAction_Back.generated.h"

class UCommonActivatableWidget;
/**
 * Finds the first "UCommonActivatableWidgetStack" parent of the instigator widget and calls "DeactiveWidget" on it.
 */
UCLASS(DisplayName = "Back")
class EASYUSERINTERFACEMANAGEMENT_API UButtonAction_Back : public UCommonButtonClickActionBase
{
	GENERATED_BODY()

public:
	UButtonAction_Back(const FObjectInitializer& ObjectInitializer);
	
public:
	/**
	 * To find the parent widget to deactivate, this class will search for the first parent of type "UCommonActivatableWidget" in the widget hierarchy.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Action", meta = (AllowAbstract))
	TSubclassOf<UCommonActivatableWidget> FindParentToDeactivate;
	
public:
	virtual void ExecuteAction_Implementation(UWidget* InstigatorWidget) override;
};
