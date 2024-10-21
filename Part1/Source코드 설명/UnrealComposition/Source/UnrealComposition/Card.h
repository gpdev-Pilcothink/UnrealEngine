// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Card.generated.h"

UENUM() //이 매크로를 선언하면 열거형 객체에 대한 정보를 언리얼 엔진이 파악하여 우리가 유용한 정보를 가져올 수 있다.
enum class ECardType : uint8 //열거형 타입은 항상 접두사E를 붙이고, 열거형이 가지는 기본 타입은 보통 uint8과 같이 바이트 형태로 해준다.
{
	Student = 1 UMETA(DisplaName = "For Student"), //언리얼 방식에는 
	Teacher UMETA(DisplaName = "For Teacher"),
	Staff UMETA(DisplaName = "For Staff"),
	Invalid //이렇게 열거형의 각 데이터마다 메타정보들을 집어 넣을 수 있고 코드에서 사용할수 있는데 이렇게 열거형이 지정되면 ECardType을 만들어줘야한다.
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
	uint32 Id; //이번 예제에서 사용 안함
	
};
