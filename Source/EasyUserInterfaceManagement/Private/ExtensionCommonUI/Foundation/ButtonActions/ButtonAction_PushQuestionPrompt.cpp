// Copyright Elmarath Studio 2025


#include "ExtensionCommonUI/Foundation/ButtonActions/ButtonAction_PushQuestionPrompt.h"

#include "EasyUserInterfaceManagement.h"
#include "EasyUserInterfaceManagement/EasyUserInterfaceManager.h"

UButtonAction_PushQuestionPrompt::UButtonAction_PushQuestionPrompt(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	QuestionWidgetClass = nullptr;
	RootStackLayer = FGameplayTag::EmptyTag;
	QuestionBodyInfo = FPromptQuestionBodyInfo();
}

void UButtonAction_PushQuestionPrompt::ExecuteAction_Implementation(UWidget* InstigatorWidget)
{
	Super::ExecuteAction_Implementation(InstigatorWidget);

	ULocalPlayer* LocalPlayer = InstigatorWidget->GetOwningLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		UE_LOG(LogEasyUserInterfaceManagement, Error, TEXT("Invalid LocalPlayer in ButtonAction_PushQuestionPrompt::ExecuteAction_Implementation"));
		return;
	}
	
	UEasyUserInterfaceManager* UserInterfaceManager = LocalPlayer->GetSubsystem<UEasyUserInterfaceManager>();

	if (IsValid(QuestionWidgetClass))
	{
		const FPromtResultEventDelegate OnPromptResultDelegate;
		UserInterfaceManager->PushQuestionPromptToLayer(QuestionWidgetClass, RootStackLayer, QuestionBodyInfo, OnPromptResultDelegate);
	}
}
