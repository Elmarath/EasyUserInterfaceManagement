// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonClickActionBase.h"
#include "ExtensionCommonUI/PromptQuestion/EasyPromptQuestion.h"
#include "ButtonAction_PushQuestionPrompt.generated.h"

/**
 * Button Action to Push a Question Prompt Widget onto the specified Root Stack Layer.
 */
UCLASS(DisplayName= "Push Question Prompt")
class EASYUSERINTERFACEMANAGEMENT_API UButtonAction_PushQuestionPrompt : public UCommonButtonClickActionBase
{
	GENERATED_BODY()

public:
	UButtonAction_PushQuestionPrompt(const FObjectInitializer& ObjectInitializer);

public:
	virtual void ExecuteAction_Implementation(UWidget* InstigatorWidget) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Push Question Widget")
	TSubclassOf<UEasyPromptQuestion> QuestionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Push Question Widget", meta = (Categories = "UI.Layer"))
	FGameplayTag RootStackLayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Push Question Widget")
	FPromptQuestionBodyInfo QuestionBodyInfo;
};
