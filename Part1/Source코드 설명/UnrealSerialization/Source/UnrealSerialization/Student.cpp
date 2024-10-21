// Fill out your copyright notice in the Description page of Project Settings.


#include "Student.h"

UStudent::UStudent()
{
	Order = -1;
	Name = TEXT("홍길동");
}

void UStudent::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	//수퍼에서는 언리얼 오브젝트가 가져야할 기본적인 정보를 알아서 처리해주고
	//우리는 밑에 코드처럼 프로퍼티의 순번만 지정해주면 된다.
	Ar << Order;
	Ar << Name;
}
