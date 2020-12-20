#include "ItemDetailWnd.h"

#include "Engine/DataTable.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"


#include "Single/GameInstance/ARPGGameInstance.h"

#include "Structures/ItemInfo/ItemInfo.h"


void UItemDetailWnd::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateWndSize(400.0f, 600.0f);

	Text_ItemName			= Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	Text_ItemDescription	= Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemDescription")));
	Text_ItemType			= Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemType")));
	Image_ItemSprite		= Cast<UImage>(GetWidgetFromName(TEXT("Image_ItemSprite")));

	// 유저에게 보여지긴 하지만, 마우스와 상호작용 할수 없는 상태로 설정합니다.
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UItemDetailWnd::UpdateDetailWnd(class UDataTable* dt_ItemInfo, FName itemCode)
{
	FString contextString;
	FItemInfo * itemInfo = dt_ItemInfo->FindRow<FItemInfo>(itemCode, contextString);

	Text_ItemName->SetText(itemInfo->ItemName);
	Text_ItemDescription->SetText(itemInfo->ItemDescription);

	
	switch (itemInfo->ItemType)
	{
	case EItemType::IT_EtCetera:
		Text_ItemType->SetText(FText::FromString(TEXT("기타 아이템")));
		break;
	case EItemType::IT_Consumption:
		Text_ItemType->SetText(FText::FromString(TEXT("소비 아이템")));
		break;
	}

	auto streamableManager = Cast<UARPGGameInstance>(GetGameInstance())->GetStreamableManager();
	Image_ItemSprite->SetBrushFromTexture(Cast<UTexture2D>(streamableManager->LoadSynchronous(itemInfo->ItemSpritePath)));
}
