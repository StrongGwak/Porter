// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "PPlayer.h"
#include "PHero.h"

UPlayerManager::UPlayerManager()
{
	int32 PortFloor = 0;
	for (int PortNum=0; PortNum<MaximumArraySize+1; PortNum++)
	{
		PortFloor = 0;
		while (true)
		{
			if (PortNum <= PortFloor * (PortFloor + 1) / 2) break;
			else PortFloor++;
		}
		PortFloorArray.Emplace(PortFloor);

		float OffsetY = 0;
		int32 SpawnNum = PortNum - (PortFloor - 1) * PortFloor / 2;
		for(int32 i=0; i<SpawnNum; i++)
		{
			if (i%2 == 1) OffsetY = -1*OffsetY - 2*PortWidth;
			else OffsetY *= -1;
		}
		if (PortFloor%2 == 0) OffsetY += PortWidth;
		OffsetArray.Emplace(FVector(OffsetX, OffsetY, PortFloor*PortHeight));
	}
	OffsetArray.RemoveAt(0);
	PortArray.Init(nullptr,MaximumArraySize);
	HeroArray.Init(nullptr,MaximumArraySize);
}

void UPlayerManager::Initialize(TArray<TSubclassOf<AActor>> Port, TArray<TSubclassOf<APHero>> Hero)
{
	PortTypeArray = Port;
	HeroTypeArray = Hero;
}

void UPlayerManager::SetPlayerStats(const FPlayerStatsStruct& UpdateStats)
{
	PlayerStats = UpdateStats;
}

FPlayerStatsStruct UPlayerManager::GetPlayerStats() const
{
	return PlayerStats;
}

int32 UPlayerManager::SpawnPort(int32 PortType)
{
	APawn* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
	int32 PortNum = CheckPortNum();
	float SpringArmLength = AddCameraLength*PortFloorArray[PortNum];
	if(PortNum < MaxPortNum)
	{
		FVector RelativeOffset = PlayerCharacter->GetActorForwardVector()*OffsetArray[PortNum].X + PlayerCharacter->GetActorRightVector()*OffsetArray[PortNum].Y + FVector(0,0,OffsetArray[PortNum].Z);
		FVector SpawnLocation = PlayerCharacter->GetActorLocation() + RelativeOffset;
		AActor* Port = GetWorld()->SpawnActor<AActor>(PortTypeArray[PortType], SpawnLocation, PlayerCharacter->GetActorRotation());

		if (Port)
		{
			PortArray[PortNum] = Port;
			Port->AttachToComponent(PlayerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			Port->SetActorEnableCollision(false);
			SpringArmLength = AddCameraLength*PortFloorArray[PortNum+1];
		}
	}
	return SpringArmLength;
}

int32 UPlayerManager::DestroyPort()
{
	int32 PortNum = CheckPortNum();
	if(PortNum > 0)
	{
		PortNum--;
		PortArray[PortNum]->Destroy();
		PortArray[PortNum] = nullptr;

		// 만약 영웅이 타고있다면 함께 파괴
		if (HeroArray[PortNum] != nullptr)
		{
			HeroArray[PortNum]->Destroy();
			HeroArray[PortNum] = nullptr;
		}
	}
	float SpringArmLength = AddCameraLength*PortFloorArray[PortNum];
	return SpringArmLength;
}

int32 UPlayerManager::CheckPortNum()
{
	int32 Count = 0;
	for (AActor* i : PortArray)
	{
		if (i != nullptr)
		{
			Count++;
		}
	}
	return Count;
}

void UPlayerManager::SpawnHero(int32 HeroType)
{
	APawn* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
	int32 PortNum = CheckPortNum();
	int32 HeroNum = CheckHeroNum();

	int32 FirstEmptyIndex = -1;
	if (HeroNum < PortNum)
	{
		for (int32 i=0; i<HeroArray.Num(); i++)
		{
			if(HeroArray[i] == nullptr)
			{
				FirstEmptyIndex = i;
				break;
			}
		}
		if (FirstEmptyIndex == -1) return;

		FVector RelativeOffset = PlayerCharacter->GetActorForwardVector()*OffsetArray[FirstEmptyIndex].X + PlayerCharacter->GetActorRightVector()*OffsetArray[FirstEmptyIndex].Y + FVector(0,0,OffsetArray[FirstEmptyIndex].Z + HeroOffset);
		FVector SpawnLocation = PlayerCharacter->GetActorLocation() + RelativeOffset;
		APHero* Hero = GetWorld()->SpawnActor<APHero>(HeroTypeArray[HeroType], SpawnLocation, PlayerCharacter->GetActorRotation());

		if (Hero)
		{
			HeroArray[FirstEmptyIndex] = Hero;
			Hero->AttachToComponent(PlayerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			// 카메라 안가리게 바꾸는 법 알아보기
			Hero->SetActorEnableCollision(false);
			Hero->Index = FirstEmptyIndex;
		}
	}
}

void UPlayerManager::DestroyHero(int32 HeroIndex)
{
	if (HeroArray[HeroIndex] != nullptr)
	{
		HeroArray[HeroIndex]->Destroy();
		HeroArray[HeroIndex] = nullptr;
	}
}

void UPlayerManager::SwapHeroes(TArray<int32> IndexArray)
{
	TArray<APHero*> BeforeHeroArray;
	APawn* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	BeforeHeroArray.Append(HeroArray);
	for (int32 i=0; i<HeroArray.Num(); ++i)
	{
		HeroArray[i] = BeforeHeroArray[IndexArray[i]];
		if (HeroArray[i] != nullptr)
		{
			HeroArray[i]->SetActorLocation(
				PlayerCharacter->GetActorLocation()
				+ PlayerCharacter->GetActorForwardVector()*OffsetArray[i].X
				+ PlayerCharacter->GetActorRightVector()*OffsetArray[i].Y
				+ FVector(0,0,OffsetArray[i].Z + HeroOffset)
			);
			HeroArray[i]->Index = i;
		}
	}
}

// 항상 0보다 커야함. 0 == 죽음
int32 UPlayerManager::LastHeroNum()
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

int32 UPlayerManager::CheckHeroNum()
{
	int32 Count = 0;
	for (APHero* i : HeroArray)
	{
		if (i != nullptr)
		{
			Count++;
		}
	}
	return Count;
}
