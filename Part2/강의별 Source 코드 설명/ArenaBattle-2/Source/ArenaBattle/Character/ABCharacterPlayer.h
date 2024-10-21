// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "InputActionValue.h"
#include "ABCharacterPlayer.generated.h"


/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase
{
	GENERATED_BODY()
	
public:
	AABCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	//BeginPlay()�� ���⼭ �Է� ���� ���ؽ�Ʈ�� �Ҵ��ϴ� ������ �����Ұ���

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//SetupPlayerInputComponent�� �𸮾� ������ Input System���� �Է¾׼ǰ� move�� Look �Լ��� ���⼭ ���� �����ִ°��� �̸� �Ϸ����ٰ���.
	//������� �ؼ� �Է� ������ ������ְ� ���������� �̰��� Ű���带 ���ؼ� �Է��� ������ Ű�е带 ���ؼ� �������� BeginPlay�� DefaultMappingContext�� ���ؼ� ������� �ٰ���.


// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USpringArmComponent> CameraBoom;
	//���������̶�� ī�޶� �������ִ� ������ ������ �ϴ� ������Ʈ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UCameraComponent> FollowCamera;
	//�̰� ��¥ ī�޶� ������Ʈ 

	/*�� �� ������Ʈ�� ���� Meta��� �ϴ� �߰� �����ڸ� �߰��� �����ߴµ� 
	�� ��쿡�� private�� ������ �𸮾� ������Ʈ ��ü���� �������Ʈ�� ���� �����ϵ��� ���ִ� Ư���� �������̴�. */


// Input Section
protected:
	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta=(AllowPrivateAccess = "true"));
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta=(AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta=(AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta=(AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> LookAction;
	/*��ü���� ������ ����ѵ� �� ���µ��� ��쿡�� �ٸ� �������� ���� �� �� �ֵ��� �����ϱ� ���� �����ڸ� EditAnyWhere�� ������
	�Ѱ��� �������ؽ�Ʈ�� ������ �׼��� �ִµ� �̰͵��� 3��Ī ���ø��� �����ϴ°��� �״�� �����.*/

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& value);
	//FInputActionValue�� �ޱ� ���ؼ��� #include "InputActionValue.h" ����߰��ؾ���.
	//���� ArenaBattle.Build.cs�� ���� EnhancedInput ��� �߰� ����� �۵���.

};
