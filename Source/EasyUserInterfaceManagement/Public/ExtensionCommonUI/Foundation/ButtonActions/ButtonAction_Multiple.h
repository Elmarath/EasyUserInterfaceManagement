// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonClickActionBase.h"
#include "ButtonAction_Multiple.generated.h"

/**
 * Multiple button action that executes a list of actions when triggered. 0 to last.
 */
UCLASS(DisplayName="Multiple")
class EASYUSERINTERFACEMANAGEMENT_API UButtonAction_Multiple : public UCommonButtonClickActionBase
{
	GENERATED_BODY()

public:
	UButtonAction_Multiple(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Action", Instanced)
	TArray<TObjectPtr<UCommonButtonClickActionBase>> Actions;
	
public:
	virtual void ExecuteAction_Implementation(UWidget* InstigatorWidget) override;
};
