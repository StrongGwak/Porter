// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/Engine.h"
#include "PHero.h"
#include "VectorTypes.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	bUseControllerRotationYaw = true; 

	FObjectFinderInputManager();
	MakeArrays();
	UpdateStats(PlayerAndHeroStats);
}

// Called when the game starts or when spawned
void APPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(IMC, 0);
		}
	}
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
	EnhancedInputComponent->BindAction(TestHeroUpAction, ETriggerEvent::Started, this, &APPlayer::UpHerosFromArray);
}

// 반드시 같은 크기의 행렬(15)만 온다는 가정하기 -> 따로 함수로 만들어야 함
// 일단 포트로 해놨는데, 플레이어로만 가능하게 바꿔야함. 플레이어 제작 후 생각
// 바꾸는건 무게 필요없음
void APPlayer::SwapHeroesByArray(TArray<int32> SwapArray)
{
	// SwapArray : 4 3 2 1 0 5~14 == 위치한 PortNum. 1번부터 시작한다. 0~14가 들어가있어야 한다.
	// SwapArray에는 index를 어떻게 바꾸어야 하는지가 들어가 있음
	TArray<APHero*> TempSwapArray;
	// TempSwapArray.Empty();
	TempSwapArray.Append(HeroBoxArray);
	for (int32 i=0; i<HeroBoxArray.Num(); ++i)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(SwapArray[i]));
		HeroBoxArray[i] = TempSwapArray[SwapArray[i]];
		
		// 실체 위치 변경
		if (HeroBoxArray[i] != nullptr)
		{
			HeroBoxArray[i]->SetActorLocation(
			   GetActorLocation()
			   + GetActorForwardVector()*OffsetArray[i].X
			   + GetActorRightVector()*OffsetArray[i].Y
			   + FVector(0, 0, OffsetArray[i].Z + HeroOffset)
			   );
		}
	}
}

// 블루프린트에서 업데이트 할 사항
void APPlayer::UpdateStats(FPlayerStatsStruct UpdateStat)
{
	PlayerAndHeroStats = UpdateStat;
	
	// 바로 적용해야 할 것들 나중에 따로 정리 필요하긴 함
	// 틱에서 검사하는게 아니니까 이 과정이 필요하다. 
	GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerWalkSpeed;

	// 현재 지게 수가 max를 넘게 될 경우 없애버리기
	// (수정 필요) 무게도 생각해야하는게?  
	while (PortNum > PlayerAndHeroStats.MaxPortNum)
	{
		DownPort();
	}
	MaxStamina = PlayerAndHeroStats.PlayerMaxStamina;
	DecreaseStamina = PlayerAndHeroStats.PlayerDecreaseStamina;
	IncreaseStamina = PlayerAndHeroStats.PlayerIncreaseStamina;
	ZeroToHundredIncreaseStamina = PlayerAndHeroStats.PlayerZeroToHundredIncreaseStamina;
	MaxWeight = PlayerAndHeroStats.MaxWeight;
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
		DownPort();
		CurrentHP = MaxHp;
	}
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
		GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerRunSpeed;
	}
}

void APPlayer::StopBoost()
{
	bIsBoost = false;
	RestTime = GetWorld()->GetTimeSeconds();
	TempStamina = CurrentStamina;
	GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerWalkSpeed;
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
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentStamina));
}

// 나중에 변수로 빼야함
void APPlayer::UpPort()
{
	SpawnPort(0);
}

