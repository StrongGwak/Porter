// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameInstance.h"
#include "PPlayer.h"
#include "PHero.h"


UPGameInstance::UPGameInstance()
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

int32 UPGameInstance::SpawnPort(int32 PortType)
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

int32 UPGameInstance::DestroyPort()
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

int32 UPGameInstance::CheckPortNum()
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

void UPGameInstance::SpawnHero(int32 HeroType)
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

		FVector RelativeOffset = PlayerCharacter->GetActorForwardVector()*OffsetArray[HeroNum].X + PlayerCharacter->GetActorRightVector()*OffsetArray[HeroNum].Y + FVector(0,0,OffsetArray[HeroNum].Z + HeroOffset);
		FVector SpawnLocation = PlayerCharacter->GetActorLocation() + RelativeOffset;
		APHero* Hero = GetWorld()->SpawnActor<APHero>(HeroTypeArray[HeroType], SpawnLocation, PlayerCharacter->GetActorRotation());

		if (Hero)
		{
			HeroArray[HeroNum] = Hero;
			Hero->AttachToComponent(PlayerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			// 카메라 안가리게 바꾸는 법 알아보기
			Hero->SetActorEnableCollision(false);
		}
	}
}

int32 UPGameInstance::CheckHeroNum()
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
