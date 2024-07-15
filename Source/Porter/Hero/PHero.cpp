// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHero.h"

#include "Hero/PHeroStruct.h"
#include "Hero/PHeroAIController.h"

// Sets default values
APHero::APHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/MarketAssets/KoreanTraditionalMartialArts/Meshs/Characters/Meshs/SKM_Soldier_1.SKM_Soldier_1'"));
	if (TempMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector3d(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	AIControllerClass = APHeroAIController::StaticClass();
	
}

// Called when the game starts or when spawned
void APHero::BeginPlay()
{
	Super::BeginPlay();

	name = GetActorLabel();
	APHeroAIController* AIController = Cast<APHeroAIController>(GetController());
	if (AIController)
	{
		UDataTable* DT_Hero = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Hero/DT_Hero.DT_Hero'"));
		FPHeroStruct* HeroData = nullptr;
		TArray<FPHeroStruct*> AllRows;
		DT_Hero->GetAllRows(TEXT(""),AllRows);
		for (FPHeroStruct* data : AllRows)
		{
			if (data->HeroName == name)
			{
				HeroData = data;
				break;
			}
		}
		
		if (HeroData != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Sight Radius :: %f"), HeroData->SightRadius);
			AIController->SetSightConfig(HeroData->SightRadius, HeroData->SightRadius + 10.0f, HeroData->PeripheralVisionAngle);
		}
	}
	
}

// Called every frame
void APHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