// Hero 1개 생성 + 종류 추가
// HeroNum과 PortNum 구분해서 써야함
// 또한, HeroIndex라는 변수도 생각해야함 - 이 Index는 1부터 시작하고 ... <- 그냥 0부터 시작하게 하면 안돼? 다른걸 고쳐서
// 
void APPlayer::SpawnPort(int32 PortTypeIndex)
{
	if (PortNum < PlayerAndHeroStats.MaxPortNum)
	{
		FVector RelativeOffset = GetActorForwardVector()*OffsetArray[PortNum].X+ GetActorRightVector()*OffsetArray[PortNum].Y + FVector(0, 0, OffsetArray[PortNum].Z);
		FVector SpawnLocation = GetActorLocation() + RelativeOffset; 
		ACharacter* Port = GetWorld()->SpawnActor<ACharacter>(PortType[PortTypeIndex], SpawnLocation, GetActorRotation());
		
		if(Port)
		{
			PortArray[PortNum] = Port;
			Port->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
		PortNum++;
		SpringArm->TargetArmLength = 400 + PortFloorArray[PortNum] * AddCameraLength;

		Port->SetActorEnableCollision(false);
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(PortNum));
}


void APPlayer::UpHerosFromArray()
{
	int32 RandomValue = std::rand() % 3;
	SpawnHero(RandomValue);
}


void APPlayer::SpawnHero(int32 HeroTypeIndex)
{
	if (HeroNum < PlayerAndHeroStats.MaxPortNum && HeroNum < PortNum)
	{
		int32 TempPosition = -1;
		for (int32 i=0; i < HeroBoxArray.Num(); i++)
		{
			if (HeroBoxArray[i] == nullptr)
			{
				TempPosition = i;
				break;
			}
		}
		// 예외처리 - 이미 막지 않았나
		if (TempPosition == -1) return;
		
		FVector RelativeOffset =  GetActorForwardVector()*OffsetArray[TempPosition].X+ GetActorRightVector()*OffsetArray[TempPosition].Y + FVector(0, 0, OffsetArray[TempPosition].Z + HeroOffset);
		FVector SpawnLocation = GetActorLocation() + RelativeOffset;
		APHero* Heroes = GetWorld()->SpawnActor<APHero>(HeroType[HeroTypeIndex], SpawnLocation, GetActorRotation());
		
		if (Heroes)
		{
			HeroBoxArray[TempPosition] = Heroes;
			Heroes->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
		Heroes->SetActorEnableCollision(false);
		HeroNum++;
	}
}


// Hero 1개 파괴(가장 끝) - 지게의 수만큼 돌리며 서치 필요함
// HeroNum에 위치하지 않을 수 있다. <- 이부분이 가장 중요. 가장 큰 변경점
// 빈자리에 nullptr이 들어가있어야 한다. -> 거꾸로 검색하기
// 이거 포트 파괴가 아니라 Hero 파괴여야함. port는 남아있어야 한다.
void APPlayer::DownPort()
{
	if (PortNum > 0)
	{
		PortNum--;
		SpringArm->TargetArmLength = 400 + PortFloorArray[PortNum] * AddCameraLength;
		
		PortArray[PortNum]->Destroy();	// 실적용
		PortArray[PortNum] = nullptr;	// 내가 가진 Array에만 적용

		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHP));

		// 만약 PortNum에 영웅이 있다면
		if (HeroBoxArray[PortNum] != nullptr)
		{
			HeroBoxArray[PortNum]->Destroy();
			HeroBoxArray[PortNum] = nullptr;
			HeroNum--;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(PortNum));
}



// 생성자에서 미리 배열 만들어두기
// 여기 offset은 1부터 시작한다.  
void APPlayer::MakeArrays()
{
	int PortFloor = 0;
	for(int TempPortNum=0; TempPortNum<15+1; TempPortNum++)
	{
		// PortNum 수에 따른 카메라 레벨 == 쌓은 층 수
		PortFloor = 0;
		while (true)
		{
			if (TempPortNum <= PortFloor * (PortFloor + 1) / 2) break;
			else PortFloor++;
		}
		// 배열 추가
		PortFloorArray.Emplace(PortFloor);
		
		// 층수로 계산하는 스폰 위치 Offset
		float OffsetY = 0;
		float CurrentSpawnNum = TempPortNum - (PortFloor-1)*PortFloor/2;
		int32 ForCount = round(PortFloor);
		for (int32 i = 0; i<round(CurrentSpawnNum); i++)
		{
			if (i%2 == 1) OffsetY = -1*OffsetY - 2*PortWidth;
			else OffsetY *= -1;
		}
		if (ForCount%2 == 0) OffsetY += PortWidth;
		OffsetArray.Emplace(FVector(-100, OffsetY, PortFloor*PortHeight));
	}
	// index 0부터 적용시키기
	OffsetArray.RemoveAt(0);

	// Hero와 Port의 초기 15개 만들기
	HeroBoxArray.Init(nullptr, 15);
	PortArray.Init(nullptr, 15);
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
}

void APPlayer::Die()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("YOU DIE"));
}

void APPlayer::PlaySwap()
{
	// 0~PortNum
	ItemIndexArray = {4, 3, 2, 1, 0, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	SwapHeroesByArray(ItemIndexArray);
}

// nullptr이 아닌 원소의 수만 세는 함수
int32 APPlayer::CheckArrayNum(TArray<APHero*> CheckCharacterArray)
{
	int32 EntireArrayNum = CheckCharacterArray.Num();
	int32 Count = 0;
	for (int32 i=0;i<EntireArrayNum;++i)
	{
		if (CheckCharacterArray[i] != nullptr)
		{
			Count++;
		}
	}
	return Count;
}

// 영웅 배치, 지게 늘리거나 영웅 늘릴 때 등 수시로 사용하기  
bool APPlayer::CheckCondition()
{
	EntireWeight = HeroWeight * HeroNum + PortWeight * PortNum;
	HeroNum = CheckArrayNum(HeroBoxArray);

	int32 LastHeroIndex = -1;
	for (int32 i = HeroBoxArray.Num() - 1; i>=0; --i)
	{
		if (HeroBoxArray[i] != nullptr)
		{
			LastHeroIndex = i;
			break;
		}
	}

	bool IsOverWeight = EntireWeight > MaxWeight;
	bool IsOverIndex = LastHeroIndex > PortNum - 1;
	
	// 무게 안넘고, 지게 수가 같거나 더 적고, 영웅 위치가 지게 가장 끝 위치보다 같거나 작아야 한다. 
	if (IsOverWeight || IsOverIndex) return false;
	else return true;
}



