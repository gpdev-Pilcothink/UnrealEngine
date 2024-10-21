// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Card.generated.h"

UENUM() //�� ��ũ�θ� �����ϸ� ������ ��ü�� ���� ������ �𸮾� ������ �ľ��Ͽ� �츮�� ������ ������ ������ �� �ִ�.
enum class ECardType : uint8 //������ Ÿ���� �׻� ���λ�E�� ���̰�, �������� ������ �⺻ Ÿ���� ���� uint8�� ���� ����Ʈ ���·� ���ش�.
{
	Student = 1 UMETA(DisplaName = "For Student"), //�𸮾� ��Ŀ��� 
	Teacher UMETA(DisplaName = "For Teacher"),
	Staff UMETA(DisplaName = "For Staff"),
	Invalid //�̷��� �������� �� �����͸��� ��Ÿ�������� ���� ���� �� �ְ� �ڵ忡�� ����Ҽ� �ִµ� �̷��� �������� �����Ǹ� ECardType�� ���������Ѵ�.
};

/**
 * 
 */
UCLASS()
class UNREALCOMPOSITION_API UCard : public UObject
{
	GENERATED_BODY()
	
public:
	UCard();
	
	ECardType GetCardType() const { return CardType; }
	void SetCardType(ECardType InCardType) {CardType = InCardType;}

private:

	UPROPERTY();
	ECardType CardType;

	UPROPERTY();
	uint32 Id; //�̹� �������� ��� ����
	
};
