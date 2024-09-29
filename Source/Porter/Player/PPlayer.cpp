// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/Engine.h"
#include "../Hero/PHero.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Core/PGameInstance.h"

// Sets default values
APPlayer::APPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldLocation(FVector(0, 0, 70));
	SpringArm->TargetArmLength = 400;
	SpringArm->bUsePawnControlRotation=true;
	SpringArm->bDoCollisionTest = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	bUseControllerRotationYaw = true; 

	FObjectFinderInputManager();
}

// Called when the game starts or when spawned
void APPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 생성자에서 쓰면 안됨
	GI = Cast<UPGameInstance>(GetGameInstance());
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(IMC, 0);
		}
	}
	SetStats(Stats);
	GetCharacterMovement()->MaxWalkSpeed = Stats.WalkSpeed;

	UpPort();
	
}

// Called every frame
void APPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPlayer::Look);
	EnhancedInputComponent->BindAction(TestUpAction, ETriggerEvent::Started, this, &APPlayer::UpPort);
	EnhancedInputComponent->BindAction(TestDownAction, ETriggerEvent::Started, this, &APPlayer::DownPort);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APPlayer::Boost);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APPlayer::StopBoost);
	EnhancedInputComponent->BindAction(SwapAction, ETriggerEvent::Started, this, &APPlayer::PlaySwap);
	EnhancedInputComponent->BindAction(TestHeroUpAction, ETriggerEvent::Started, this, &APPlayer::TempTest);
	EnhancedInputComponent->BindAction(TestHeroKill, ETriggerEvent::Started, this, &APPlayer::MakeHeroHPZero);
	EnhancedInputComponent->BindAction(TestDamage, ETriggerEvent::Started, this, &APPlayer::GetDamage);
	EnhancedInputComponent->BindAction(TestSave, ETriggerEvent::Started, this, &APPlayer::SaveSpawn);
	EnhancedInputComponent->BindAction(TestOpen, ETriggerEvent::Started, this, &APPlayer::OpenSpawn);
}

// 블루프린트에서 업데이트 할 사항
void APPlayer::SetStats(FPlayerStatsStruct UpdateStat)
{
	Stats = UpdateStat;

	// 스탯 저장용
	MaxHp = Stats.MaxHp;
	MaxStamina = Stats.MaxStamina;
	DecreaseStamina = Stats.DecreaseStamina;
	IncreaseStamina = Stats.IncreaseStamina;
	ZeroToHundredIncreaseStamina = Stats.ZeroToHundredIncreaseStamina;
	MaxWalkSpeed = Stats.WalkSpeed;
	BoostSpeed = Stats.BoostSpeed;
	MaxWeight = Stats.MaxWeight;

	// 
}

FPlayerStatsStruct APPlayer::GetStats()
{
	return Stats;
}

void APPlayer::PlusHP(int32 Heal)
{
	if (CurrentHP + Heal >= MaxHp )
	{
		CurrentHP = MaxHp;
	}
	else
	{
		CurrentHP += Heal;
	}
}

void APPlayer::MinusHP(int32 Damage)
{
	if (CurrentHP > Damage)
	{
		CurrentHP -= Damage;
	}
	else
	{
		// (수정 필요) hero 없애기
		int32 LastNum = GI->GetHeroManager()->LastHeroNum();
		if (LastNum == -1)
		{
			Die();
		}
		else
		{
			GI->GetHeroManager()->DestroyHero(LastNum);
			CurrentHP = MaxHp;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Debug %d"), CurrentHP));
}

void APPlayer::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y * GetWorld()->DeltaTimeSeconds * mouseSpeed * -1);
	AddControllerYawInput(LookAxisVector.X * GetWorld()->DeltaTimeSeconds * mouseSpeed);
}

void APPlayer::Boost()
{
	if(bCanBoost && GetCharacterMovement()->Velocity.Size() > 0.1)
	{
		bIsBoost = true;
		StartBoostTime = GetWorld()->GetTimeSeconds();
		TempStamina = CurrentStamina;
		GetWorldTimerManager().ClearTimer(RestTimeHandle);
		GetWorldTimerManager().SetTimer(BoostTimeHandle, this, &APPlayer::UpdateBoost, 0.1f, true);
		GetCharacterMovement()->MaxWalkSpeed = Stats.BoostSpeed;
	}
}

void APPlayer::StopBoost()
{
	bIsBoost = false;
	RestTime = GetWorld()->GetTimeSeconds();
	TempStamina = CurrentStamina;
	GetCharacterMovement()->MaxWalkSpeed = Stats.WalkSpeed;
	GetWorldTimerManager().ClearTimer(BoostTimeHandle);
	// 이렇게 하지 않으면 뗐을때도 RestTimer를 Set해서 계속해서 시간 측정함
	if (CurrentStamina < MaxStamina)
	{
		GetWorldTimerManager().SetTimer(RestTimeHandle, this, &APPlayer::UpdateBoost, 0.1f, true);
	}
}

