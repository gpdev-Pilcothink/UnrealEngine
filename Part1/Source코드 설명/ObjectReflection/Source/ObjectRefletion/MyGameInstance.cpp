#include "MyGameInstance.h"
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
	UE_LOG(LogTemp, Log, TEXT("===================="));

	SchoolName = TEXT("청강문화산업대학교");
	//기본 객체와 무관하게 생성된 MyGameInstance에는 "청강문화산업대학교"라는 학교이름이 설정 됨.

	UE_LOG(LogTemp, Log, TEXT("학교 이름 : %s"), *SchoolName);
	UE_LOG(LogTemp, Log, TEXT("학교 이름 기본값 : %s"), *GetClass()->GetDefaultObject<UMyGameInstance>()->SchoolName);
	/*
	이상태로 컴파일 하면 글자가 안나올수도 있는데
	CDO는 이 에디터가 활성화 되기 이전에 초기화 되는 순서를 가지고 있기 때문에
	에디터에서 인지를 못하는 경우가 종종있다.
	따라서 CDO를 고쳐주는 (기본값을 고쳐주는) 생성자 코드를 고치는 경우에는
	에디터를 꺼줘야 한다.!
	*/
	UE_LOG(LogTemp, Log, TEXT("===================="));


	//이부분부터는 student와 teacher를 선언하고 getter와 setter 방식을 다룰것임.
	UStudent* Student = NewObject<UStudent>();
	UTeacher* Teacher = NewObject<UTeacher>();



	//방식1 (getter와 setter)
	Student->SetName(TEXT("학생1"));
	UE_LOG(LogTemp, Log, TEXT("새로운 학생 이름 %s"), *Student->GetName());

	//방식2 (리플렉션 사용하기)
	FString CurrentTecherName; //이름 속성을 가져오기 위한 지정
	FString NewTeacherName(TEXT("이득우"));
	FProperty* NameProp = UTeacher::StaticClass()->FindPropertyByName(TEXT("Name"));
	/*
	* UTeacher의 static클래스 정보를 얻어오고 FindPropertyByName이라고 클래스에서 제공하는 함수 사용
	* FindPropertyByName은 속성 이름을 검색하여 프로퍼티에 대한 포인터를 가져오는 방식임
	* 실제로 있게 된다면 NULL이 아니게 됨.
	*/
	if (NameProp)
	{
		NameProp->GetValue_InContainer(Teacher, &CurrentTecherName);
		/*
		* MameProp은 결국 클래스에 대한 속성이다.
		* 이 속성에 대해서 우리가 지정한 인스턴스의 값을 빼올수가 있다.
		* 속성에서 인스턴스 빼오는 함수==>GetValue_InContainer();
		* Teacher가 가지는 객체의 속성중에서 이름 속성값을 가져와야하는데
		* 우리가 지정을 해줘야 한다. ==>FString CurrentTecherName 선언후 인자에 포인터로 넣기.
		*/
		UE_LOG(LogTemp, Log, TEXT("현재 선생님 이름 %s"), *CurrentTecherName);

		//이부분부터는 선생님 이름을 바꿔볼거임.

		NameProp->SetValue_InContainer(Teacher, &NewTeacherName);
		UE_LOG(LogTemp, Log, TEXT("새로운 선생님 이름 %s"), *Teacher->GetName());
	}

	//방식3(리플렉션 사용하여 함수 출력하기)
	UE_LOG(LogTemp, Log, TEXT("===================="));

	Student->DoLesson();
	//이거는 일반적인 방식
	
	//이 밑에서부터 리플렉션을 이용하여 선생님의 함수를 불러볼거임.
	UFunction* DoLessonFunc = Teacher->GetClass()->FindFunctionByName(TEXT("DoLesson"));
	if (DoLessonFunc)
	{
		Teacher->ProcessEvent(DoLessonFunc, nullptr);
		//인스턴스를 지정하여 ProcessEvent라는 함수를 사용하여 함수 포인터를 넘겨줌으로써 실행 가능하다.
	}

	UE_LOG(LogTemp, Log, TEXT("===================="));
}
	
