// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PChangeLevel.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PPlayer.h"

// Sets default values
APChangeLevel::APChangeLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	//CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APChangeLevel::OnPlayerBeginOverlap(this));

	bIsLoading = false;

}

// Called when the game starts or when spawned
void APChangeLevel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APChangeLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APChangeLevel::OnPlayerBeginOverlap(AActor* ThisActor, AActor* PPlayer)
{
	APPlayer* Player = Cast<APPlayer>(PPlayer);
	if (Player && !bIsLoading)
	{
		bIsLoading = true;

		// 로딩맵으로 이동
		if (!LoadingLevel.IsNone())
		{
			UGameplayStatics::OpenLevel(this, LoadingLevel);
		}

		// 1초 후에 맵 전환
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APChangeLevel::TransitionToNewMap, 1.0f, false);
	}
}

void APChangeLevel::TransitionToNewMap()
{
	UGameplayStatics::OpenLevel(this, NextLevel);
}

