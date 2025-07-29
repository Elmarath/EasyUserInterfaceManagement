// Copyright Elmarath Studio 2025

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonAnalogCursor.h"
#include "EasyCommonUIActionRouter.generated.h"

/**
 * 
 */
UCLASS()
class EASYUSERINTERFACEMANAGEMENT_API UEasyCommonUIActionRouter : public UCommonUIActionRouterBase
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<FCommonAnalogCursor> MakeAnalogCursor() const override;

public:
	UFUNCTION(BlueprintCallable, Category = "EasyCommonUIActionRouterBase")
	virtual void SetAnalogMovementEnabled(bool bInIsAnalogMovementEnabled);

	UFUNCTION(BlueprintPure, Category = "EasyCommonUIActionRouterBase")
	virtual bool IsAnalogMovementEnabled() const { return GetCommonAnalogCursor()->IsAnalogMovementEnabled(); }
};

class EASYUSERINTERFACEMANAGEMENT_API FEasyCommonAnalogCursor : public FCommonAnalogCursor
{
public:
	FEasyCommonAnalogCursor(const UCommonUIActionRouterBase& InActionRouter);

	void SetAnalogMovementEnabled(bool bInIsAnalogMovementEnabled);
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
};