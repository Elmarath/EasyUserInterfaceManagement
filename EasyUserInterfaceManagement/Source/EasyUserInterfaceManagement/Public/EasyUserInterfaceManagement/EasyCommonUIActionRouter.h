// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonAnalogCursor.h"
#include "EasyCommonUIActionRouter.generated.h"

/**
 * Common UI Action Router that extends functionality to manage analog cursor movement enabling/disabling.
 */
UCLASS()
class EASYUSERINTERFACEMANAGEMENT_API UEasyCommonUIActionRouter : public UCommonUIActionRouterBase
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<FCommonAnalogCursor> MakeAnalogCursor() const override;

public:
	/**
	 * Enables or disables analog cursor movement.
	 * @param bInIsAnalogMovementEnabled - Whether analog movement should be enabled.
	 */
	UFUNCTION(BlueprintCallable, Category = "EasyCommonUIActionRouterBase")
	virtual void SetAnalogMovementEnabled(bool bInIsAnalogMovementEnabled);
	/**
	 * Checks if analog cursor movement is enabled.
	 * @return True if analog movement is enabled, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "EasyCommonUIActionRouterBase")
	virtual bool IsAnalogMovementEnabled() const { return GetCommonAnalogCursor()->IsAnalogMovementEnabled(); }
};

class EASYUSERINTERFACEMANAGEMENT_API FEasyCommonAnalogCursor : public FCommonAnalogCursor
{
public:
	FEasyCommonAnalogCursor(const UCommonUIActionRouterBase& InActionRouter);

	void SetAnalogMovementEnabled(bool bInIsAnalogMovementEnabled);
};