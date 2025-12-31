// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "ExtensionCommonUI/Foundation/ButtonActions/ButtonAction_Multiple.h"

UButtonAction_Multiple::UButtonAction_Multiple(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Actions = TArray<UCommonButtonClickActionBase*>();
}

void UButtonAction_Multiple::ExecuteAction_Implementation(UWidget* InstigatorWidget)
{
	Super::ExecuteAction_Implementation(InstigatorWidget);

	for (UCommonButtonClickActionBase* ButtonClickAction : Actions)
	{
		if (ButtonClickAction)
		{
			ButtonClickAction->ExecuteAction(InstigatorWidget);
		}
	}
}
