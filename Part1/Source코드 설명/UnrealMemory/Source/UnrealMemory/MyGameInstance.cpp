// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Student.h"
#include "StudentManager.h"

void CheckUObjectIsvalid(const UObject* InObject, const FString& InTag)
{
	if (InObject->IsValidLowLevel())
	{
		//IsValidLowLevel 함수는 null 포인터도 함께 체크가 되어있기 때문에 안심해서 사용 가능하다.
		UE_LOG(LogTemp, Log, TEXT("[%s] 유효한 언리얼 오브젝트"), *InTag);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] 유효하지 않은 언리얼 오브젝트"), *InTag);
	}
}

void CheckUObjectIsNull(const UObject* InObject, const FString& InTag)
{
	if (nullptr == InObject) //널 값인지 아닌지를 체크해주는 함수
	{
		//IsValidLowLevel 함수는 null 포인터도 함께 체크가 되어있기 때문에 안심해서 사용 가능하다.
		UE_LOG(LogTemp, Log, TEXT("[%s] 널 포인터 언리얼 오브젝트"), *InTag);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] 널 포인터가 아닌 언리얼 오브젝트"), *InTag);
	}
}

void UMyGameInstance::Init()
{
	Super::Init();
	NonPropStudent = NewObject<UStudent>();
	PropStudent = NewObject<UStudent>();

	//
	NonPropStudents.Add(NewObject<UStudent>());
	PropStudents.Add(NewObject<UStudent>());

	StudentManager = new FStudentManager(NewObject<UStudent>());

	/*여기서 Manager는 일반 c++ 객체이고, 안에 있는 UObject를 관리할 수 있는 능력이 전혀 없다.
	StudentManager 클래스는 UPROPERTY같은 것들을 전혀 쓸 수 가 없기 때문에 생성자로 UObject가 들어와도 지킬 방법이 없다.
	따라서 가비지 컬렉션이 발동되면 이 객체 안에 있는 언리얼 오브젝트는 회수가 된다.
	*/
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();

	const UStudent* StudentInManager = StudentManager->GetStudent();

	delete StudentManager;
	StudentManager = nullptr;
	
	//3
	CheckUObjectIsNull(StudentInManager, TEXT("StudentInManager"));
	CheckUObjectIsvalid(StudentInManager, TEXT("StudentInManager"));
	//3

	//1
	CheckUObjectIsNull(NonPropStudent,TEXT("NonPropStudent"));
	CheckUObjectIsvalid(NonPropStudent, TEXT("NonPropStudent"));

	CheckUObjectIsNull(PropStudent, TEXT("PropStudent"));
	CheckUObjectIsvalid(PropStudent, TEXT("PropStudent"));

	//2
	CheckUObjectIsNull(NonPropStudents[0], TEXT("NonPropStudents"));
	CheckUObjectIsvalid(NonPropStudents[0], TEXT("NonPropStudents"));

	CheckUObjectIsNull(PropStudents[0], TEXT("PropStudents"));
	CheckUObjectIsvalid(PropStudents[0], TEXT("PropStudents"));

}
