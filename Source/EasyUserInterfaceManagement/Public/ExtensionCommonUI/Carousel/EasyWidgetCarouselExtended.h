// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonWidgetCarouselNavBar.h"
#include "EasyWidgetCarouselExtended.generated.h"

class UCommonButtonBase;
/**
 * Exposes some of the protected members of UCommonWidgetCarouselNavBar for easier access in Blueprints.
 */
UCLASS()
class EASYUSERINTERFACEMANAGEMENT_API UEasyWidgetCarouselExtended : public UCommonWidgetCarouselNavBar
{
	GENERATED_BODY()

public:
	/**
	 * Gets the navigation button at the specified index.
	 * @param Index The index of the button to retrieve.
	 * @return The navigation button at the specified index, or nullptr if the index is invalid.
	 */
	UFUNCTION(BlueprintPure, Category = "CommonWidgetCarouselExtended")
	UCommonButtonBase* GetNavButtonAtIndex(int32 Index) const
	{
		return Buttons.IsValidIndex(Index) ? Buttons[Index] : nullptr;
	}

	/**
	 * Gets all navigation buttons.
	 * @return An array of all navigation buttons.
	 */
	UFUNCTION(BlueprintPure, Category = "CommonWidgetCarouselExtended")
	TArray<UCommonButtonBase*> GetNavButtons() const
	{
		return Buttons;
	}
};
