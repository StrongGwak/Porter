// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroManager.h"
#include "PGameInstance.h"
#include "../Hero/PHero.h"

UHeroManager::UHeroManager()
{
	HeroArray.Init(nullptr, MaximumArraySize);
	HeroDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Porter/Develop/Hero/DT_Hero.DT_Hero"));
	HeroClass = APHero::StaticClass();
}

void UHeroManager::Initialize()
{
	
}

void UHeroManager::SetGameInstance(UPGameInstance* PGameInstance)
{
	GI = PGameInstance;
}

// 레벨 넘어가기 전 hero stat 저장
void UHeroManager::SaveSpawnInformation()
{
	if (!HeroArray.IsEmpty())
	{
		for (APHero* Hero : HeroArray)
		{
			HeroStructArray.Add(Hero->GetHeroStats());
		}
	}
}

// 레벨 넘어간 후 Hero Stat에 따른 Hero 소환
void UHeroManager::OpenSpawnInformation(ACharacter* PlayerCharacter)
{
	if (!HeroArray.IsEmpty())
	{
		for (FPHeroStruct HeroStruct : HeroStructArray)
        	{
        		SpawnHero(HeroStruct);
        	}
	}
}

TArray<APHero*> UHeroManager::GetHeroArray()
{
	return HeroArray;
}

void UHeroManager::SetHeroArray(TArray<APHero*> Heroes)
{
	HeroArray = Heroes;
}

APHero* UHeroManager::FindHero(FName RowName)
{
	FPHeroStruct HeroStruct;
	static const FString ContextString(TEXT("Hero Null"));
	if (HeroDataTable)
	{
		FPHeroStruct* HeroStructPtr = HeroDataTable->FindRow<FPHeroStruct>(RowName, ContextString);
		if (HeroStructPtr)
		{
			HeroStruct = *HeroStructPtr;
			return SpawnHero(HeroStruct);
		}
	}
	return nullptr;
}


APHero* UHeroManager::SpawnHero(FPHeroStruct HeroStruct)
{
	if (HeroStruct.Index == -1)
	{
		APHero* Hero = GetWorld()->SpawnActor<APHero>(HeroClass);
		if (Hero)
		{
			int index = CheckHeroNum();
			// 위치와 종류 부여
			HeroStruct.Index = index;

			// 변경사항 저장
			HeroArray[index] = Hero;
		
			Hero->Initialize(HeroStruct);

			return Hero;
		}
	}
	else
	{
		APHero* Hero = GetWorld()->SpawnActor<APHero>(HeroClass);
		if (Hero)
		{

			// 변경사항 저장
			HeroArray[HeroStruct.Index] = Hero;
		
			Hero->Initialize(HeroStruct);

			return Hero;
		}
	}
	
	return nullptr;
}

void UHeroManager::DestroyHero(int32 HeroIndex)
{
	if (HeroArray[HeroIndex] != nullptr)
	{
		HeroArray[HeroIndex]->Die();
		//HeroArray[HeroIndex]->Destroy();
		HeroArray[HeroIndex] = nullptr;
	}
}

TArray<APHero*> UHeroManager::SwapHeroes(TArray<int32> IndexArray)
{
	for (int index : IndexArray)
	{
		HeroArray[index]->SetIndex(index);
	}

	return HeroArray;
}

int32 UHeroManager::LastHeroNum()
{
	int32 LastNum = -1;
	for (int32 i=HeroArray.Num()-1; i>=0; --i)
	{
		if (HeroArray[i] != nullptr)
		{
			LastNum = i;
			break;
		}
	}
	return LastNum;
}

int32 UHeroManager::CheckHeroNum()
{
	int32 Count = 0;
	for (auto Hero: HeroArray)
	{
		if (Hero != nullptr)
		{
			Count++;
		}
	}
	return Count;
}

