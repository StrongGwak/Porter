// Fill out your copyright notice in the Description page of Project Settings.


#include "PHero.h"
#include "PPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APHero::APHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/EngineMeshes/SkeletalCube.SkeletalCube'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	if (Index == 1)
	{
		ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("/Script/Engine.Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
		if (Mat.Succeeded())
		{
			GetMesh()->SetMaterial(0, Mat.Object);
		}
	}
	else if (Index == 0)
	{
		ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/BasicShapes/BasicShapeMaterial_Inst.BasicShapeMaterial_Inst'"));
		if (Mat.Succeeded())
		{
			GetMesh()->SetMaterial(0, Mat.Object);
		}
	}
}

// Called when the game starts or when spawned
void APHero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APHero::Die()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("DEAD"));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(Index));
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	APPlayer* RealPlayer = Cast<APPlayer>(player);
	RealPlayer->DestroyHero(Index);
}

