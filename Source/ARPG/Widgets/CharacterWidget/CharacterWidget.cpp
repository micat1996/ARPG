#include "CharacterWidget.h"
#include "Components/TextBlock.h"

void UCharacterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Name")));

}

void UCharacterWidget::SetNameText(FText value)
{
	Text_Name->SetText(value);
}
