// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::Init()
{
	//**예제1: FName인 대소문자 구분이 없음**//
	FName key1(TEXT("PELVIS"));
	FName key2(TEXT("pelvis"));
	UE_LOG(LogTemp, Log, TEXT("FNAME 비교결과 : %s"), key1 == key2 ? TEXT("같음") : TEXT("다름"));
	

	//**예제2: FName 구성 방법
	for (int i = 0; i < 10000; ++i)
	{
		FName SearchInNamePool = FName(TEXT("pelvis"));
		/*
		이렇게 선언하게 되면, 생성자에 이
		문자열을 넣으면 FNAME은 문자열을 key로 변환하여 
		key가 전역풀에 있는지 조사하는 작업을 거치게 되며 
		이렇게 빈번한 작업이 일어나는데 있어서 오버헤드가 발생할 수 있다.
		결과적으로 조사해서 FName에 관련 key값만 저장하면 됨
		전에 한번만 선언해주거나 const이용을 하면 된다.
		*/

		const static FName StaticOnlyOnce(TEXT("pelvis"));
		/*
		이렇게하면 처음 초기화할때 데이터를 저장하고, 
		local static으로 선언했으니 그 다음부터 찾을일이 없다.

		나중에 게임 만들대 FName이 Tick과 같이 자주 실행되는 함수에 있는경우
		오버해드가 발생할 수 있으니 잘 알아두자.
		*/
	}
}
