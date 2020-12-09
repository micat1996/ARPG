#include "DraggableWndTitle.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDraggableWndTitle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Close = Cast<UButton>(GetWidgetFromName(TEXT("Button_Close")));
	Text_Title = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Title")));
	Text_Title->TextDelegate.BindDynamic(this, &UDraggableWndTitle::UpdateTitleText);
}
