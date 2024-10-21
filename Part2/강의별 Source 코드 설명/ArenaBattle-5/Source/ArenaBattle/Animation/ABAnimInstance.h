// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	UABAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	//NativeInitializeAnimation()�Լ��� AnimInstance�� ó�� �����ɶ� �ѹ� ȣ��

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//NativeUpdateAnimation�Լ��� �����Ӹ��� ȣ��ȴ�.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;
	//AnimInstance�� �����ϰ� �ִ� ĳ������ ������ ��� �ִ� Owner ���� �߰�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;
	//ĳ���� �����Ʈ ������Ʈ�� ���� ��ü �����͸� ��� Movement ���� �߰�.

	/*������� �� ���� �߰� VisibleAnywhere->EditAnywhere�� ��ü*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;
	//���� ĳ������ �ӵ��� ������ �� �ִ� Velocity ���� �߰�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;
	//�������� �ӷ��� ������ �� �ִ� GroundSpeed�� ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;
	//���� Idle�������� ��Ÿ���� ���� �Ҹ��� ����
	//��Ÿ���� ����� �𸣱� ������ �𸮾� ������ ������������ �����ϵ� �տ� ���λ� 'b'�� ���̰� ��Ʈ �÷��׸� �߰��� �޾��ش�.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;
	//�����̰� �ִ��� �����ִ��� �����ϴ� MovingThreshould �߰�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;
	//�������� �ִ��� �˻��ϴ� bIsFalling ���� �߰�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;
	//���� �����ϰ� �ִ����� �˻��ϴ� bIsJumping ���� �߰�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;
	//���� ���������� ��Ÿ���� JumpingThreshould ���� �߰�.
};
