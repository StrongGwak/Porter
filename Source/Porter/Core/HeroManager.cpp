// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroManager.h"
#include "PGameInstance.h"
#include "../Hero/PHero.h"

UHeroManager::UHeroManager()
{
	HeroArray.Init(nullptr, MaximumArraySize);
}

void UHeroManager::Initialize(TArray<TSubclassOf<APHero>> Hero)
{
	HeroTypeArray = Hero;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hero Manager")));
}

void UHeroManager::SetGameInstance(UPGameInstance* PGameInstance)
{
	GI = PGameInstance;
}

// 레벨 넘어가기 전 hero stat 저장
void UHeroManager::SaveSpawnInformation()
{
	FPHeroStruct HeroStruct;
	HeroStructArray.Empty();
	HeroStructArray.Init(HeroStruct, MaximumArraySize);

	for (int32 i=0; i<MaximumArraySize; i++)
	{
		if (HeroArray[i] != nullptr)
		{
			HeroStructArray[i] = HeroArray[i]->GetHeroStats();
		}
	}
}

// 레벨 넘어간 후 Hero Stat에 따른 Hero 소환
void UHeroManager::OpenSpawnInformation(ACharacter* PlayerCharacter)
{
	for (int32 i=0; i<HeroStructArray.Num(); ++i)
	{
		if (HeroStructArray[i].Type == -1)
		{
			continue;
		}
		SpawnHero(HeroStructArray[i].Type, PlayerCharacter, true, i);
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


void UHeroManager::SpawnHero(int32 HeroType, ACharacter* PlayerCharacter, bool bUseHeroIndex, int32 HeroIndex)
{
	USkeletalMeshComponent* SMComp = PlayerCharacter->GetMesh();
	
	int32 PortNum = GI->GetPlayerManager()->CheckPortNum();
	//GEngine->AddOnScreenDebugMessage(-1,3,FColor::Blue,FString::FromInt(PortNum));
	int32 HeroNum = CheckHeroNum();

	// HeroIndex를 사용하지 않으면, 빈 곳에 영웅 소환하기
	if (!bUseHeroIndex && HeroNum < PortNum)
	{
		for (int32 i=0; i<HeroArray.Num(); i++)
		{
			if(HeroArray[i] == nullptr)
			{
				HeroIndex = i;
				break;
			}
		}
	}
	if (HeroIndex == -1)
	{
		return;
	}

	TArray<FVector> OffsetArray = GI->GetPlayerManager()->OffsetArray;
	FVector SocketLocation = SMComp->GetSocketLocation(FName("PortSocket"));
	FVector RelativeOffset = SocketLocation.ForwardVector*OffsetArray[HeroIndex].X
							+ SocketLocation.RightVector*OffsetArray[HeroIndex].Y
							+ SocketLocation.UpVector*(OffsetArray[HeroIndex].Z + HeroOffset);
	FVector SpawnLocation = SocketLocation + RelativeOffset;
	APHero* Hero = GetWorld()->SpawnActor<APHero>(HeroTypeArray[HeroType], SpawnLocation, PlayerCharacter->GetActorRotation());

	if (Hero)
	{
		FPHeroStruct HeroStruct = Hero->GetHeroStats();

		//Hero->AttachToComponent(PlayerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		Hero->AttachToComponent(SMComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("PortSocket"));
		Hero->SetActorRelativeLocation(RelativeOffset);

		// 위치와 종류 부여
		HeroStruct.Index = HeroIndex;
		HeroStruct.Type = HeroType;

		// 변경사항 저장
		HeroArray[HeroIndex] = Hero;
		Hero->SetHeroStats(HeroStruct);
	}
}

void UHeroManager::DestroyHero(int32 HeroIndex)
{
	if (HeroArray[HeroIndex] != nullptr)
	{
		HeroArray[HeroIndex]->Destroy();
		HeroArray[HeroIndex] = nullptr;
	}
}

void UHeroManager::SwapHeroes(TArray<int32> IndexArray, ACharacter* PlayerCharacter)
{
	FPHeroStruct HeroStruct;
	USkeletalMeshComponent* MeshComp = PlayerCharacter->GetMesh();
	TArray<APHero*> BeforeHeroArray;
	TArray<FVector> OffsetArray = GI->GetPlayerManager()->OffsetArray;
	FVector SocketLocation = MeshComp->GetSocketLocation(FName("PortSocket"));
	BeforeHeroArray.Append(HeroArray);
	
	for (int32 HeroIndex=0; HeroIndex<HeroArray.Num(); ++HeroIndex)
	{
		HeroArray[HeroIndex] = BeforeHeroArray[IndexArray[HeroIndex]];
		if (HeroArray[HeroIndex] != nullptr)
		{
			HeroArray[HeroIndex]->SetActorRelativeLocation(
				SocketLocation.ForwardVector*OffsetArray[HeroIndex].X
				+ SocketLocation.RightVector*OffsetArray[HeroIndex].Y
				+ SocketLocation.UpVector*(OffsetArray[HeroIndex].Z + HeroOffset)
			);
		
			// Index 재할당 및 SpawnInformation의 HeroType 고치기
			HeroStruct = HeroArray[HeroIndex]->GetHeroStats();
			HeroStruct.Index = HeroIndex;
			HeroArray[HeroIndex]->SetHeroStats(HeroStruct);
		}
	}
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
