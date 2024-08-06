// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"

#include "Commandlets/GatherTextFromSourceCommandlet.h"
#include "Hero/PHero.h"

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
	HeroArray.Init(nullptr, MaximumArraySize);
	//이거 쓰면 다운됨
	//SpawnInfo.HeroStructArray.Init(HeroStruct, MaximumArraySize);
}

void UPlayerManager::Initialize(TArray<TSubclassOf<AActor>> Port, TArray<TSubclassOf<APHero>> Hero)
{
	PortTypeArray = Port;
	HeroTypeArray = Hero;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player Manager")));
}

// 레벨 넘어가기 전 저장
void UPlayerManager::SaveSpawnInformation()
{
	FPHeroStruct HeroStruct;
	
	SavedPortNum = CheckPortNum();
	HeroStructArray.Empty();
	HeroStructArray.Init(HeroStruct, MaximumArraySize);
	
	for (int32 i=0; i<SavedPortNum; i++)
	{
		if(HeroArray[i] != nullptr)
		{
			HeroStructArray[i] = HeroArray[i]->GetHeroStats();
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Open Success! : %d"), CheckPortNum()));
}

// 레벨 넘어간 후 불러오기
void UPlayerManager::OpenSpawnInformation(ACharacter* PlayerCharacter)
{
	for (int32 i=0; i<SavedPortNum; ++i)
	{
		SpawnPort(0, PlayerCharacter);
	}
	
	for (int32 i=0; i<HeroStructArray.Num(); ++i)
	{
		if (HeroStructArray[i].Type == -1) 
		{
			continue;
		}
		SpawnHero(HeroStructArray[i].Type, PlayerCharacter, true, i);
	}
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
	float SpringArmLength = AddCameraLength*PortFloorArray[PortNum];
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
	SpringArmLength = AddCameraLength*PortFloorArray[PortNum];
	
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

void UPlayerManager::SpawnHero(int32 HeroType, ACharacter* PlayerCharacter, bool bUseHeroIndex, int32 HeroIndex)
{
	USkeletalMeshComponent* MeshComp = PlayerCharacter->GetMesh();
	int32 PortNum = CheckPortNum();
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

	FVector SocketLocation = MeshComp->GetSocketLocation(FName("PortSocket"));
	FVector RelativeOffset = SocketLocation.ForwardVector*OffsetArray[HeroIndex].X
							+ SocketLocation.RightVector*OffsetArray[HeroIndex].Y
							+ SocketLocation.UpVector*(OffsetArray[HeroIndex].Z + HeroOffset);
	FVector SpawnLocation = SocketLocation + RelativeOffset;
	APHero* Hero = GetWorld()->SpawnActor<APHero>(HeroTypeArray[HeroType], SpawnLocation, PlayerCharacter->GetActorRotation());

	if (Hero)
	{
		FPHeroStruct HeroStruct = Hero->GetHeroStats();

		//Hero->AttachToComponent(PlayerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		Hero->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("PortSocket"));
		Hero->SetActorRelativeLocation(RelativeOffset);

		// 위치와 종류 부여
		HeroStruct.Index = HeroIndex;
		HeroStruct.Type = HeroType;

		// 변경사항 저장
		HeroArray[HeroIndex] = Hero;
		Hero->SetHeroStats(HeroStruct);
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

void UPlayerManager::SwapHeroes(TArray<int32> IndexArray, ACharacter* PlayerCharacter)
{
	FPHeroStruct HeroStruct;
	USkeletalMeshComponent* MeshComp = PlayerCharacter->GetMesh();
	TArray<APHero*> BeforeHeroArray;

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