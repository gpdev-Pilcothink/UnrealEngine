// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TFCharacterBase.h"
#include "InputActionValue.h"
#include "TFCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class THEIOSFLOWER_API ATFCharacterPlayer : public ATFCharacterBase
{
	GENERATED_BODY()
	
public:
	ATFCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	//BeginPlay()�� ���⼭ �Է� ���� ���ؽ�Ʈ�� �Ҵ��ϴ� ������ �����Ұ���

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//SetupPlayerInputComponent�� �𸮾� ������ Input System���� �Է¾׼ǰ� move�� Look �Լ��� ���⼭ ���� �����ִ°��� �̸� �Ϸ����ٰ���.
	//������� �ؼ� �Է� ������ ������ְ� ���������� �̰��� Ű���带 ���ؼ� �Է��� ������ Ű�е带 ���ؼ� �������� BeginPlay�� DefaultMappingContext�� ���ؼ� ������� �ٰ���.

/*CharacterControlSection*/
protected:
	void ChangeCharaceterControl(); //VŰ�� �������� ������ ����ǵ��� �ϴ� �Լ�.
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	//������ ������ �߻����� �� ��Ʈ�ѿ� ���õ� ��� ������ �������ִ� SetCharacterControl�Լ�
	virtual void SetCharacterControlData(const class UTFCharacterControlData* CharacterControlData) override;



// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USpringArmComponent> CameraBoom;
	//���������̶�� ī�޶� �������ִ� ������ ������ �ϴ� ������Ʈ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UCameraComponent> FollowCamera;
	//�̰� ��¥ ī�޶� ������Ʈ 

	/*�� �� ������Ʈ�� ���� Meta��� �ϴ� �߰� �����ڸ� �߰��� �����ߴµ�
	�� ��쿡�� private�� ������ �𸮾� ������Ʈ ��ü���� ��������Ʈ�� ���� �����ϵ��� ���ִ� Ư���� �������̴�. */


	// Input Section
protected:


	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnyWhere, bluePrintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> AttackAction;

	/*��ü���� ������ ����ѵ� �� ���µ��� ��쿡�� �ٸ� �������� ���� �� �� �ֵ��� �����ϱ� ���� �����ڸ� EditAnyWhere�� ������
	�Ѱ��� �������ؽ�Ʈ�� ������ �׼��� �ִµ� �̰͵��� 3��Ī ���ø��� �����ϴ°��� �״�� �����.*/

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& value);
	void QuaterMove(const FInputActionValue& Value);
	//FInputActionValue�� �ޱ� ���ؼ��� #include "InputActionValue.h" ����߰��ؾ���.
	//���� ArenaBattle.Build.cs�� ���� EnhancedInput ��� �߰� ����� �۵���.
	
	void Attack();

	ECharacterControlType CurrentCharacterControlType;
	//���� � �� �������� Ȯ���ϱ� ���ؼ� �ռ� ������ �������� Ȱ���Ͽ� CurrentCharacterControlType�� �������ְ� ���⿡ ���� �⺻���� Quater�� �ʱ�ȭ

};