// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonClickActionBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ButtonAction_QuitGame.generated.h"

/**
 * Quit Game Button Action
 */
UCLASS(DisplayName = "Quit Game")
class EASYUSERINTERFACEMANAGEMENT_API UButtonAction_QuitGame : public UCommonButtonClickActionBase
{
	GENERATED_BODY()

public:
	UButtonAction_QuitGame(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quit Game")
	TEnumAsByte<EQuitPreference::Type> QuitPreference;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quit Game")
	bool bIgnorePlatformRestrictions;

public:
	virtual void ExecuteAction_Implementation(UWidget* InstigatorWidget) override;
};
