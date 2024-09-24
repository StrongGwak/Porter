// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PPort.h"

// Sets default values
APPort::APPort()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Port = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Port"));
	RootComponent = Port;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SMPort(TEXT("/Script/Engine.StaticMesh'/Game/Porter/Develop/Hero/SM_Port2.SM_Port2'"));
	if (SMPort.Succeeded())
	{
		Port->SetStaticMesh(SMPort.Object);
	}
}

// Called when the game starts or when spawned
void APPort::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

