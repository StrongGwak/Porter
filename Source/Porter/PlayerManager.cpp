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

	// 레벨 넘어갈 떄 출력되는지? - 여기선 안되는듯
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player Manager")));
}

void UPlayerManager::Initialize(TArray<TSubclassOf<AActor>> Port, TArray<TSubclassOf<APHero>> Hero)
{
	PortTypeArray = Port;
	HeroTypeArray = Hero;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player Manager")));
}

void UPlayerManager::SetSpawnInformation(const FSpawnInformation& UpdateStats)
{
	SpawnInformation = UpdateStats;
}

FSpawnInformation UPlayerManager::GetSpawnInformation() const
{
	return SpawnInformation;
}

// 레벨 넘어가기 전 저장
void UPlayerManager::SaveSpawnInformation()
{
	FSpawnInformation SpawnInfo = GetSpawnInformation();
	SpawnInfo.SavedPortNum = CheckPortNum();
	SpawnInfo.SavedHeroTypeArray.Empty();
	SpawnInfo.SavedHeroTypeArray.Init(-1, MaximumArraySize);
	for (int32 i=0; i<SpawnInfo.SavedPortNum; i++)
	{
		if(HeroArray[i] != nullptr)
		{
			SpawnInfo.SavedHeroTypeArray[i] = HeroArray[i]->HeroType;
		}

	}
	SetSpawnInformation(SpawnInfo);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Save Success! %d"),SpawnInfo.SavedPortNum ));
}

// 레벨 넘어간 후 불러오기
void UPlayerManager::OpenSpawnInformation()
{
	FSpawnInformation SpawnInfo = GetSpawnInformation();
	for (int32 i=0; i<SpawnInfo.SavedPortNum; ++i)
	{
		SpawnPort(0);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Open Success!")));
	
	for (int32 i=0; i<SpawnInfo.SavedHeroTypeArray.Num(); ++i)
	{
		if (SpawnInfo.SavedHeroTypeArray[i] == -1) continue;
		SpawnHero(SpawnInfo.SavedHeroTypeArray[i]);
	}
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
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,  FString::Printf(TEXT("PortNum : %d"), PortNum));
	
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
	int32 EmptyHeroIndex = -1;
	
	// if (bNotUseHeroIndex)
	
	if (HeroNum < PortNum)
	{
		for (int32 i=0; i<HeroArray.Num(); i++)
		{
			if(HeroArray[i] == nullptr)
			{
				EmptyHeroIndex = i;
				break;
			}
		}
		if (EmptyHeroIndex == -1)
		{
			return;
		}
		
		FVector RelativeOffset = PlayerCharacter->GetActorForwardVector()*OffsetArray[EmptyHeroIndex].X + PlayerCharacter->GetActorRightVector()*OffsetArray[EmptyHeroIndex].Y + FVector(0,0,OffsetArray[EmptyHeroIndex].Z + HeroOffset);
		FVector SpawnLocation = PlayerCharacter->GetActorLocation() + RelativeOffset;
		APHero* Hero = GetWorld()->SpawnActor<APHero>(HeroTypeArray[HeroType], SpawnLocation, PlayerCharacter->GetActorRotation());

		if (Hero)
		{
			HeroArray[EmptyHeroIndex] = Hero;
			Hero->AttachToComponent(PlayerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			Hero->Index = EmptyHeroIndex;
			Hero->HeroType = HeroType;
			// 스텟들 넣기 - HeroManager와 통신해야함
			Hero->SetActorEnableCollision(false);
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
	FSpawnInformation SpawnInfo = GetSpawnInformation();
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
			SpawnInfo.SavedHeroTypeArray[i] = HeroArray[i]->HeroType;
		}
	}
	SetSpawnInformation(SpawnInfo);
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
