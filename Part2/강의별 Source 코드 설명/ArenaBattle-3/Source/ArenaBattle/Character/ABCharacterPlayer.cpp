// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ABCharacterControlData.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	//ī�޶� ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	/*
	���������� ��� ��Ʈ ������Ʈ�� ������.
	���̴�4m
	CameraBoom->bUsePawnControlRotation = true;�� ȸ���� ���� �������� ���� ī�޶� �ٷ궧 �ڼ��� ����
	*/


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	/*
	ī�޶� ������Ʈ�� �������ϰ� ī�޶� �������� ������ �Ͽ� ������ ��.
	������ �Ҷ� Ư�� ��ġ�� �����ϴ°��� �ƴ� SocketName�̶�� �ϴ� �̸� �����ڸ� �������شٸ� ������ ���� ���� �ڵ����� �޶�ٰ� ��.
	���� �̸��� F12Ű�� ���� ���� Ȯ�� �����ϴ�.
	*/

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangerControl.IA_ChangerControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}


	CurrentCharacterControlType = ECharacterControlType::Quater;
	

}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
	//SetCharacterControl�� ȣ���Ͽ� ���ͺ� ��Ʈ�ѷ� �ٲ�ġ�� �ϵ��� �����ϸ� �ȴ�.
}



void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	//���⼭ �Է��� ������ �ռ� �����̵忡�� EnhancedInputComponent�� ����Ѵ� ��� �����ߴ�. ���� ���࿡ �̰��� ������ ���� ��찡 �������� �ִµ� 
	//������ �ʴ°�� CastChecked�� ����Ͽ� �ݵ�� EnhancedInputComponent�� ����ϵ��� �ڵ带 ����.
	//���������� ĳ���� �Ǿ��ٸ� ���� �ռ��� ������ �𸮾� ������Ʈ��� Move�� Look�Լ��� ���ؼ� ���ε��� �����ϰ� 
	//JumpAction�� ��쿣 ������ ĳ���Ͱ� �����ϴ� Jump�Լ��� StopJumping�Լ��� ���� �����Ͽ� ���õ� �׼��� ���़�ϵ��� ������ ���� �ش�.
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangerCharaceterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuaterMove);
}
/* �� �Լ��� ���� ����� 3�� �߰��ؾ� �Ѵ�.
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
*/

void AABCharacterPlayer::ChangerCharaceterControl()
{
	//VŰ�� ���� ��Ʈ���� �����ϴ� �Լ��� ��� ENUM���� �˻��Ͽ� �ݴ��� ENUM���� ����ǵ��� �����ϰ� ������ �ڼ��Ѱ� SetCharacterControl���⼭ ����

	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UABCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl); //��Ʈ�� �����Ͱ� �ݵ�� �־�� �ϱ⿡ check�Լ� ������.

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController()); //��Ʈ�ѷ� �ݵ�� �����;� �ؼ� CastChecked �����.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//�츮�� ���� ��ǲ ���� ���ؽ�Ʈ�� ��ϴ� Subsystem�� �����´�.
		Subsystem->ClearAllMappings(); //�ٲ�ġ�� �ϱ����� ������ �ִ� �������� �����ش�.
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}


void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	//FInputActionValue���� X�� Y���� ������ �Ŀ� �̸� �����Ʈ ������Ʈ�� �����Ͽ� ���������� ĳ���͸� �̵���Ű�ų� 
	//��Ʈ�ѷ��� ȸ���� ���������ν� ���������� �ش� ��Ʈ�ѷ��� �ٶ󺸵��� �����Ͽ���.
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AABCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	//FInputActionValue���� X�� Y���� ������ �Ŀ� �̸� �����Ʈ ������Ʈ�� �����Ͽ� ���������� ĳ���͸� �̵���Ű�ų� 
	//��Ʈ�ѷ��� ȸ���� ���������ν� ���������� �ش� ��Ʈ�ѷ��� �ٶ󺸵��� �����Ͽ���.
	FVector2D MovementVector = Value.Get<FVector2D>(); //���� �����Ʈ �ޱ�
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f; //ũ�� 1�� ����
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f); //X�� Y���� Modifiers�� �����߱� ������ �״�� �ֵ� ��.
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator()); 
	//ControlRotation��  MoveDirection(foward����)���� �������ָ� �����Ʈ ������Ʈ���� ������ �ɼǿ� ���� ĳ���Ͱ� �ڵ����� �̵��ϴ� �������� ȸ���ϰ� �ȴ�.
	AddMovementInput(MoveDirection, MovementVectorSize);
}


