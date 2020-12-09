// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "Single/ManagerClass/ManagerClass.h"
#include "Structures/PlayerInfo/PlayerInfo.h"
#include "PlayerManager.generated.h"

UCLASS()
class ARPG_API UPlayerManager : public UManagerClass
{
    GENERATED_BODY()


private :
    // 플레이어 정보
    FPlayerInfo PlayerInfo;

    // 플레이어 캐릭터 컨트롤러
    UPROPERTY()
    class ARPGPlayerController* PlayerController;
        

public:
    virtual void InitManagerClass() override;

public :
    FORCEINLINE FPlayerInfo* GetPlayerInfo()
    { return &PlayerInfo; }

    class ARPGPlayerController* GetPlayerController();




};
