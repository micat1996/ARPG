#pragma once

#include "ARPG.h"
#include "Components/ActorComponent.h"

#include "Widgets/ClosableWnd/ClosableWnd.h"
#include "Widgets/CharacterWidget/HpableCharacterWidget/PlayerCharacterWidget/PlayerCharacterWidget.h"
#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

#include "ClosableWndControllerComponent.generated.h"


UCLASS()
class ARPG_API UClosableWndControllerComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	UPROPERTY()
	ARPGPlayerController* PlayerController;

	// �Ҵ�� â ��ü���� ������ �迭
	UPROPERTY()
	TArray<UClosableWnd*> AllocatedWnds;

public:	
	UClosableWndControllerComponent();

protected:
	virtual void BeginPlay() override;

public :
	// â�� �߰��մϴ�.
	template<typename T>
	T* AddWnd(TSubclassOf<UClosableWnd> wndCls)
	{
		// â ��ü�� �����մϴ�.
		T * wndInst = Cast<T>(CreateWidget<UClosableWnd>(PlayerController, wndCls));

		wndInst->SetClosableWndContorller(this);

		// �迭�� �߰��մϴ�.
		AllocatedWnds.Add(wndInst);

		// ȭ�鿡 ǥ���մϴ�.
		PlayerController->GetPlayerCharacterWidgetInstance()->AddClosableWnd(wndInst);

		// â ��ü�� ��ȯ�մϴ�.
		return wndInst;
	}

	// â�� �ݽ��ϴ�.
	/// - bAllClose : ��� â�� �ݵ��� �� �������� �����մϴ�.
	/// - closableWndInstanceToClose : � â�� ���� ������ �����մϴ�.
	///   - ���� nullptr ��� �������� ������ â�� �����ϴ�.
	void CloseWnd(bool bAllClose = false, class UClosableWnd* closableWndInstanceToClose = nullptr);

	// �Ҵ�� â ������ ��ȯ�մϴ�.
	FORCEINLINE int32 GetAllocatedWndCount() const
	{ return AllocatedWnds.Num(); }

	// â�� �Ҵ�Ǿ��ִ��� Ȯ���մϴ�.
	bool IsAllocated(UClosableWnd* closableWndInstance);

	// â�� �ֻ������ �����մϴ�.
	void SetHighestPriorityWnd(UClosableWnd* closableWndInstance);
		
};
