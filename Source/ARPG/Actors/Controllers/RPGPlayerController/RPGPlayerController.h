// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()


private :
	// ���� �� �ִ� â�� �����ϴ� ������Ʈ�Դϴ�.
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UClosableWndControllerComponent * ClosableWndController;

	// ���� ������ ���� UClass �� �ν��Ͻ��� ��Ÿ���ϴ�.
	TSubclassOf<class UPlayerCharacterWidget> GameWidgetClass;
	class UPlayerCharacterWidget* PlayerCharacterWidgetInstance;


public :
	ARPGPlayerController();

protected :
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;



private :
	void InputMouseX(float axis);
	void InputMouseY(float axis);

public :
	class UPlayerCharacterWidget* GetPlayerCharacterWidgetInstance();
	
	FORCEINLINE class UClosableWndControllerComponent* GetClosableWndControllerComponent() const
	{ return ClosableWndController; }

	// �� Ÿ���� �����մϴ�.
	/// - nullptr �� ���޵� ��� ������ �����ϴ� �÷��̾� ī�޶�� �����մϴ�.
	void ChangeViewTarget(class AActor* newViewTarget);

};
