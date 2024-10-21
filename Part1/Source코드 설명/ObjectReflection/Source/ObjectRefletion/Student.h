// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.h"
#include "Student.generated.h"

/**
 * 
 */
UCLASS()
class OBJECTREFLETION_API UStudent : public UPerson
{
	GENERATED_BODY()
	
public:
	UStudent();
	virtual void DoLesson() override;
	
private:
	UPROPERTY();
	int32 Id;

};
/*
*person을 상속받아야 하는데 person을 상속받을려면 헤더파일을 include해야 한다.
*언리얼 오브젝트 헤더에서 다른 헤더를 include하게 되면 조심해야 하는데언리얼 헤더툴에서 generated.h 파일 밑에 include 했기 때문이다.
*따라서 언리얼 오브젝트에서 선언할때는 generated.h가 가장 밑에 있는것이기본 규칙이다.
*/

/*
* Person에서 구현된 DoLesson 함수를 실행한 후에 Student의 DoLesson을 진행할 것임.
*/