// 시간이 흐름 주의
void APPlayer::UpdateBoost()
{
	// 스태미나가 남아서 달릴 수 있는 상태
	if (bIsBoost && bCanBoost)
	{
		// CurrentBoostTime : 누적해서 커짐 - 1차 함수
		float CurrentBoostTime = GetWorld()->GetTimeSeconds()-StartBoostTime;
		CurrentStamina = TempStamina - DecreaseStamina*CurrentBoostTime;
		if (CurrentStamina <= 0)
		{
			bCanBoost = false;
			StopBoost();
			CurrentStamina = 0;
			RestTime = GetWorld()->GetTimeSeconds();
		}
	}
	// 휴식 타임
	else if(!bIsBoost && CurrentStamina < MaxStamina)
	{
		float CurrentRestTime = GetWorld()->GetTimeSeconds() - RestTime;
		CurrentStamina = TempStamina + IncreaseStamina*CurrentRestTime;
		if (CurrentStamina > MaxStamina)
		{
			bCanBoost = true;
			GetWorldTimerManager().ClearTimer(RestTimeHandle);
			CurrentStamina = MaxStamina;
		}
	}
	// 방전
	else if(!bCanBoost)
	{
		float CurrentEmptyTime = GetWorld()->GetTimeSeconds() - RestTime;
		CurrentStamina = TempStamina + ZeroToHundredIncreaseStamina*CurrentEmptyTime;
		if (CurrentStamina > MaxStamina)
		{
			bCanBoost = true;
			GetWorldTimerManager().ClearTimer(RestTimeHandle);
			CurrentStamina = MaxStamina;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentStamina));
}

void APPlayer::UpPort()
{
	int32 Check = GI->GetPlayerManager()->CheckPortNum();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Check! : %d"), Check));
	SpringArm->TargetArmLength = 400 + GI->GetPlayerManager()->SpawnPort(0, this);
}

void APPlayer::UpHeroesFromArray(FName RowName)
{
	// 수정했음
	//int32 RandomInt = rand() % 5;
	//RowName = TEXT("Test1");
	int32 RealHeroArrayNum = GI->GetHeroManager()->CheckHeroNum();
	int32 MaximumArraySize = GI->GetHeroManager()->MaximumArraySize;
	if(RealHeroArrayNum >= MaximumArraySize)
	{
		return;
	}
	
	APHero* Hero = GI->GetHeroManager()->FindHero(RowName);
	if (Hero)
	{
		// hero 숫자가 더 많으면 지게도 쌓아올리기
		int32 PortNum = GI->GetPlayerManager()->CheckPortNum();
		int32 LastHeroIndex = GI->GetHeroManager()->LastHeroNum();
		if (PortNum <= LastHeroIndex)
		{
			UpPort();
		}
		// Player에서 처리
		USkeletalMeshComponent* SMComp = GetMesh();
		
		//GEngine->AddOnScreenDebugMessage(-1,3,FColor::Blue,FString::FromInt(PortNum));
		int32 HeroNum = Hero->GetHeroStats().Index;

		// Offset Array에서 해당 위치에 맞는 값 가져오기
		TArray<FVector> OffsetArray = GI->GetPlayerManager()->OffsetArray;
		FVector SocketLocation = SMComp->GetSocketLocation(FName("PortSocket"));
		UE_LOG(LogTemp, Log, TEXT("OffsetArray : %f, %f, %f"), OffsetArray[0].X, OffsetArray[0].Y, OffsetArray[0].Z);
		FVector RelativeOffset = SocketLocation.ForwardVector*(OffsetArray[HeroNum].X + -40.0f)
								+ SocketLocation.RightVector*(OffsetArray[HeroNum].Y)
								+ SocketLocation.UpVector*(OffsetArray[HeroNum].Z);
		FVector SpawnLocation = SocketLocation + RelativeOffset;
		
		Hero->SetActorLocation(SpawnLocation);
		Hero->SetActorRotation(GetActorRotation());

		Hero->AttachToComponent(SMComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("PortSocket"));
		Hero->SetActorRelativeLocation(RelativeOffset);
	}
	
	//GI->GetHeroManager()->SpawnHero(0, this);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::FromInt(RandomInt));
}

void APPlayer::DownPort()
{
	SpringArm->TargetArmLength = 400 + GI->GetPlayerManager()->DestroyPort();
}

