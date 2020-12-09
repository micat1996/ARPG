// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CharacterWidget/HpableCharacterWidget/HpableCharacterWidget.h"
#include "PlayerCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UPlayerCharacterWidget : public UHpableCharacterWidget
{
	GENERATED_BODY()

private :
	class UCanvasPanel* Canvas_ClosableWnds;
	class UProgressBar* ProgressBar_HP;

protected :
	virtual void NativeOnInitialized() override;

public :
	// ���� �� �ִ� â�� �߰��մϴ�.
	void AddClosableWnd(class UClosableWnd* closableWnd);

	// ���� �� �ִ� â�� �����մϴ�.
	void RemoveClosableWnd(class UClosableWnd* closableWnd);

	FORCEINLINE class UCanvasPanel* GetClosableWnds() const
	{ return Canvas_ClosableWnds; }
	
};
