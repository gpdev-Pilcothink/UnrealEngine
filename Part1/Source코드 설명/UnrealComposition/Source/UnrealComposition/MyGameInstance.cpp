#include "MyGameInstance.h"
#include "Staff.h"
#include "Student.h"
#include "Teacher.h"
#include "Card.h"
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
	for (const auto Person : Persons)
	{
		const UCard* OwnCard =Person->GetCard();
		check(OwnCard);
		ECardType CardType = OwnCard->GetCardType();
		//UE_LOG(LogTemp, Log, TEXT("%s님이 소유한 카드 종류 %d"), *Person->GetName(), CardType)


		const UEnum* CardEnumType= FindObject<UEnum>(nullptr, TEXT("/Script/UnrealComposition.ECardType"));
		//열거형 타입을 가져오는데 첫번째 인자는 null값 두번째 인자로는 TEXT를 넣어준다
		//TEXT에 들어가는 절대 주소값을 사용하여 우리가 원하는 타입 정보를 얻어올 수 있다.
		//c++에 생성된 언리얼 객체들은 Script라고 하는 절대 주소를 가지는데 우리는 언리얼 컴포지션이라고 하는 모듈이름 을 써준다.
		//이때 프로젝트 이름이 모듈 이름이 된다.
		if (CardEnumType)
		{
			FString CardMetaData = CardEnumType->GetDisplayNameTextByValue((int64)CardType).ToString();
			//GetDisplayNameTextByValue는 int64만 받기 때문에 CardType을 형변환 해준다.
			//그다음에 다시 String으로 형변환해주는데  GetDisplayNameTextByValue는 FText로 반환되기 때문에 다국어 지원 문자열이라서 출력할땐 String으롤 변환해주어야 한다.
			UE_LOG(LogTemp, Log, TEXT("%s님이 소유한 카드 종류 %s"), *Person->GetName(), *CardMetaData)
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("=================="));
}
	
