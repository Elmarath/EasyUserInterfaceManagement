// Copyright Elmarath Studio 2025 All Rights Reserved.


#include "ExtensionCommonUI/PromptQuestion/EasyPromptQuestion.h"

#include "CommonRichTextBlock.h"
#include "EasyUserInterfaceManagement/EasyUserInterfaceManager.h"
#include "ExtensionCommonUI/Foundation/EasyCommonButtonExtended.h"
#include "ExtensionCommonUI/Foundation/ButtonActions/CommonButtonClickActionBase.h"

UEasyPromptQuestion::UEasyPromptQuestion(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bHasInteractionCompleted = false;
}

void UEasyPromptQuestion::NativePreConstruct()
{
	Super::NativePreConstruct();
#if WITH_EDITOR
	if (IsDesignTime())
	{
		RefreshWidget();
	}
#endif
}

void UEasyPromptQuestion::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshWidget();
}

void UEasyPromptQuestion::NativeDestruct()
{
	ExecutePromptQuestionResultExecutions(EPromptQuestionResult::PQR_None);
	OnPromptResultUpdated(EPromptQuestionResult::PQR_None);
	
	Super::NativeDestruct();
}

void UEasyPromptQuestion::NativeOnActivated()
{
	Super::NativeOnActivated();

	Super::NativeOnActivated();

	if (ConfirmButtonWidget)
	{
		ConfirmButtonWidget->OnClicked().AddUObject(this, &UEasyPromptQuestion::OnConfirmButtonClicked);
	}
	if (CancelButtonWidget)
	{
		CancelButtonWidget->OnClicked().AddUObject(this, &UEasyPromptQuestion::OnCancelButtonClicked);
	}
}

void UEasyPromptQuestion::NativeOnDeactivated()
{
	if (ConfirmButtonWidget && ConfirmButtonWidget->OnClicked().IsBound())
	{
		ConfirmButtonWidget->OnClicked().Clear();
	}

	if (CancelButtonWidget && CancelButtonWidget->OnClicked().IsBound())
	{
		CancelButtonWidget->OnClicked().Clear();
	}

	if (!bHasInteractionCompleted)
	{
		ExecutePromptQuestionResultExecutions(EPromptQuestionResult::PQR_NoUserInput);
		OnPromptResultUpdated(EPromptQuestionResult::PQR_NoUserInput);
	}
	
	Super::NativeOnDeactivated();
}

void UEasyPromptQuestion::RefreshWidget()
{
	if (QuestionHeaderTextWidget)
	{
		QuestionHeaderTextWidget->SetText(QuestionBodyInfo.QuestionHeaderText);
	}

	if (QuestionBodyTextWidget)
	{
		QuestionBodyTextWidget->SetText(QuestionBodyInfo.QuestionBodyText);
	}

	if (ConfirmButtonWidget)
	{
		ConfirmButtonWidget->SetButtonText(QuestionBodyInfo.ConfirmButtonText);
	}

	if (CancelButtonWidget)
	{
		CancelButtonWidget->SetButtonText(QuestionBodyInfo.CancelButtonText);
	}
}

void UEasyPromptQuestion::OnConfirmButtonClicked_Implementation()
{
	bHasInteractionCompleted = true;
	
	ExecutePromptQuestionResultExecutions(EPromptQuestionResult::PQR_Confirm);
	OnPromptResultUpdated(EPromptQuestionResult::PQR_Confirm);
}

void UEasyPromptQuestion::OnCancelButtonClicked_Implementation()
{
	bHasInteractionCompleted = true;

	ExecutePromptQuestionResultExecutions(EPromptQuestionResult::PQR_Cancel);
	OnPromptResultUpdated(EPromptQuestionResult::PQR_Cancel);
}

void UEasyPromptQuestion::OnPromptResultUpdated(EPromptQuestionResult Result)
{
	OnPromptQuestionResult.Broadcast(Result);
	
	if (QuestionBodyInfo.bDeactivateWidgetOnAnyResult)
	{
		if (bHasInteractionCompleted)
		{
			DeactivateWidget();	
		}
	}
}

void UEasyPromptQuestion::ExecutePromptQuestionResultExecutions(EPromptQuestionResult PromptQuestionResult)
{
	if (QuestionBodyInfo.PromptQuestionResultExecutions.Contains(PromptQuestionResult))
	{
		if (const TObjectPtr<UCommonButtonClickActionBase> ClickAction =
			QuestionBodyInfo.PromptQuestionResultExecutions.FindRef(PromptQuestionResult))
		{
			ClickAction->ExecuteAction(this);
		}
	}
}
