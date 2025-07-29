// Copyright Elmarath Studio 2025

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
	UPROPERTY(BlueprintReadOnly, Category = "Prompt Question|Body", meta = (MultiLine = true), EditAnywhere)
	FText ConfirmButtonText;
	UPROPERTY(BlueprintReadOnly, Category = "Prompt Question|Body", meta = (MultiLine = true), EditAnywhere)
	FText CancelButtonText;
	UPROPERTY(BlueprintReadOnly, Category = "Prompt Question|Body", EditAnywhere, AdvancedDisplay)
	bool bDeactivateWidgetOnAnyResult;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prompt Question|Body")
	FPromptQuestionBodyInfo QuestionBodyInfo;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Prompt Question|Events")
	void RefreshWidget();
	
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Prompt Question|Events")
	void OnConfirmButtonClicked();
	virtual void OnConfirmButtonClicked_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Prompt Question|Events")
	void OnCancelButtonClicked();
	virtual void OnCancelButtonClicked_Implementation();
	
public:
	UFUNCTION()
	void OnPromptResultUpdated(EPromptQuestionResult Result);
	
	/** Event for when the user makes a selection */
	UPROPERTY(BlueprintAssignable, Category = "Prompt Question|Events")
	FOnPromtQuestionResult OnPromptQuestionResult;

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Prompt Question Widget Tree")
	TObjectPtr<UCommonTextBlock> QuestionHeaderTextWidget;
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Question Widget Tree")
	TObjectPtr<UCommonRichTextBlock> QuestionBodyTextWidget;
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Question Widget Tree")
	TObjectPtr<UEasyCommonButtonExtended> ConfirmButtonWidget;
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Prompt Question Widget Tree")
	TObjectPtr<UEasyCommonButtonExtended> CancelButtonWidget;

private:
	bool bHasInteractionCompleted;

private:
	void ExecutePromptQuestionResultExecutions(EPromptQuestionResult PromptQuestionResult);
};
