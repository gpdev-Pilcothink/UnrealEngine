// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class UNREALMEMORY_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	TObjectPtr<class UStudent> NonPropStudent;

	UPROPERTY()
	TObjectPtr<class UStudent> PropStudent;


	//이 밑에서부턴 자료구조 컨테이너 안에 언리얼 오브젝트 안전하게 관리해보기
	TArray< TObjectPtr<class UStudent>> NonPropStudents;

	UPROPERTY()
	TArray< TObjectPtr<class UStudent>> PropStudents;

	class FStudentManager* StudentManager = nullptr;
		//일반 객체는 UPROPERTY() 사용을 하지 못하기 때문에 값 보장을 하지 못한다 따라서 nullptr로 초기화 하는게 좋다.

	
};
