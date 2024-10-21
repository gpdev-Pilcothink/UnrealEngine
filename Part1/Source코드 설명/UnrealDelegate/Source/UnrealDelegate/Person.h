// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDELEGATE_API UPerson : public UObject
{
	GENERATED_BODY()
	
public:
	UPerson();
	
	FORCEINLINE const FString& GetName() const { return Name; }
	// GetName() �ڿ� const�����ڸ� �ٿ��ִ°� ������ ���� �ȴٸ� ������ ����µ� �ֳ��ϸ� �������� �ʰڴٰ� const�� �����ߴµ� ���ϰ��� ���۷���(&)�� �ް� �ִ�.
	// ���ϰ��� ���۷����� ������ ���������� ������ �����ϱ� ������ const�����ڰ� ����.
	//���۷����� ��ȯ�Ҷ� const�� �����ϸ� FString �տ��� const�� ��ȯ���־�� �Ѵ�.
	FORCEINLINE void SetName(const FString& InName) { Name = InName; }

	FORCEINLINE class UCard* GetCard() const { return Card;}
	FORCEINLINE void SetCard(class UCard* InCard) { Card = InCard; }

protected:
	UPROPERTY()
	FString Name;

	//ī�� ��ü�� �����ϵ��� �����ϱ�
	//Card* Card;
	/*
	�츮�� ���� �̷��� ī�忡 ���� ��ü�� �����ϱ� ���ؼ��� ����� ���� �����ؾ� �Ѵ�.
	������ �츮�� �������� ���迡 ���� ���� ���� �ؼ� ���� ������ �ϴ� ���� ����.
	���� ������ �ϸ� ����� �������� �ʰ� ������Ʈ�� �����ͷ� �����ϱ� ������
	�츮�� ��Ȯ�� �����δ� �� �� ������ ������ ũ�⸦ ������ ������ ���� ������ ���� �������� ���� �� �ִ�.
	*/

	// �̺κк��� ���漱��
	/*
	UPROPERTY()
	class UCard* Card; 
	*/
	//�̷��� �𸮾� ������Ʈ�� �����Ͽ� �������� ���踦 ���� ��.
	/*
	�̹����� 4���������� �����̾�����
	5�������ʹ� �ٸ� ������� �����϶�� ���ο� ǥ���� ����
	�𸮾� ���� 5 ���̱׷��̼� ���̵� (4->5 �ٲܶ� ��������) �� ����
	c++ ������Ʈ ������ ������Ƽ ��� �׸��� ���� ������ ����ϴ� ���� ������ UPROPERTY������ ���������Ͱ� �־���
	������� TObjectPtr�� ����Ͽ� �����϶�� ������ �ִ�.
	�׷��� ���� ����������, ��� �����ͷ� ����� �͵��� TObjectPtr�̶�� ���ø����� ���μ� �����϶�� �Ǿ� �ִ�.
	���� ���� ���� �κи� �̷��� TObjectPtr�� ����ϰ� �����ο����� �����͸� ����ص� ū ������ ���ٰ� ����Ѵ�.
	*/

	//������� ��¥ �𸮾� 5���� ���漱��
	UPROPERTY()
	TObjectPtr<class UCard> Card;


	
};
