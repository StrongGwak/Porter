// Fill out your copyright notice in the Description page of Project Settings.


#include "TestHeroBox.h"
#include "PPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATestHeroBox::ATestHeroBox()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/EngineMeshes/SkeletalCube.SkeletalCube'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
}

// Called when the game starts or when spawned
void ATestHeroBox::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	APPlayer* PPlayer = Cast<APPlayer>(Player);
	if(PPlayer)
	{
		Offset = PPlayer->Offset;
	}
}

// Called every frame
void ATestHeroBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Player)
	{
		SetActorLocation(Player->GetActorLocation() + Offset); //GetActorForwardVector를 사용하기  
	}
}

// Called to bind functionality to input
void ATestHeroBox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

