#include "RootItemSlot.h"
#include "Engine/DataTable.h"

#include "Single/GameInstance/ARPGGameInstance.h"

#include "Structures/ItemInfo/ItemInfo.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

URootItemSlot::URootItemSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEMINFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEMINFO.Succeeded()) DT_ItemInfo = DT_ITEMINFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("RootItemSlot.cpp :: %d LINE :: DT_ITEMINFO is not Loaded!"), __LINE__); }
}

void URootItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Image_ItemSprite = Cast<UImage>(GetWidgetFromName(TEXT("Image_ItemSprite")));
	Text_ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemCount")));
}

void URootItemSlot::InitializeRootItem(FName itemCode, int32 itemCount)
{
	FString contextString;
	FItemInfo* itemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCode, contextString);

	UTexture2D* itemImage = Cast<UARPGGameInstance>(GetGameInstance())->
		GetStreamableManager()->LoadSynchronous<UTexture2D>(itemInfo->ItemSpritePath);

	Image_ItemSprite->SetBrushFromTexture(itemImage);
	Text_ItemCount->SetText(FText::FromString(FString::FromInt(itemCount)));
}

