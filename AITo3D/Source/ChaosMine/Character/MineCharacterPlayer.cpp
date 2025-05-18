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
	* ĳ���� �⺻ ���� ����
	****************************************/
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true; //�ɱ� Ȱ��ȭ
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanSwim = true;
	// Camera
	/**********************************************************
	* 1��Ī&&3��Ī ���̴�.
	* 1��Ī�϶� ĳ������ ��Ʈ���� ���� �پ�� ������ 3��Ī�� �ƴϴ�.
	* 3��Ī�϶� SpringArm�� �̿��ϱ� ������ CameraBoom������ �ʿ��ϴ�.
	* 1��Ī�� 3��Ī�� ���� �ڼ��� ������ BeginPlay���� SetCharacterControl()�Լ��� ���ؼ� ���õȴ�.
	***********************************************************/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = true; //1��Ī�� 3��Ī�� ��� true�� �ٵ� Ȥ���� ���� ���� ���ۿ��� ���ͺ���� ������ ���ɼ��� �����صΰ� ������ ���¿��ٰ��� �����س���.


	// Input
	/**********************************************************
	* 1��Ī�� ��� ���� �Է½ý����� ����� (1��Ī �������Ʈ ���� ���� �Է� �ý����� ����ϰ� ������ Ȯ�� ����)
	* ConstroctorHelpers�� ����Ͽ� ���� �Է� �ý����� ��ü���� �����ͼ� ��������.
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


	// �ϴ� ���� ��带 FirstPerson���� ���� (BeginPlay�����ؼ� �ٷ� ����)
	CurrentCharacterControlType = ECharacterControlType::FirstPerson;
}

void AMineCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� ��, Data Asset�� 1��Ī �������� ����
	SetCharacterControl(ECharacterControlType::FirstPerson);
}

void AMineCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//������Ʈ ���ÿ��� ���� �Է½ý����� ����Ѵ� �߱⿡ �ݵ�� üũ �������.
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
	if(CurrentCharacterControlType == ECharacterControlType::FirstPerson) //1��Ī
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
			TEXT("head") // Ȥ�� CharacterControlData->FirstPersonSocketName
		);
	}
	else if(CurrentCharacterControlType == ECharacterControlType::ThirdPerson) //3��Ī ����.
	{
		UE_LOG(LogTemp, Warning, TEXT("ThirdPerson ViewMode"));
		if (CameraBoom)
		{
			// SpringArm&&Camera
			CameraBoom->SetActive(true);

			// Data Asset���� ������ ������ ����
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
	if(!GetCharacterMovement()->IsFalling()) //�������� ���߿��� �ɱ� ��� �ȵ�.
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
