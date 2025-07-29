// Copyright Elmarath Studio 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonButtonClickActionBase.generated.h"

class UWidget;
/**
 * Action base for button click events in the Easy User Interface Management system.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, ClassGroup = (UI), meta = (BlueprintSpawnableComponent), Abstract, CollapseCategories, meta= (ShowWorldContextPin))
class EASYUSERINTERFACEMANAGEMENT_API UCommonButtonClickActionBase : public UObject
{
	GENERATED_BODY()

public:
	UCommonButtonClickActionBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
	}
	
public:
	/**
	 *	Called when the button is clicked. Implement this function to add custom functionality to the button.
	 * 
	 * @param InstigatorWidget The parent widget that calls this function. (Generally the "Common Button" that calls this function)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Common Button Click Action")
	void ExecuteAction(UWidget* InstigatorWidget);
	virtual void ExecuteAction_Implementation(UWidget* InstigatorWidget)
	{
		
	}
};
