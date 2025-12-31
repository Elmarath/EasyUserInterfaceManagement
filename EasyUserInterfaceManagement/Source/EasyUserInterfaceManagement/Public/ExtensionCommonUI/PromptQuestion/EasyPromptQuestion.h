// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ExtensionCommonUI/ActivatableWidget/EasyActivatableWidgetBase.h"
#include "EasyPromptQuestion.generated.h"


class UCommonButtonClickActionBase;
class UCommonTextBlock;
class UCommonRichTextBlock;
class UEasyCommonButtonExtended;

UENUM(BlueprintType)
enum EPromptQuestionResult
{
	PQR_Confirm UMETA(DisplayName = "Confirm"),
	PQR_Cancel UMETA(DisplayName = "Cancel"),
	PQR_NoUserInput UMETA(DisplayName = "No User Input"),
	PQR_None UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FPromptQuestionBodyInfo
{
	GENERATED_BODY()
	
	/** Question Header Text */
	UPROPERTY(BlueprintReadOnly, Category = "Prompt Question|Body", meta = (MultiLine = true), EditAnywhere)
	FText QuestionHeaderText;
	/** Question Body Text */
	UPROPERTY(BlueprintReadOnly, Category = "Prompt Question|Body", meta = (MultiLine = true), EditAnywhere)
	FText QuestionBodyText;
	/** Question MetaData, can be used to store any kind of data */
	UPROPERTY(BlueprintReadOnly, Category = "Prompt Question|Body", meta = (MultiLine = true), Instanced, AdvancedDisplay, EditAnywhere)
	TMap<FName, TObjectPtr<UObject>> QuestionBodyMetaData;
	/** Specific Confirm Button Text */
	UPROPERTY(BlueprintReadOnly, Category = "Prompt Question|Body", meta = (MultiLine = true), EditAnywhere)
	FText ConfirmButtonText;
	/** Specific Cancel Button Text */
	UPROPERTY(BlueprintReadOnly, Category = "Prompt Question|Body", meta = (MultiLine = true), EditAnywhere)
	FText CancelButtonText;
	/** Whether to deactivate the widget on any result (Any Button Press) */
	UPROPERTY(BlueprintReadOnly, Category = "Prompt Question|Body", EditAnywhere, AdvancedDisplay)
	bool bDeactivateWidgetOnAnyResult;
	/** Map of Prompt Question Results to their corresponding execution actions */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category= "Prompt Question|Behaviour")
	TMap<TEnumAsByte<EPromptQuestionResult>, TObjectPtr<UCommonButtonClickActionBase>> PromptQuestionResultExecutions;

	FPromptQuestionBodyInfo()
	{
		QuestionHeaderText = FText::GetEmpty();
		QuestionBodyText = FText::GetEmpty();
		QuestionBodyMetaData = {};
		ConfirmButtonText = FText::GetEmpty();
		CancelButtonText = FText::GetEmpty();
		bDeactivateWidgetOnAnyResult = true;
		PromptQuestionResultExecutions = {};
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPromtQuestionResult, EPromptQuestionResult, Result);

/**
 * Base class for a prompt question widget that can be used to ask the user a question with confirm and cancel options.
 */
UCLASS(Abstract)
class EASYUSERINTERFACEMANAGEMENT_API UEasyPromptQuestion : public UEasyActivatableWidgetBase
{
	GENERATED_BODY()

public:
	UEasyPromptQuestion(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

public:
	/** Information about the prompt question body */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompt Question|Body")
	FPromptQuestionBodyInfo QuestionBodyInfo;
	
public:
	/**
	 * Refreshes the widget to reflect any changes made to the QuestionBodyInfo struct.
	 */
	UFUNCTION(BlueprintCallable, Category = "Prompt Question|Events")
	void RefreshWidget();
	
protected:
	/**
	 * Event called when the Confirm button is clicked.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Prompt Question|Events")
	void OnConfirmButtonClicked();
	virtual void OnConfirmButtonClicked_Implementation();

	/**
	 * Event called when the Cancel button is clicked.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Prompt Question|Events")
	void OnCancelButtonClicked();
	virtual void OnCancelButtonClicked_Implementation();
	
public:
	/**
	 * Internal event for when the prompt result is updated.
	 * @param Result The result of the prompt question.
	 */
	UFUNCTION()
	void OnPromptResultUpdated(EPromptQuestionResult Result);
	
	/** Event for when the user makes a selection */
	UPROPERTY(BlueprintAssignable, Category = "Prompt Question|Events")
	FOnPromtQuestionResult OnPromptQuestionResult;

public:
	/** Header Text Widget */
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Prompt Question Widget Tree")
	TObjectPtr<UCommonTextBlock> QuestionHeaderTextWidget;
	/** Question Body Rich Text Widget */
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Question Widget Tree")
	TObjectPtr<UCommonRichTextBlock> QuestionBodyTextWidget;
	/** Confirm Button Widget */
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Question Widget Tree")
	TObjectPtr<UEasyCommonButtonExtended> ConfirmButtonWidget;
	/** Cancel Button Widget */
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Question Widget Tree")
	TObjectPtr<UEasyCommonButtonExtended> CancelButtonWidget;

private:
	bool bHasInteractionCompleted;

private:
	void ExecutePromptQuestionResultExecutions(EPromptQuestionResult PromptQuestionResult);
};
