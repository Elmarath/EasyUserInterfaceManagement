// Copyright Elmarath Studio 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonWidgetCarouselNavBar.h"
#include "EasyWidgetCarouselExtended.generated.h"

class UCommonButtonBase;
/**
 * 
 */
UCLASS()
class EASYUSERINTERFACEMANAGEMENT_API UEasyWidgetCarouselExtended : public UCommonWidgetCarouselNavBar
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "CommonWidgetCarouselExtended")
	UCommonButtonBase* GetNavButtonAtIndex(int32 Index) const
	{
		return Buttons.IsValidIndex(Index) ? Buttons[Index] : nullptr;
	}

	UFUNCTION(BlueprintPure, Category = "CommonWidgetCarouselExtended")
	TArray<UCommonButtonBase*> GetNavButtons() const
	{
		return Buttons;
	}
};
