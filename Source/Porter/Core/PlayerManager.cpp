// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "PGameInstance.h"
#include "Commandlets/GatherTextFromSourceCommandlet.h"
#include "../Hero/PHero.h"
#include "Player/PPlayer.h"

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
		OffsetArray.Emplace(FVector(OffsetX, OffsetY, PortFloor*PortHeight - 100));
	}
	OffsetArray.RemoveAt(0);
	PortArray.Init(nullptr, MaximumArraySize);
}

void UPlayerManager::Initialize(TArray<TSubclassOf<AActor>> Port)
{
	PortTypeArray = Port;
}

void UPlayerManager::SetGameInstance(UPGameInstance* PGameInstance)
{
	GI = PGameInstance;
}

// 레벨 넘어가기 전 저장
void UPlayerManager::SaveSpawnInformation()
{
	SavedPortNum = CheckPortNum();
}

// 레벨 넘어간 후 저장된 Port 수만큼 Port 소환
void UPlayerManager::OpenSpawnInformation(ACharacter* PlayerCharacter)
{
	for (int32 i=0; i<SavedPortNum - 1; ++i)
	{
		SpawnPort(0, PlayerCharacter);
	}
}

void UPlayerManager::SavePlayerStats(ACharacter* PlayerCharacter)
{
	APPlayer* Player = Cast<APPlayer>(PlayerCharacter);
	PPlayerStats = Player->GetStats();
}

void UPlayerManager::OpenPlayerStats(ACharacter* PlayerCharacter)
{
	APPlayer* Player = Cast<APPlayer>(PlayerCharacter);
	Player->SetStats(PPlayerStats);
}

int32 UPlayerManager::SpawnPort(int32 PortType, ACharacter* PlayerCharacter)
{
	USkeletalMeshComponent* MeshComp = PlayerCharacter->GetMesh();
	int32 PortNum = CheckPortNum();
	float SpringArmLength = AddCameraLength*PortFloorArray[PortNum];
	
	if(PortNum < MaxPortNum)
	{
		FVector SocketLocation = MeshComp->GetSocketLocation(FName("PortSocket"));
		FVector RelativeOffset = SocketLocation.ForwardVector*OffsetArray[PortNum].X
								+ SocketLocation.RightVector*OffsetArray[PortNum].Y
								+ SocketLocation.UpVector*OffsetArray[PortNum].Z;
		FVector SpawnLocation = SocketLocation + RelativeOffset;
		AActor* Port = GetWorld()->SpawnActor<AActor>(PortTypeArray[PortType], SpawnLocation, PlayerCharacter->GetActorRotation());

		if (Port)
		{
			Port->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("PortSocket"));
			Port->SetActorRelativeLocation(RelativeOffset);
			Port->SetActorEnableCollision(false);
			SpringArmLength = AddCameraLength*PortFloorArray[PortNum+1];
			PortArray[PortNum] = Port;
		}
	}
	return SpringArmLength;
}

int32 UPlayerManager::DestroyPort()
{
	int32 PortNum = CheckPortNum();
	TArray<APHero*> HeroArray = GI->GetHeroManager()->GetHeroArray();
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
	GI->GetHeroManager()->SetHeroArray(HeroArray);
	
	return SpringArmLength;
}

int32 UPlayerManager::CheckPortNum()
{
	int32 Count = 0;
	for (auto i : PortArray)
	{
		if (i != nullptr)
		{
			Count++;
		}
	}
	return Count;
}