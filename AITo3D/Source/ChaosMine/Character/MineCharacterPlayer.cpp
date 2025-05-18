// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MineCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MineCharacterControlData.h"

AMineCharacterPlayer::AMineCharacterPlayer()
{
	/***************************************
	* 캐릭터 기본 설정 변경
	****************************************/
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true; //앉기 활성화
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanSwim = true;
	// Camera
	/**********************************************************
	* 1인칭&&3인칭 폰이다.
	* 1인칭일땐 캐릭터의 컨트롤이 폰과 붙어야 하지만 3인칭은 아니다.
	* 3인칭일땐 SpringArm을 이용하기 때문에 CameraBoom세팅이 필요하다.
	* 1인칭과 3인칭에 대한 자세한 셋팅은 BeginPlay에서 SetCharacterControl()함수에 의해서 세팅된다.
	***********************************************************/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = true; //1인칭과 3인칭은 모두 true임 근데 혹여나 차후 게임 제작에서 쿼터뷰까지 제연할 가능성을 염두해두고 데이터 에셋에다가는 구현해놨음.


	// Input
	/**********************************************************
	* 1인칭의 경우 향상된 입력시스템을 사용함 (1인칭 블루프린트 보면 향상된 입력 시스템을 사용하고 있음을 확인 가능)
	* ConstroctorHelpers를 사용하여 향상된 입력 시스템의 객체들을 가져와서 배정해줌.
	***********************************************************/

	//movment
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ChaosMine/ViewPerson/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ChaosMine/ViewPerson/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ChaosMine/ViewPerson/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSprintRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ChaosMine/ViewPerson/Input/Actions/IA_Sprint.IA_Sprint'"));
	if (nullptr != InputActionSprintRef.Object)
	{
		SprintAction = InputActionSprintRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionCrouchRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ChaosMine/ViewPerson/Input/Actions/IA_Crouch.IA_Crouch'"));
	if (nullptr != InputActionCrouchRef.Object)
	{
		CrouchAction = InputActionCrouchRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ChaosMine/ViewPerson/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	//Attack
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ChaosMine/ViewPerson/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}


	// 일단 시작 모드를 FirstPerson으로 지정 (BeginPlay에의해서 바로 적용)
	CurrentCharacterControlType = ECharacterControlType::FirstPerson;
}

void AMineCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시, Data Asset의 1인칭 세팅으로 통일
	SetCharacterControl(ECharacterControlType::FirstPerson);
}

void AMineCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//프로젝트 세팅에서 향상된 입력시스템을 사용한다 했기에 반드시 체크 해줘야함.
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//MovementInput Bind
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMineCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMineCharacterPlayer::StartSprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMineCharacterPlayer::StopSprint);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMineCharacterPlayer::StartCrouch);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMineCharacterPlayer::StopCrouch);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMineCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AMineCharacterPlayer::ChangeCharacterControl);

	//AttackInputBind
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMineCharacterPlayer::Attack);
}

void AMineCharacterPlayer::ChangeCharacterControl()
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeCharacterControl() called. CurrentCharacterControlType = %d"),
		static_cast<int32>(CurrentCharacterControlType)
	);
	if (CurrentCharacterControlType == ECharacterControlType::FirstPerson)
	{
		UE_LOG(LogTemp, Warning, TEXT("change Third Person"));
		SetCharacterControl(ECharacterControlType::ThirdPerson);
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else if (CurrentCharacterControlType == ECharacterControlType::ThirdPerson)
	{
		UE_LOG(LogTemp, Warning, TEXT("change First Person"));
		SetCharacterControl(ECharacterControlType::FirstPerson);
	}
}

void AMineCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	CurrentCharacterControlType = NewCharacterControlType;
	UMineCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
}

void AMineCharacterPlayer::SetCharacterControlData(const UMineCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	UE_LOG(LogTemp, Warning, TEXT("SetCharacterControlData() called. CurrentType=%d"), (int32)CurrentCharacterControlType);
	if(CurrentCharacterControlType == ECharacterControlType::FirstPerson) //1인칭
	{
		UE_LOG(LogTemp, Warning, TEXT("FirstPerson ViewMode"));

		//Camera
		if (CameraBoom)
		{
			CameraBoom->SetActive(false);
		}

		FollowCamera->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("head") // 혹은 CharacterControlData->FirstPersonSocketName
		);
	}
	else if(CurrentCharacterControlType == ECharacterControlType::ThirdPerson) //3인칭 설정.
	{
		UE_LOG(LogTemp, Warning, TEXT("ThirdPerson ViewMode"));
		if (CameraBoom)
		{
			// SpringArm&&Camera
			CameraBoom->SetActive(true);

			// Data Asset에서 가져온 설정값 적용
			CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
			CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
			CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
			CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
			CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
			CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
			CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
		}
		FollowCamera->AttachToComponent(
			CameraBoom,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			USpringArmComponent::SocketName
		);
	}
}

void AMineCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AMineCharacterPlayer::StartSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Start"));
	GetCharacterMovement()->MaxWalkSpeed = GetMovement(ESpeedGroup::Basic, EMovementType::Sprint);
	GetCharacterMovement()->MaxWalkSpeedCrouched = GetMovement(ESpeedGroup::Basic, EMovementType::CrouchSprint);
}

void AMineCharacterPlayer::StopSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Stop"));
	GetCharacterMovement()->MaxWalkSpeed = GetMovement(ESpeedGroup::Basic, EMovementType::Walk);
	GetCharacterMovement()->MaxWalkSpeedCrouched = GetMovement(ESpeedGroup::Basic, EMovementType::CrouchWalk);
}

void AMineCharacterPlayer::StartCrouch()
{
	if(!GetCharacterMovement()->IsFalling()) //떨어지는 도중에는 앉기 기능 안됨.
	{
		UE_LOG(LogTemp, Warning, TEXT("Crouch Start"));
		Crouch();
	}
}

void AMineCharacterPlayer::StopCrouch()
{
	UE_LOG(LogTemp, Warning, TEXT("Crouch Stop"));
	UnCrouch();
}

void AMineCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AMineCharacterPlayer::Attack()
{
	ProcessComboCommand();
}
