// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Algo/Accumulate.h" //합을 구하는 알고리즘 라이브러리

FString MakeRandomName()
{
	TCHAR FirstChar[] = TEXT("김이박최");
	TCHAR MiddleChar[] = TEXT("상혜지성");
	TCHAR LastChar[] = TEXT("수은원연");
	//총 64개의 조합이 이루어짐

	TArray<TCHAR> RandArray;
	/*
	TArray같은 경우 TCHAR배열을 포함하고 있는 컨테이너이다.
	따라서 포인터 값을 넘겨주면 반환 값을 FString으로 지정해줬기 
	때문에 자동으로 FString이 만들어져서 반환이 될 것이다.
	*/
	/*
	
	*/
	RandArray.SetNum(3);
	RandArray[0] = FirstChar[FMath::RandRange(0, 3)];
	RandArray[1] = MiddleChar[FMath::RandRange(0, 3)];
	RandArray[2] = LastChar[FMath::RandRange(0, 3)];

	return RandArray.GetData(); //이부분의 GetData를 통해 자동으로 String이 만들어지도록 한다.
}


void UMyGameInstance::Init()
{
	Super::Init();

	const int32 ArrayNum = 10;
	TArray<int32> Int32Array;

	for (int32 ix = 1; ix <= ArrayNum; ++ix)
	{
		Int32Array.Add(ix);
		// 성능을 신경 쓴다면 Add 대신 Emplace 써도 됨
	}

	Int32Array.RemoveAll( //RemoveAll의 조건은 보통 람다 함수로 작성한다.
		[](int32 val)
		{
			return val % 2 == 0;
		}
	);

	Int32Array += {2, 4, 6, 8, 10}; //오퍼레이터 연산으로 다시 집어 넣을 수 있다.

	/*< 이 밑에 부분은 c스타일로 로우하게 메모리에 접근하는 방법을 설명한다> */
	TArray<int32> Int32ArrayCompare;
	int32 CArray[] = { 1, 3, 5 ,7 ,9 , 2 , 4, 6, 8, 10 };
	Int32ArrayCompare.AddUninitialized(ArrayNum); //AddUninitialized를 통해 초기화 되지 않은 데이터를 빠르게 넣어준다.
	FMemory::Memcpy(Int32ArrayCompare.GetData(), CArray, sizeof(int32) * ArrayNum);
	// Int32ArrayCompare에 있는 포인터를 가져온다음에 앞서 제작한 Arraynum 크기만큼 복제를 해준다.


	ensure(Int32Array == Int32ArrayCompare);
	//만약 실행할때 문제가 있다면 ensure함수에 의해서 빨간색 에러가 뜰 것이다.
	//하지만 ensure에 너무 의존하면 불편하고 직접 Array가 구성되어있는지 보고싶을땐 브레이크 포인터를 걸어서 디버깅을 해보자.
	/*
	디버깅을 위해서 여러가지 모드를 제공하지만 그냥 F5 하고 실행하면 실행 흐름은 추적이 가능하지만 내부 내용까지 추적이 가능한
	상세한 모드로 디버깅이 되지 않는다 따라서 빌드 모드를 바꿔주어야 한다.
	상단의 Development Editor를 DebugGame Editor로 바꿔야 한다.
	*/

	/* ***언리얼 엔진은 여러 가지 알고리즘 라이브리러리를 언리얼엔진 컨테이너에 맞게 제공하는데 대표적으로 합을 구하는 알고리즘이 있다.****/
	
	// <보통  배열의 합 구하기>
	int32 Sum = 0;

	for (const int32& Int32Elem : Int32Array)
	{
		Sum += Int32Elem;
	}
	ensure(Sum == 55);

	//다음 위처럼 구문을 만들면 복잡한데 이때 accumulate 함수를 사용해보자

	int32 SumByAlgo = Algo::Accumulate(Int32Array, 0); //두번째 인자는 시작값으로 0으로 지정하자
	ensure(Sum == SumByAlgo);

	TSet<int32> Int32Set;
	for (int32 ix = 1; ix <= ArrayNum; ++ix)
	{
		Int32Set.Add(ix);
	}

	Int32Set.Remove(2);
	Int32Set.Remove(4);
	Int32Set.Remove(6);
	Int32Set.Remove(8);
	Int32Set.Remove(10);

	Int32Set.Add(2);
	Int32Set.Add(4);
	Int32Set.Add(6);
	Int32Set.Add(8);
	Int32Set.Add(10);

	//******구조체 학습부분****** 300명정도의 학생 이름을 생성해볼거임


	const int32 StudentNum = 300;
	for (int32 ix = 1; ix <= StudentNum; ++ix)
	{
		StudentsData.Emplace(FStudentData(MakeRandomName(), ix));
		//구조체로 데이터를 집어 넣는 경우에는 복사 비용이 발생하기 때문에 Emplace를 사용하여 신경 써주는게 좋다.
	}

	TArray<FString> AllStudentNames;
	//ranged for문 같은 for문을 이용하여 300개의 이름 구조체를 옮기겠다는 생각을 할 것이다.
	//하지만 앞선 강의에서 설명했듯 알고리즘 라이브러리를 사용하면 보다 간편하게 옮길 수 있다.
	Algo::Transform(StudentsData, AllStudentNames,
		[](const FStudentData& Val)//TArray에 선언한 데이터 타입을 첫번째 인자로 가져 온다.
		{
			return Val.Name; //옮길 데이터 타입에 대한 FString값을 리턴 값으로 주면 된다.
		}
		//이를 통해 StudentData의 TArray를 String TArray를 이 함수 호출 한번으로 처리 할 수 있게 된다.
	);

	UE_LOG(LogTemp, Log, TEXT("모든 학생 이름의 수 : %d"), AllStudentNames.Num());
	
	TSet<FString> AllUniqueNames; //TSet은 중복이 없다.
	Algo::Transform(StudentsData, AllUniqueNames,
		[](const FStudentData& Val)
		{
			return Val.Name; 
		}
		
	);
	UE_LOG(LogTemp, Log, TEXT("중복 없는 학생 이름의 수 : %d"), AllUniqueNames.Num());



	//*********이 부분부터는 TMap 실습**********

	Algo::Transform(StudentsData, StudentsMap,
		[](const FStudentData& Val)
		{
			return TPair<int32, FString>(Val.Order, Val.Name);
				//여기서 리턴값은 Map과 똑같은 타입인 TPair로 반환해줘야 한다.
		}

	);
	UE_LOG(LogTemp, Log, TEXT("순번에 따른 학생 맵의 레코드 수 : %d"), StudentsMap.Num());


	//중복이 안될경우
	TMap<FString, int32> StudentsMapByUniqueName;

	Algo::Transform(StudentsData, StudentsMapByUniqueName,
		[](const FStudentData& Val)
		{
			return TPair<FString, int32>(Val.Name, Val.Order);
	//여기서 리턴값은 Map과 똑같은 타입인 TPair로 반환해줘야 한다.
		}

	);
	UE_LOG(LogTemp, Log, TEXT("이름에 따른 학생 맵의 레코드 수 : %d"), StudentsMapByUniqueName.Num());
	/*이름에 따른 중복 안되는 레코드가 63개인데 랜덤 함수가 다르게 실행되서 처음에 나왔던 64개가 안될 것이다. */
	//중복을 허용하고싶을땐
	TMultiMap<FString, int32> StudentMapByName;
	Algo::Transform(StudentsData, StudentMapByName,
		[](const FStudentData& Val)
		{
			return TPair<FString, int32>(Val.Name, Val.Order);
			//여기서 리턴값은 Map과 똑같은 타입인 TPair로 반환해줘야 한다.
		}

	);

	UE_LOG(LogTemp, Log, TEXT("순번에 따른 학생 멀티맵의 레코드 수 : %d"), StudentMapByName.Num());

	/*어떤 이름을 가진 학생이 몇명인지 정보 뽑아보기 실습*/
	const FString TargetName(TEXT("이혜은"));
	TArray<int32> AllOrders;
	StudentMapByName.MultiFind(TargetName, AllOrders);

	UE_LOG(LogTemp, Log, TEXT("이름이 %s인 학생 수 : %d"),*TargetName, AllOrders.Num());

	/*이번에는 FStudentData를 TSet에 쓰기 위해서 어떤 작업을 하면 되는지에 대해서 살펴보겠다.*/
	
	TSet<FStudentData> StudentsSet;
	for (int32 ix = 1; ix <= ArrayNum; ++ix)
	{
		StudentsSet.Emplace(FStudentData(MakeRandomName(), ix));
	}
	/*
	헤더파일 수정 없이 이상태로 실행 시키면 에러가 많이 뜨는데 
	우리가 지정한 커스텀 구조체에 대한 GetTypeHash 함수가 지정되어 있지 않아
	해쉬값을 만들 수가 없다는 에러 메시지 이다. 
	*/

}
