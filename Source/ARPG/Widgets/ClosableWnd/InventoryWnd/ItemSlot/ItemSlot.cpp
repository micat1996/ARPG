#include "ItemSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/PlayerInventory/PlayerInventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Widgets/ClosableWnd/InventoryWnd/InventoryWnd.h"
#include "Widgets/ClosableWnd/InventoryWnd/ItemSlot/ItemSlotDragDropOperation.h"

#include "Single/GameInstance/ARPGGameInstance.h"

#include "Structures/ItemInfo/ItemInfo.h"



UItemSlot::UItemSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_ITEM_SLOT(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/InventoryWnd/BP_DragItemImage.BP_DragItemImage_C'"));
	if (BP_ITEM_SLOT.Succeeded()) ItemSlotWidgetClass = BP_ITEM_SLOT.Class;
	else { UE_LOG(LogTemp, Error, TEXT("ItemSlot.cpp :: %d LINE :: BP_ITEM_SLOT is not loaded"), __LINE__); }

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEM_INFO.Succeeded()) DT_ItemInfo = DT_ITEM_INFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("ItemSlot.cpp :: %d LINE :: DT_ITEM_INFO is not loaded"), __LINE__); }

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_EMPTY_TEXTURE(
		TEXT("Texture2D'/Game/Resources/UIImage/ItemSlot/empty.empty'"));
	if (T_EMPTY_TEXTURE.Succeeded()) EmptyTexture = T_EMPTY_TEXTURE.Object;
	else { UE_LOG(LogTemp, Error, TEXT("ItemSlot.cpp :: %d LINE :: T_EMPTY_TEXTURE is not loaded"), __LINE__); }

	NormalSlotColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	DraggingSlotColor = FLinearColor(0.15f, 0.15f, 0.15f, 1.f);
}


void UItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Image_ItemSprite = Cast<UImage>(GetWidgetFromName(TEXT("Image_ItemSprite")));
	Text_ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemCount")));
}

FReply UItemSlot::NativeOnMouseButtonDown(
	const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply retVal = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


	if (GetItemSlotInfo().IsEmpty())
		return retVal;
	else
	{
		// �巡�� �������� �����մϴ�.
		Image_ItemSprite->SetBrushTintColor(DraggingSlotColor);

		// �巡�� �� ��� �۾��� �����ϸ�, �۾� ����� ��ȯ�մϴ�.
		return UWidgetBlueprintLibrary::DetectDragIfPressed(
			InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
}

void UItemSlot::NativeOnDragDetected(
	const FGeometry& InGeometry, 
	const FPointerEvent& InMouseEvent, 
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// �巡�� �� ��� �۾� ��ü�� �����մϴ�.
	UItemSlotDragDropOperation* dragDropOp = Cast<UItemSlotDragDropOperation>(
		UWidgetBlueprintLibrary::CreateDragDropOperation(UItemSlotDragDropOperation::StaticClass()));

	if (IsValid(dragDropOp))
	{
		// �ǹ��� �����մϴ�.
		dragDropOp->Pivot = EDragPivot::CenterCenter;

		// �巡�� �� ����� ������ ������ �����մϴ�.
		UUserWidget* dragVisual = CreateWidget<UUserWidget>(this, ItemSlotWidgetClass);

		// �巡�� �� ����� ������ ������ �����մϴ�.
		dragDropOp->DefaultDragVisual = dragVisual;

		// �巡�� �� ����� ������ �̹����� �����մϴ�.
		Cast<UImage>(dragVisual->GetWidgetFromName(TEXT("Image_DragItem")))->SetBrushFromTexture(
			Cast<UTexture2D>(Image_ItemSprite->Brush.GetResourceObject()) );

		// �巡���� ������ ������ �����մϴ�.
		dragDropOp->DraggingSlot = this;
	}

	// ��¿� �Ű� ������ �巡�� �� ��� �۾� ��ü�� �����մϴ�.
	OutOperation = dragDropOp;
}

void UItemSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	// �⺻ �������� �����մϴ�.
	Image_ItemSprite->SetBrushTintColor(NormalSlotColor);
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool retVal = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemSlotDragDropOperation* dragDropOp = Cast<UItemSlotDragDropOperation>(InOperation);
	UPlayerInventoryComponent* playerInventory = InventoryWnd->GetPlayerInventoryComponent();

	playerInventory->SwapItem(this, dragDropOp->DraggingSlot);

	return retVal;
}

void UItemSlot::UpdateItemSlot()
{
	FItemSlotInfo itemSlotInfo = GetItemSlotInfo();

	if (itemSlotInfo.IsEmpty())
	{
		Image_ItemSprite->SetBrushFromTexture(EmptyTexture);
		Text_ItemCount->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		FString contextString;
		FItemInfo * itemInfo = DT_ItemInfo->FindRow<FItemInfo>(
			itemSlotInfo.ItemCode, contextString);

		// ������ �̹����� �ε��մϴ�.
		UARPGGameInstance* gameInst = Cast<UARPGGameInstance>(GetGameInstance());
		UTexture2D * itemImage = Cast<UTexture2D>(gameInst->GetStreamableManager()->
			LoadSynchronous(itemInfo->ItemSpritePath));

		// �̹����� �����մϴ�.
		Image_ItemSprite->SetBrushFromTexture(itemImage);

		// �⺻ �������� �����մϴ�.
		Image_ItemSprite->SetBrushTintColor(NormalSlotColor);

		Text_ItemCount->SetText(FText::FromString(
			FString::FromInt(itemSlotInfo.ItemCount) ));
	}
}

FItemSlotInfo UItemSlot::GetItemSlotInfo()
{
	FItemSlotInfo slotInfo = InventoryWnd->GetPlayerInventoryComponent()->
		GetInventoryItems()[InventorySlotIndex];

	return slotInfo;
}

void UItemSlot::SetItemSlotInfo(FItemSlotInfo itemInfo)
{
	InventoryWnd->GetPlayerInventoryComponent()->
		SetItemSlotInfo(InventorySlotIndex, itemInfo);
}
