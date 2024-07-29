// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPortBox.h"

// Sets default values
ATestPortBox::ATestPortBox()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/EngineMeshes/SkeletalCube.SkeletalCube'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetWorldScale3D(FVector(0.5f, 0.5f, 2.0f));
	}

	if (index == 1)
	{
		ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("/Script/Engine.Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
		if (Mat.Succeeded())
		{
			GetMesh()->SetMaterial(0, Mat.Object);
		}
	}
	else if (index == 0)
	{
		ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/BasicShapes/BasicShapeMaterial_Inst.BasicShapeMaterial_Inst'"));
		if (Mat.Succeeded())
		{
			GetMesh()->SetMaterial(0, Mat.Object);
		}
	}
}

// Called when the game starts or when spawned
void ATestPortBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestPortBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestPortBox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

