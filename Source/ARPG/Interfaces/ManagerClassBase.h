#pragma once
#include "ARPG.h"
#include "Single/GameInstance/ARPGGameInstance.h"
#include "UObject/Interface.h"
#include "ManagerClassBase.generated.h"




UINTERFACE(MinimalAPI)
class UManagerClassBase : public UInterface
{
	GENERATED_BODY()
};

class ARPG_API IManagerClassBase
{
	GENERATED_BODY()

public:
	virtual void InitManagerClass() PURE_VIRTUAL(IManagerClassBase::InitManagerClass, );
};
