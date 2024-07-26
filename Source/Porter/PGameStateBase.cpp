// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameStateBase.h"
#include "PPlayer.h"
#include "PHero.h"

APGameStateBase::APGameStateBase()
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

int32 APGameStateBase::CheckPortNum()
{
	int32 Count = 0;
	for (AActor* PortAddress : PortArray)
	{
		if (PortAddress != nullptr)
		{
			Count++;
		}
	}
	return Count;
}

int32 APGameStateBase::CheckHeroNum()
{
	int32 Count = 0;
	for (APHero* HeroAddress : HeroArray)
	{
		if (HeroAddress != nullptr)
		{
			Count++;
		}
	}
	return Count;
}
