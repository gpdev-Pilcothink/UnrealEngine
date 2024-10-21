// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LessonInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULessonInterface : public UInterface //타입 정보를 관리하기 위해 생성된 클래스
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALCOMPOSITION_API ILessonInterface //우리가 실제로 구현해야할 부분임.
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void DoLesson()
	{
		UE_LOG(LogTemp, Log, TEXT("수업에 입장합니다."));
	}

	//
};
