// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "EasyCommonButtonExtended.generated.h"

class UCommonButtonClickActionBase;
/**
 * 
 */
UCLASS()
class EASYUSERINTERFACEMANAGEMENT_API UEasyCommonButtonExtended : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UEasyCommonButtonExtended();
	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;

public:
	virtual void NativeOnClicked() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meta Data", meta = (DisplayName = "Metadata"), Instanced)
	TMap<FName, TObjectPtr<UObject>> MetadataMap;
	
	UFUNCTION(BlueprintPure, Category = "Metadata")
	UObject* GetMetadataWithID(FName MetadataID) const;

	UFUNCTION(BlueprintCallable, Category = "Metadata")
	void SetMetadataWithID(FName MetadataID, UObject* MetadataObject);

	UFUNCTION(BlueprintCallable, Category = "Metadata")
	void RemoveMetadataWithID(FName MetadataID);

public:
	/** Actions To Execute when this button is clicked, this instance objects could be created in Blueprints to further enhance modularity*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", Instanced, meta = (DisplayName = "Button Click Action", ExposeOnSpawn))
	TObjectPtr<UCommonButtonClickActionBase> ButtonClickAction;

public:
	// Custom event for cancelled holds
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHoldCancelled);
	UPROPERTY(BlueprintAssignable, Category = "Button")
	FOnHoldCancelled OnHoldCancelled;
	UFUNCTION(BlueprintImplementableEvent, Category = "Button", meta = (DisplayName = "On Hold Cancelled"))
	void OnHoldCancelled_BP();
protected:
	virtual void NativeOnReleased() override;
	
public:
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Button")
	TObjectPtr<UCommonTextBlock> ButtonTextWidget;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadWrite, Category = "Button")
	TObjectPtr<UCommonLazyImage> ButtonImageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta=(ExposeOnSpawn))
	FText ButtonText;

	// should be a texture or a material
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta=(ExposeOnSpawn), meta = (AllowedClasses = "/Script/Engine.Texture2D,/Script/Engine.MaterialInterface"))
	TSoftObjectPtr<UObject> ButtonDisplayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta=(ExposeOnSpawn))
	bool bCollapseSelfIfContentEmpty = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (ExposeOnSpawn, InlineEditConditionToggle))
	bool bMatchImageSizeByDefault = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (ExposeOnSpawn, EditCondition = "!bMatchImageSizeByDefault"))
	FVector2D ImageSizeOverride;
	
	UFUNCTION(BlueprintCallable, Category = "Button")
	void RefreshButton();
	
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonText(const FText& InText);
	
	/**
	 * Sets the button image from a texture or material asset.
	 * @param InDisplayAsset The asset to use for the button image, can be a Texture2D or MaterialInterface.
	 * @param bMatchImageSize If true, the button image will match the size of the asset.
	 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonImageBrushFromLazyDisplayAsset(const TSoftObjectPtr<UObject>& InDisplayAsset, bool bMatchImageSize = false);

	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonImageSizeOverride(const FVector2D& InSize);
};
