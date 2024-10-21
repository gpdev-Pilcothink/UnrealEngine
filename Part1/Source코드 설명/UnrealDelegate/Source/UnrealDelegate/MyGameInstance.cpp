#include "MyGameInstance.h"
#include "Staff.h"
#include "Student.h"
#include "Teacher.h"
#include "Card.h"
#include "CourseInfo.h"
//여기 헤더선언 부분도 주의해야할것이  언리얼 오브젝트에 선언된 MynameInstance가 가자 위에 있어야 한다.

UMyGameInstance::UMyGameInstance()
{
	SchoolName = TEXT("학교");
	//이 기본값은 CDO라고 하는 탬플릿 객체에 저장이 되어있음

}

void UMyGameInstance::Init()
{
	Super::Init();

	CourseInfo = NewObject<UCourseInfo>(this);


	UE_LOG(LogTemp, Log, TEXT("=================="));
	
	UStudent* Student1 = NewObject<UStudent>(); //이런 구문은 실행이 되면 자동으로 소멸되기 때문에 outer 설정이 불필요하다.
	Student1->SetName(TEXT("학생1"));
	UStudent* Student2 = NewObject<UStudent>();
	Student2->SetName(TEXT("학생2"));
	UStudent* Student3 = NewObject<UStudent>();
	Student3->SetName(TEXT("학생3"));

	CourseInfo->OnChanged.AddUObject(Student1, &UStudent::GetNotification);
	CourseInfo->OnChanged.AddUObject(Student2, &UStudent::GetNotification);
	CourseInfo->OnChanged.AddUObject(Student3, &UStudent::GetNotification);

	CourseInfo->ChangeCourseInfo(SchoolName, TEXT("변경된 학사 정보"));
	
	UE_LOG(LogTemp, Log, TEXT("=================="));
}
	
