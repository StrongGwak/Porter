// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameInstance.h"
#include "PPlayer.h"
#include "PHero.h"

UPGameInstance::UPGameInstance()
{
}

void UPGameInstance::Init()
{
	Super::Init();

	PlayerManager = NewObject<UPlayerManager>(this);
	if (PlayerManager)
	{
		PlayerManager->Initialize(PortTypeArray, HeroTypeArray);
	}
}

UPlayerManager* UPGameInstance::GetPlayerManager() const
{
	return PlayerManager;
}
