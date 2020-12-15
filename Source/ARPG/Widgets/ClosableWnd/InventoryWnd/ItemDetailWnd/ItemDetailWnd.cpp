#include "ItemDetailWnd.h"

void UItemDetailWnd::NativeOnInitialized()
{
	Super::NativeOnInitialized();


}

void UItemDetailWnd::NativeConstruct()
{
	Super::NativeConstruct();

	WndSize = FVector2D(400.0f, 600.0f);
	UpdateWndSize();
}
