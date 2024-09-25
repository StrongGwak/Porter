// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameInstance.h"

UPGameInstance::UPGameInstance()
{
}

void UPGameInstance::Init()
{
	Super::Init();

	PlayerManager = NewObject<UPlayerManager>(this);
	if (PlayerManager)
	{
		PlayerManager->Initialize(PortTypeArray);
		PlayerManager->SetGameInstance(this);
		
	}

	HeroManager = NewObject<UHeroManager>(this);
	if (HeroManager)
	{
		HeroManager->Initialize();
		HeroManager->SetGameInstance(this);
	}

	ItemManager = NewObject<UItemManager>(this);
	if (ItemManager)
	{
		ItemManager->SetGameInstance(this);
	}
}

UPlayerManager* UPGameInstance::GetPlayerManager()
{
	return PlayerManager;
}

UHeroManager* UPGameInstance::GetHeroManager()
{
	return HeroManager;
}

UItemManager* UPGameInstance::GetItemManager()
{
	return ItemManager;
}
