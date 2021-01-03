#include "PlayerInventoryComponent.h"
#include "Engine/DataTable.h"

#include "Structures/EquipItemInfo/EquipItemInfo.h"
#include "Structures/ItemInfo/ItemInfo.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"

#include "Widgets/ClosableWnd/InventoryWnd/InventoryWnd.h"
#include "Widgets/ClosableWnd/InventoryWnd/ItemSlot/ItemSlot.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"




UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UInventoryWnd> BP_INVENTORY(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/InventoryWnd/BP_Inventory.BP_Inventory_C'"));
	if (BP_INVENTORY.Succeeded()) BPInventoryWndClass = BP_INVENTORY.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable>  DT_EQUIP_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EquipItemInfo.DT_EquipItemInfo'"));
	if (DT_EQUIP_ITEM_INFO.Succeeded()) DTEquipItemInfo = DT_EQUIP_ITEM_INFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("PlayerInventoryComponent.cpp :: %d LINE :: DT_EQUIP_ITEM_INFO is not loaded!"), __LINE__); }

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEM_INFO.Succeeded()) DTItemInfo = DT_ITEM_INFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("PlayerInventoryComponent.cpp :: %d LINE :: DT_ITEM_INFO is not loaded!"), __LINE__); }

}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();


	auto gameInst = GetGameInst();
	if (!IsValid(gameInst)) return;
	if (!gameInst->bIsLevelLoaded) return;

	InitializeEquipItems();
	InitializeInventoryItems();


	auto playerController = GetManager(UPlayerManager)->GetPlayerController();
	if (!IsValid(playerController)) return;

	ClosableWndController = playerController->GetClosableWndControllerComponent();
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerInventoryComponent::InitializeEquipItems()
{
	EquipItems.Empty();
	EquipItems.Add(EEquipItemType::EI_Head,		TEXT("10000"));
	EquipItems.Add(EEquipItemType::EI_Mask,		TEXT("11000"));
	EquipItems.Add(EEquipItemType::EI_Top,		TEXT("12000"));
	EquipItems.Add(EEquipItemType::EI_Bottom,	TEXT("13000"));
	EquipItems.Add(EEquipItemType::EI_Hand,		TEXT("14000"));
	EquipItems.Add(EEquipItemType::EI_Foot,		TEXT("15000"));
	EquipItems.Add(EEquipItemType::EI_Weapon,	TEXT("16000"));
}

void UPlayerInventoryComponent::InitializeInventoryItems()
{
	UPlayerManager* playerManager = GetManager(UPlayerManager);
	FPlayerInfo* playerInfo = playerManager->GetPlayerInfo();



	for (int32 i = 0; i < playerInfo->InventorySlotCount; ++i)
		InventoryItems.Add(FItemSlotInfo());

	// -- TEST CODE --
	AddItem(FName(TEXT("20000")), 11);
	AddItem(FName(TEXT("20001")), 12);
	AddItem(FName(TEXT("20002")), 13);
	AddItem(FName(TEXT("20003")), 14);
	AddItem(FName(TEXT("20004")), 15);
	AddItem(FName(TEXT("20005")), 16);

	RemoveItem(0, 6);
	RemoveItem(1, 100);
}

void UPlayerInventoryComponent::OpenInventory()
{
	if (!IsValid(ClosableWndController)) return;

	// �κ��丮 â ������ �����մϴ�.
	InventoryWnd = ClosableWndController->AddWnd<UInventoryWnd>(BPInventoryWndClass);

	// �κ��丮 ������Ʈ�� �����մϴ�.
	InventoryWnd->InitializeInventoryWnd(this);

}

void UPlayerInventoryComponent::CloseInventory()
{
	ClosableWndController->CloseWnd(false, InventoryWnd);

	InventoryWnd = nullptr;
}

void UPlayerInventoryComponent::UpdateEquipItemMesh(EEquipItemType equipitemType,
	class USkeletalMeshComponent* refSkMeshToUpdate,
	class USkeletalMeshComponent* refSetSkMeshToUpdate)
{
	// �������� ������ �ڵ带 �����մϴ�.
	FName itemCode = EquipItems[equipitemType];

	// ������ �ڵ带 �̿��Ͽ� ������ ������ ����ϴ�.
	FString contextString;
	FEquipItemInfo* equipItemInfo = DTEquipItemInfo->FindRow<FEquipItemInfo>(
		itemCode, contextString);

	// SkMesh ��ΰ� ����ٸ� ���޵� SkMesh Component �� ǥ���ϴ� Mesh �� ���ϴ�.
	if (equipItemInfo->SKMeshPath.ToString().IsEmpty())
	{
		refSkMeshToUpdate->SetSkeletalMesh(nullptr);

		if (equipitemType == EEquipItemType::EI_Hand)
			refSetSkMeshToUpdate->SetSkeletalMesh(nullptr);
	}

	// �ƴ϶�� ǥ���ϴ� Mesh �� �����մϴ�.
	else
	{
		// GameInstance
		UARPGGameInstance* gameInst = Cast<UARPGGameInstance>(GetWorld()->GetGameInstance());
		USkeletalMesh * itemMesh = Cast<USkeletalMesh>(gameInst->GetStreamableManager()->LoadSynchronous(
			equipItemInfo->SKMeshPath));

		// ��� Ÿ���� �� Ÿ���̶��
		if (equipitemType == EEquipItemType::EI_Hand)
		{
			auto setMesh = Cast<USkeletalMesh>(
				gameInst->GetStreamableManager()->LoadSynchronous(
					equipItemInfo->SKMeshSetPath));

			refSetSkMeshToUpdate->SetSkeletalMesh(setMesh);
		}
		
		refSkMeshToUpdate->SetSkeletalMesh(itemMesh);
	}
}

