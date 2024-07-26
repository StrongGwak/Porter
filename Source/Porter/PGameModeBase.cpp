// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameModeBase.h"
#include "PPlayer.h"
#include "PHero.h"
#include "PGameStateBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

APGameModeBase::APGameModeBase()
{
	// 생성자에서 해야 하나
	// PPlayer = Cast<APPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
}

void APGameModeBase::SpawnPort(int32 PortTpye, FVector CurrentLocation, FVector ForwardV, FVector RightV)
{
	PPlayer = Cast<APPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(1000));
	
	APGameStateBase* PGameState = GetWorld()->GetGameState<APGameStateBase>();
	
	if(PGameState) 		
	{
		int32 PortNum = PGameState->CheckPortNum();
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(PortNum));
		
		if (PortNum < PGameState->MaxPortNum)
		{
			FVector RelativeOffset = ForwardV*PGameState->OffsetArray[PortNum].X + RightV*PGameState->OffsetArray[PortNum].Y + FVector(0,0,PGameState->OffsetArray[PortNum].Z);
			FVector SpawnLocation = CurrentLocation + RelativeOffset;

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(RelativeOffset.X));
			
			AActor* Port = GetWorld()->SpawnActor<AActor>(PortClassArray[PortTpye], SpawnLocation, PPlayer->GetActorRotation());

			if (Port)
			{
				PGameState->PortArray[PortNum] = Port;
				Port->AttachToComponent(PPlayer->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			}
			PPlayer->SpringArm->TargetArmLength = 400 + PGameState->PortFloorArray[PortNum+1]*PPlayer->AddCameraLength;
			Port->SetActorEnableCollision(false);
			
		}
		
	}
	
}

void APGameModeBase::DestroyPort(int32 Value)
{
}

void APGameModeBase::SpawnHero(int32 HeroType)
{
}

void APGameModeBase::DestroyHero(int32 Value)
{
}

/*
 플레이어를 알아야 붙임
 hero, port을 알아야 소환
 */
