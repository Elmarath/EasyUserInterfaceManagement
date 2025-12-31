// Copyright Elmarath Studio 2025 All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "CommonButtonClickActionBase.h"
#include "GameplayTagContainer.h"
#include "ButtonAction_PushWidgetToStack.generated.h"

class UCommonActivatableWidget;
/**
 * Button Action to Push a Widget to a specified Widget Stack Layer in the Easy User Interface Management system.
 */
UCLASS(DisplayName = "Push Widget")
class EASYUSERINTERFACEMANAGEMENT_API UButtonAction_PushWidgetToStack : public UCommonButtonClickActionBase
{
	GENERATED_BODY()

public:
	UButtonAction_PushWidgetToStack(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Actions", meta = (Categories = "UI.Widget"))
	TSoftClassPtr<UCommonActivatableWidget> WidgetToPush;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Actions", meta = (Categories = "UI.Layer"))
	FGameplayTag LayerTag;

public:
	virtual void ExecuteAction_Implementation(UWidget* InstigatorWidget) override;
};
