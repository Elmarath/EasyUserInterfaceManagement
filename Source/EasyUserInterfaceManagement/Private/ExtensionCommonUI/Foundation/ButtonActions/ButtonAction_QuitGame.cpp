// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "ExtensionCommonUI/Foundation/ButtonActions/ButtonAction_QuitGame.h"

#include "EasyUserInterfaceManagement.h"
#include "Components/Widget.h"
#include "Kismet/KismetSystemLibrary.h"

UButtonAction_QuitGame::UButtonAction_QuitGame(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	QuitPreference = EQuitPreference::Quit;
	bIgnorePlatformRestrictions = false;
}

void UButtonAction_QuitGame::ExecuteAction_Implementation(UWidget* InstigatorWidget)
{
	Super::ExecuteAction_Implementation(InstigatorWidget);

	if (!InstigatorWidget)
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("ButtonAction_QuitGame::OnButtonClicked_Implementation: InstigatorWidget is nullptr!"));
		return;
	}
	
	APlayerController* OwningPlayer = InstigatorWidget->GetOwningPlayer();
	
	UKismetSystemLibrary::QuitGame(OwningPlayer, OwningPlayer, QuitPreference, bIgnorePlatformRestrictions);
}
