#include "MyGameInstance.h"
#include "Staff.h"
#include "Student.h"
#include "Teacher.h"
//여기 헤더선언 부분도 주의해야할것이  언리얼 오브젝트에 선언된 MynameInstance가 가자 위에 있어야 한다.

UMyGameInstance::UMyGameInstance()
{
	SchoolName = TEXT("기본학교");
	//이 기본값은 CDO라고 하는 탬플릿 객체에 저장이 되어있음

}

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Log, TEXT("=================="));
	TArray<UPerson*> Persons = { NewObject<UStudent>(), NewObject<UTeacher>(), NewObject<UStaff>()};

	//언리얼 엔진이 제공하는 라이브러리로 TArray가 있는데 이후 강좌에서 소개할 예정임.
	//동적 가변배열에 집어넣은 변수들은 'ranged for'문을 통해서 이터레이트(순회) 할 수 있다.
	for (const auto Person : Persons) //포인터 경우엔 쓰기 불편하니 auto로 퉁침. , persons에 있는 모든 요소를 순회한다는 의미임.
	{
		UE_LOG(LogTemp, Log, TEXT("구성원 이름 : %s"), *Person->GetName());
	}
	UE_LOG(LogTemp, Log, TEXT("=================="));

	for (const auto Person : Persons)
	{
		ILessonInterface* LessonInterface = Cast<ILessonInterface>(Person);
		if (LessonInterface)
		{
			UE_LOG(LogTemp, Log, TEXT("%s님은 수업에 참여할 수 있습니다."), * Person->GetName());
			LessonInterface->DoLesson();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s님은 수업에 참여할 수 없습니다.."), *Person->GetName());
		}
	}
	UE_LOG(LogTemp, Log, TEXT("=================="));
}
	
