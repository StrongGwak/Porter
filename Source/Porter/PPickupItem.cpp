// Fill out your copyright notice in the Description page of Project Settings.


#include "PPickupItem.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

// Sets default values
APPickupItem::APPickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));

	RootComponent = TriggerVolume;
	ItemMesh->SetupAttachment(RootComponent);

	TriggerVolume->SetBoxExtent(FVector(40, 42, 30));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Item(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (Item.Succeeded())
	{
		ItemMesh->SetStaticMesh(Item.Object);
	}

}

// Called when the game starts or when spawned
void APPickupItem::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APPickupItem::MyCustomOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APPickupItem::MyCustomOverlapEnd);;
	
}

// Called every frame
void APPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPickupItem::MyCustomOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("BeginOverlap"));
}

void APPickupItem::MyCustomOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("EndOverlap"));
}



