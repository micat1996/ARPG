#include "ReinforceResultWnd.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Fonts/SlateFontInfo.h"

void UReinforceResultWnd::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Image_Line = Cast<UImage>(GetWidgetFromName(TEXT("Image_Line")));
	Text_Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Result")));
}


void UReinforceResultWnd::SetReinforceResult(EReinforceResult result)
{
	FSlateFontInfo newFont = Text_Result->Font;

	switch (result)
	{
	case EReinforceResult::RF_Success:
		Image_Line->SetColorAndOpacity(FLinearColor(0.02f, 0.25f, 0.0f, 0.4f));
		newFont.OutlineSettings.OutlineColor = FLinearColor(0.02f, 0.25f, 0.0f, 1.0f);
		Text_Result->SetText(FText::FromString(TEXT("Success!")));
		break;

	case EReinforceResult::RF_LackOfMoney:
		break;

	case EReinforceResult::RF_Fail:
		Image_Line->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.22f, 0.4f));
		newFont.OutlineSettings.OutlineColor = FLinearColor(1.0f, 0.0f, 0.22f, 1.0f);
		Text_Result->SetText(FText::FromString(TEXT("Fail...")));
		break;
	}

	Text_Result->SetFont(newFont);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle,
		[this]() { CloseButton->SetVisibility(ESlateVisibility::Visible); },
		1.0f, 
		false);
}