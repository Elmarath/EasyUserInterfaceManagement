// Copyright Elmarath Studio 2025 All Rights Reserved.

#include "ExtensionCommonUI/Foundation/EasyCommonButtonExtended.h"

#include "CommonActionWidget.h"
#include "EasyUserInterfaceManagement.h"
#include "ExtensionCommonUI/Foundation/ButtonActions/CommonButtonClickActionBase.h"

UEasyCommonButtonExtended::UEasyCommonButtonExtended()
{
	ButtonText = FText::FromString("Button Text");
	ButtonDisplayAsset = nullptr;
	ImageSizeOverride = FVector2D(32.0f, 32.0f);
}

void UEasyCommonButtonExtended::NativePreConstruct()
{
	Super::NativePreConstruct();

	RefreshButton();
}

void UEasyCommonButtonExtended::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	RefreshButton();
}

void UEasyCommonButtonExtended::NativeOnClicked()
{
	Super::NativeOnClicked();

	if (ButtonClickAction)
	{
		ButtonClickAction->ExecuteAction(this);
	}
}
UObject* UEasyCommonButtonExtended::GetMetadataWithID(FName MetadataID) const
{
	if (MetadataID.IsNone())
	{
		return nullptr;
	}

	if (MetadataMap.Contains(MetadataID))
	{
		return MetadataMap[MetadataID];
	}

	return nullptr;
}

void UEasyCommonButtonExtended::SetMetadataWithID(FName MetadataID, UObject* MetadataObject)
{
	if (MetadataID.IsNone())
	{
		return;
	}

	MetadataMap.Add(MetadataID, MetadataObject);
}

void UEasyCommonButtonExtended::RemoveMetadataWithID(FName MetadataID)
{
	if (MetadataID.IsNone())
	{
		return;
	}

	MetadataMap.Remove(MetadataID);
}

void UEasyCommonButtonExtended::NativeOnReleased()
{
	// Check if this was a hold interaction that didn't complete
	if (CurrentHoldProgress > 0.f && CurrentHoldProgress < 1.f)
	{
		OnHoldCancelled.Broadcast();
		OnHoldCancelled_BP();
	}
	Super::NativeOnReleased();
}

void UEasyCommonButtonExtended::RefreshButton()
{
	SetButtonText(ButtonText);
	SetButtonImageBrushFromLazyDisplayAsset(ButtonDisplayAsset, bMatchImageSizeByDefault);
}

void UEasyCommonButtonExtended::SetButtonText(const FText& InText)
{
	ButtonText = InText;
	if (ButtonTextWidget)
	{
		ButtonTextWidget->SetText(ButtonText);

		if (const TSubclassOf<UCommonTextStyle> TextStyle = GetCurrentTextStyleClass())
		{
			ButtonTextWidget->SetStyle(TextStyle);
		}

		if (ButtonText.IsEmpty())
		{
			ButtonTextWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			ButtonTextWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	if (bCollapseSelfIfContentEmpty)
	{
		SetVisibility(ButtonText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
}

void UEasyCommonButtonExtended::SetButtonImageBrushFromLazyDisplayAsset(const TSoftObjectPtr<UObject>& InDisplayAsset,
	bool bMatchImageSize)
{
	ButtonDisplayAsset = InDisplayAsset;
	bMatchImageSizeByDefault = bMatchImageSize;
	if (ButtonImageWidget)
	{
		ButtonImageWidget->SetBrushFromLazyDisplayAsset(ButtonDisplayAsset, bMatchImageSizeByDefault);

		if (!bMatchImageSizeByDefault)
		{
			ButtonImageWidget->SetDesiredSizeOverride(ImageSizeOverride);
		}

		if (ButtonDisplayAsset.IsNull())
		{
			ButtonImageWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			ButtonImageWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UEasyCommonButtonExtended::SetButtonImageSizeOverride(const FVector2D& InSize)
{
	ImageSizeOverride = InSize;
	if (ButtonImageWidget && !bMatchImageSizeByDefault)
	{
		ButtonImageWidget->SetDesiredSizeOverride(ImageSizeOverride);
	}
}
