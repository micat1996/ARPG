#pragma once

#include "ARPG.h"

#include "Components/ActorComponent.h"

#include "Enums/EquipItemType.h"
#include "Enums/ReinforceResult.h"

#include "Structures/ItemSlotInfo/ItemSlotInfo.h"

#include "PlayerInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventorySlotChangedSignature)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public :
	// �κ��丮 ���Կ� ������ ���� ��� ȣ��� �븮��
	FOnInventorySlotChangedSignature OnInventorySlotChanged;

private :
	// �κ��丮 â ���� �������Ʈ Ŭ������ ��Ÿ���ϴ�.
	TSubclassOf<class UInventoryWnd> BPInventoryWndClass;

	// ��� ������ ������ �����ϴ� DataTable Asset
	class UDataTable* DTEquipItemInfo;

	// ������ ������ �����ϴ� DawtaTable Asset
	class UDataTable* DTItemInfo;

private :
	// �κ��丮 â ��ü�� ��Ÿ���ϴ�.
	UPROPERTY()
	class UInventoryWnd* InventoryWnd;

private :
	// �������� �����۵��� ��Ÿ���ϴ�.
	TMap<EEquipItemType, FName> EquipItems;


	// ���� �� �ִ� â�� �����ϴ� ��ü�� ��Ÿ���ϴ�.
	class UClosableWndControllerComponent* ClosableWndController;

	// �������� �����۵��� ��Ÿ���ϴ�.
	TArray<FItemSlotInfo> InventoryItems;

public:	
	UPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	// ��� �������� �ʱ�ȭ�մϴ�.
	void InitializeEquipItems();

	// �κ��丮 �������� �ʱ�ȭ�մϴ�.
	void InitializeInventoryItems();

	// �κ��丮 â�� ���ϴ�.
	void OpenInventory();

	// �κ��丮 â�� �ݽ��ϴ�.
	void CloseInventory();
public :
	// �κ��丮 â ���� ���¿� ���� �κ��丮 â�� ���ų� �ݽ��ϴ�.
	FORCEINLINE void ToggleInventory()
	{
		if (IsInventoryWndOpend())	CloseInventory();
		else						OpenInventory();
	}

	// ��� ������ Mesh �� �����մϴ�.
	void UpdateEquipItemMesh(EEquipItemType equipitemType,
		class USkeletalMeshComponent* refSkMeshToUpdate,
		class USkeletalMeshComponent* refSetSkMeshToUpdate = nullptr);

	// ������ ��� ������ �ڵ带 ����ϴ�.
	FORCEINLINE FName GetEquippedItemCode(EEquipItemType equipType) const
	{ return EquipItems[equipType]; }

	// ������ ��ȭ�� �õ��մϴ�.
	EReinforceResult TryReinforce(EEquipItemType itemType);

	// �κ��丮 �������� �����մϴ�.
	FORCEINLINE void SetItemSlotInfo(int32 slotIndex, FItemSlotInfo slotInfo)
	{ InventoryItems[slotIndex] = slotInfo; }

	// �κ��丮 �������� ��ü�մϴ�.
	void SwapItem(class UItemSlot* firstItemSlot, class UItemSlot* secondItemSlot);

	// �κ��丮�� �������� �߰��մϴ�.
	/// - itemCode : �κ��丮�� �߰��� ������ �ڵ�
	/// - itemCount : �߰��� ������ ����
	/// - return : �߰� ���� ����
	bool AddItem(FName itemCode, int32 itemCount = 1);

	// �κ��丮�� �ִ� �������� �����մϴ�.
	/// - slotIndex : ������ �������� ���� ���� �ε���
	/// - removeCount : ���� ����
	/// - return : ���� ���� ����
	bool RemoveItem(int32 slotIndex, int32 removeCount);

	// �������� ��� ���� �ε����� ��ȯ�մϴ�.
	int32 GetItemIndex(FItemSlotInfo itemSlotInfo);

public :
	// �κ��丮 â�� ���ȴ����� ��Ÿ���ϴ�.
	FORCEINLINE bool IsInventoryWndOpend() const
	{ return InventoryWnd != nullptr; }

	FORCEINLINE const TArray<FItemSlotInfo>& GetInventoryItems() const
	{ return InventoryItems; }
};
