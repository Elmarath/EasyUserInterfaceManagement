// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonClickActionBase.h"
#include "ButtonAction_ExecuteConsoleCommand.generated.h"

/**
 * Button Action that executes a console command when triggered.
 */
UCLASS(DisplayName="Execute Console Command")
class EASYUSERINTERFACEMANAGEMENT_API UButtonAction_ExecuteConsoleCommand : public UCommonButtonClickActionBase
{
	GENERATED_BODY()

public:
	UButtonAction_ExecuteConsoleCommand(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Console Command", meta = (ExposeOnSpawn))
	FString ConsoleCommand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Console Command", meta = (ExposeOnSpawn))
	bool bWriteToLog;
	
public:
	virtual void ExecuteAction_Implementation(UWidget* InstigatorWidget) override;
};
