// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "EasyUserInterfaceManagement/EasyUserInterfaceHUDComponent.h"

#include "EasyUserInterfaceManagement.h"
#include "EasyUserInterfaceManagement/EasyUserInterfaceManager.h"
#include "GameFramework/HUD.h"


// Sets default values for this component's properties
UEasyUserInterfaceHUDComponent::UEasyUserInterfaceHUDComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	DefaultRootWidget = FRootWidgetInitializationParams();
}


// Called when the game starts
void UEasyUserInterfaceHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register the root widget to the Easy User Interface Manager
	RegisterRootWidgetToManager();
}

void UEasyUserInterfaceHUDComponent::RegisterRootWidgetToManager() const
{
	// Get the Easy User Interface Manager instance
	AHUD* OwnerHud = Cast<AHUD>(GetOwner());
	if (!IsValid(OwnerHud))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("EasyUserInterfaceHUDComponent's Owner Actor should be a HUD. Registering Root widget failed."));
		return;
	}
	FRootWidgetInitializationParams RootWidgetToAdd = GetRootWidgetInitializationParams_Implementation();
	OwnerHud->GetOwningPlayerController()->GetLocalPlayer()->GetSubsystem<UEasyUserInterfaceManager>()->AddRootWidgetToViewport(RootWidgetToAdd.RootWidgetClass, RootWidgetToAdd.ZOrder);
}

FRootWidgetInitializationParams UEasyUserInterfaceHUDComponent::GetRootWidgetInitializationParams_Implementation() const
{
	return DefaultRootWidget;
}

