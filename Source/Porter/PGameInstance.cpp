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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Game Instance")));
		PlayerManager->Initialize(PortTypeArray, HeroTypeArray);
	}
}

UPlayerManager* UPGameInstance::GetPlayerManager()
{
	return PlayerManager;
}

UHeroManager* UPGameInstance::GetHeroManager()
{
	return HeroManger;
}
