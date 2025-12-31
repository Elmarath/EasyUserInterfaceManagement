// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "EasyUserInterfaceManagement/EasyCommonUIActionRouter.h"

#include "EasyUserInterfaceManagement.h"

TSharedRef<FCommonAnalogCursor> UEasyCommonUIActionRouter::MakeAnalogCursor() const
{
	return FEasyCommonAnalogCursor::CreateAnalogCursor<FEasyCommonAnalogCursor>(*this);
}

void UEasyCommonUIActionRouter::SetAnalogMovementEnabled(bool bInIsAnalogMovementEnabled)
{
	// first check if already enabled
	if (IsAnalogMovementEnabled() == bInIsAnalogMovementEnabled)
	{
		return;
	}
	
	if (FEasyCommonAnalogCursor* EasyAnalogCursor = static_cast<FEasyCommonAnalogCursor*>(AnalogCursor.Get()))
	{
		EasyAnalogCursor->SetAnalogMovementEnabled(bInIsAnalogMovementEnabled);
	}
	else
	{
		UE_LOG(LogEasyUserInterfaceManagement, Warning, TEXT("Failed to cast analog cursor to FEasyCommonAnalogCursor"));
	}
}

FEasyCommonAnalogCursor::FEasyCommonAnalogCursor(const UCommonUIActionRouterBase& InActionRouter) : FCommonAnalogCursor(InActionRouter)
{
	bIsAnalogMovementEnabled = false;
}

void FEasyCommonAnalogCursor::SetAnalogMovementEnabled(bool bInIsAnalogMovementEnabled)
{
	bIsAnalogMovementEnabled = bInIsAnalogMovementEnabled;
	RefreshCursorSettings();
	RefreshCursorVisibility();
	ShouldHandleRightAnalog(bIsAnalogMovementEnabled);
}