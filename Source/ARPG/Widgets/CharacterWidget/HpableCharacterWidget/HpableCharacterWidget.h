#pragma once

#include "CoreMinimal.h"
#include "Widgets/CharacterWidget/CharacterWidget.h"
#include "HpableCharacterWidget.generated.h"

UCLASS()
class ARPG_API UHpableCharacterWidget : public UCharacterWidget
{
	GENERATED_BODY()

protected :
	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;
	class UProgressBar* ProgressBar_HP;

protected :
	virtual void NativeOnInitialized() override;

public :

	void InitializeWidget(class ABaseCharacter* ownerCharacter);
	void UpdateHp();
	
};