EReinforceResult UPlayerInventoryComponent::TryReinforce(EEquipItemType itemType)
{
	// ���� �����Ǿ��ִ� ������ ������ ����ϴ�.
	FString contextStirng;
	auto equipItemInfo = DTEquipItemInfo->FindRow<FEquipItemInfo>(GetEquippedItemCode(itemType), contextStirng);

	// ���� ������ ������ ����ϴ�.
	auto nextEquipItemInfo = DTEquipItemInfo->FindRow<FEquipItemInfo>(
		equipItemInfo->NextReinforcementItemCode, contextStirng);


	// ��ȭ�� �����ߴٸ�
	if (nextEquipItemInfo->ReinforcementProbability >= FMath::FRandRange(1.0f, 100.0f))
	{
		// �������� �������� ��ȭ�� ���������� �����մϴ�.
		EquipItems[itemType] = nextEquipItemInfo->ItemCode;
		//EquipItems[itemType] = equipItemInfo->NextReinforcementItemCode;

		// ��ȭ ��� ��ȯ
		return EReinforceResult::RF_Success;
	}


	// ��ȭ ��� ��ȯ
	return EReinforceResult::RF_Fail;
}

void UPlayerInventoryComponent::SwapItem(
	UItemSlot* firstItemSlot, UItemSlot* secondItemSlot)
{
	auto tempItemInfo = firstItemSlot->GetItemSlotInfo();
	firstItemSlot->SetItemSlotInfo(secondItemSlot->GetItemSlotInfo());
	secondItemSlot->SetItemSlotInfo(tempItemInfo);

	firstItemSlot->UpdateItemSlot();
	secondItemSlot->UpdateItemSlot();
}

bool UPlayerInventoryComponent::AddItem(FName itemCode, int32 itemCount)
{
	// �߰��Ϸ��� ������ ������ �����մϴ�.
	FString contextString;
	FItemInfo* itemInfo = DTItemInfo->FindRow<FItemInfo>(itemCode, contextString);

	// �߰��Ϸ��� �����۰� ������ �������� ��� ���� �ε����� ������ ����
	int32 slotIndex = INDEX_NONE;

	// ���ο� �������� ������ �� �ִ� �� ���� �ε����� ����Ȱ ����
	int32 emptySlotIndex = INDEX_NONE;

	// �迭���� ������ ������ ���԰� �� ������ Ž���մϴ�.
	for (int32 i = 0; i < InventoryItems.Num(); ++i)
	{
		// �߰��Ϸ��� �����۰� ������ �������� �����Ѵٸ�
		if (InventoryItems[i].ItemCode == itemCode)
		{
			if ((InventoryItems[i].ItemCount + itemCount) <= itemInfo->MaxCount)
			{
				slotIndex = i;

				// �߰��� �� �ִ� ���� �ε����� ã�����Ƿ�, Ž���� ����
				break;
			}
		}

		// �� ������ ã�Ҵٸ�
		if (emptySlotIndex == INDEX_NONE && InventoryItems[i].IsEmpty())
		{
			// �� ���� �ε��� ����
			emptySlotIndex = i;
		}
	}

	// �������� �߰��� �� �ִ� ������ �������� �ʴ´ٸ�
	if (slotIndex == INDEX_NONE && emptySlotIndex == INDEX_NONE)
		// ������ �߰� ����
		return false;

	// ������ �������� ���� ������ �����Ѵٸ�
	if (slotIndex != INDEX_NONE)
	{
		// ã�� ������ ������ ������ ������ŵ�ϴ�.
		InventoryItems[slotIndex].AddItemCount(itemCount);
	}

	// ������ �������� ���� ������ �������� ������, �� ������ ã�Ҵٸ�
	else if (emptySlotIndex != INDEX_NONE)
	{
		// ã�� ���Կ� �������� �߰��մϴ�.
		InventoryItems[emptySlotIndex] = FItemSlotInfo(itemCode, itemCount);
	}

	if (OnInventorySlotChanged.IsBound())
		OnInventorySlotChanged.Broadcast();

	// ������ �߰� ����
	return true;
}

bool UPlayerInventoryComponent::RemoveItem(int32 slotIndex, int32 removeCount)
{
	// slotIndex �� InventoryItems �迭 ������ �ʰ��Ѵٸ�
	if (InventoryItems.Num() <= slotIndex)

		// ���� ����
		return false;

	// ������ ������ ����ִٸ�
	if (InventoryItems[slotIndex].IsEmpty())

		// ���� ����
		return false;

	// ������ ���� ����
	InventoryItems[slotIndex].ItemCount -= removeCount;

	// ���� �������� ������ 0 ���϶�� ������ ���ϴ�.
	if (InventoryItems[slotIndex].ItemCount <= 0)
		InventoryItems[slotIndex] = FItemSlotInfo();

	if (OnInventorySlotChanged.IsBound())
		OnInventorySlotChanged.Broadcast();

	return true;
}


int32 UPlayerInventoryComponent::GetItemIndex(FItemSlotInfo itemSlotInfo)
{
	return InventoryItems.Find(itemSlotInfo);
}