void APPlayer::FObjectFinderInputManager()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>InputMappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Porter/Develop/Inputs/IMC_Player.IMC_Player'"));
	if (InputMappingContext.Object != nullptr)
	{
		IMC = InputMappingContext.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputMove(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Move.IA_Move'"));
	if (InputMove.Object != nullptr)
	{
		MoveAction = InputMove.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputLook(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Look.IA_Look'"));
	if (InputLook.Object != nullptr)
	{
		LookAction = InputLook.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputUp(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_UpPortTest.IA_UpPortTest'"));
	if (InputLook.Object != nullptr)
	{
		TestUpAction = InputUp.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputDown(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_DownPortTest.IA_DownPortTest'"));
	if (InputLook.Object != nullptr)
	{
		TestDownAction = InputDown.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputRun(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Run.IA_Run'"));
	if (InputLook.Object != nullptr)
	{
		RunAction = InputRun.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputSwap(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Swap24.IA_Swap24'"));
	if (InputLook.Object != nullptr)
	{
		SwapAction = InputSwap.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputHeroUp(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_UpHeroTest.IA_UpHeroTest'"));
	if (InputHeroUp.Object != nullptr)
	{
		TestHeroUpAction = InputHeroUp.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputHeroKillTest(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_KillHeroTest.IA_KillHeroTest'"));
	if (InputHeroKillTest.Object != nullptr)
	{
		TestHeroKill = InputHeroKillTest.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputTestDamage(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Damage.IA_Damage'"));
	if (InputTestDamage.Object != nullptr)
	{
		TestDamage = InputTestDamage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputSave(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_SaveTest.IA_SaveTest'"));
	if (InputSave.Object != nullptr)
	{
		TestSave = InputSave.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>InputOpen(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_OpenTest.IA_OpenTest'"));
	if (InputOpen.Object != nullptr)
	{
		TestOpen = InputOpen.Object;
	}
}

bool APPlayer::Die()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("YOU DIE"));
	bIsDead = true;
	return bIsDead;
}

void APPlayer::PlaySwap()
{
	// 0~PortNum
	TArray<int32> TempArray = {4, 3, 2, 1, 0, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	TArray<APHero*> HeroArray = GI->GetHeroManager()->SwapHeroes(TempArray);
}

// 영웅 배치, 지게 늘리거나 영웅 늘릴 때 등 수시로 사용하기  
bool APPlayer::CheckCondition()
{
	/*
	EntireWeight = HeroWeight * GI->GetHeroManager()->CheckHeroNum() + PortWeight * GI->GetPlayerManager()->CheckPortNum();

	int32 LastHeroIndex = -1;
	for (int32 i = HeroArray.Num() - 1; i>=0; --i)
	{
		if (HeroArray[i] != nullptr)
		{
			LastHeroIndex = i;
			break;
		}
	}

	bool IsOverWeight = EntireWeight > MaxWeight;
	bool IsOverIndex = LastHeroIndex > GI->GetPlayerManager()->CheckPortNum() - 1;
	
	// 무게 안넘고, 지게 수가 같거나 더 적고, 영웅 위치가 지게 가장 끝 위치보다 같거나 작아야 한다. 
	if (IsOverWeight || IsOverIndex) return false;
	else return true;
	*/
	return true;
}

void APPlayer::GetDamage()
{
	MinusHP(3);
}


void APPlayer::MakeHeroHPZero()
{
	TArray<APHero*> HeroArray = GI->GetHeroManager()->GetHeroArray();
	int32 IndexChecker = -1;
	for (int32 i=0; i<HeroArray.Num(); i++)
	{
		if (HeroArray[i] != nullptr)
		{
			IndexChecker = HeroArray[i]->GetHeroStats().Index;
			if (IndexChecker == 3)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player Manager")));
				HeroArray[i]->Destroy();
				HeroArray[i] = nullptr;
			}
		}
	}
	GI->GetHeroManager()->SetHeroArray(HeroArray);
}


void APPlayer::SaveSpawn()
{
	GI->GetPlayerManager()->SaveSpawnInformation();
	GI->GetHeroManager()->SaveSpawnInformation();
}

void APPlayer::OpenSpawn()
{
	GI->GetPlayerManager()->OpenSpawnInformation(this);
	GI->GetHeroManager()->OpenSpawnInformation(this);
	SpringArm->TargetArmLength = 400 + GI->GetPlayerManager()->AddCameraLength * GI->GetPlayerManager()->PortFloorArray[GI->GetPlayerManager()->CheckPortNum()];
}

void APPlayer::TempTest()
{
	//UpHeroesFromArray("Korean1Ssanggeom");
}
