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
    // �÷��̾� ����
    FPlayerInfo PlayerInfo;

    // �÷��̾� ĳ���� ��Ʈ�ѷ�
    UPROPERTY()
    class ARPGPlayerController* PlayerController;
        

public:
    virtual void InitManagerClass() override;

public :
    FORCEINLINE FPlayerInfo* GetPlayerInfo()
    { return &PlayerInfo; }

    class ARPGPlayerController* GetPlayerController();




};
