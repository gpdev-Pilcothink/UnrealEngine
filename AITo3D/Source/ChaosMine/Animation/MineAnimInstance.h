// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MineAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSMINE_API UMineAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMineAnimInstance();

protected:

	/*****************
	* �̺�Ʈ �Լ� ����
	******************/
	virtual void NativeInitializeAnimation() override; //ó�� ������ �� �ѹ� ȣ��

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //������ ���� ȣ��

	/*****************
	* ���� ����
	******************/

	//AnimInstance ������ ������ ĳ���� ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class AMineCharacterBase> Owner;

	//CharacterMovement�� ���� ��ü�����͸� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	//�� ������ ����
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	FVector Velocity; //���� ĳ������ �ӵ��� ����

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed; //������ ���ִ��� �ɾ��ִ��� �˻�

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float ForwardValue; //�������� �ӷ�

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float RightValue; //������ ���ִ��� �ɾ��ִ��� �˻�

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float SwimmingSpeed; //���ӿ����� �ӷ� (���� ���ɽ� Ȱ���� ������)

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1; // uint8s�� 1����Ʈ������ ��Ʈ�÷��׷� ���� 1��Ʈ�� ���.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsStanding : 1; //standing�������� �˻�.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1; //�������� �ִ��� �˻�

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1; //�����ϰ� �ִ��� �˻�

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsCrouching : 1; //�ɰ� �ִ��� �˻�.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsSwimming : 1; //�����ϰ� �ִ��� �˻�

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold; //���������� �˻���.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold; //���������� �˻���.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float SwimmingThreshold; //���������� �˻���.

};
