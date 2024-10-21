// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ArenaBattle/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	//�̰͵� ���������� ��Ʈ�ѷ��� �÷��̾� ��Ʈ�ѷ� �� �÷��̾ ������� ����� ���� ĳ���� Ŭ�����̱� ������ CastChecked�� ����� �Ѵ�.

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//���⼭ Subsystem�̶�� �ϴ� ��ǲ�ý����� �����ͼ� �������ؽ�Ʈ ������ �߰��� ���ش�.
		Subsystem->AddMappingContext(DefaultMappingContext, 0);//=> ���⿡ �켱������ �ǳ��� �����Ͽ� �پ��� �Է��� ��ġ���� �켱������ ���� �Էµ鿡 ���ؼ� �׼ǵ��� ���ε����� ������ �� �ֵ��� ������ �� �ִ�.
		//Subsystem->RemoveMappingContext(DefaultMappingContext); ==>������ ��Ÿ�ӿ��� RemoveMappingContext�Լ��� ���� ���ų� �߰��ϰų� �����Ӱ� ������ �����ϴ�.
	}
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
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Look);
}
/* �� �Լ��� ���� ����� 3�� �߰��ؾ� �Ѵ�.
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
*/

void AABCharacterPlayer::Move(const FInputActionValue& Value)
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

void AABCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

