// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Student.h"
#include "JsonObjectConverter.h"
#include "UObject/SavePackage.h"
/*패키지 이름 실제 데이터 지정*/
const FString UMyGameInstance::PackageName = TEXT("/Game/Stduent");//여기 텍스트에 우리가 만들 패키지의 이름을 지정하자.
const FString UMyGameInstance::Assetname = TEXT("TopStudent");

/*패키지 이름 실제 데이터 지정*/

/*이부분은 29분쯤에 나오는 언리얼 오브젝트 출력 함수 만드는 파트임 */
void PrintStudentInfo(const UStudent* InStudent, const FString& InTag)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] 이름 %s 순번 %d"), *InTag, *InStudent->GetName(), InStudent->GetOrder());
}


/**/


UMyGameInstance::UMyGameInstance()
{
	/*
	생성자에서 에셋을 로딩하는 경우 이 에셋은 게임이 시작하기 전에 미리 다 메모리에 올라와 있어야 함을 의미한다.
	*/
	const FString TopSoftObjectPath = FString::Printf(TEXT("%s.%s"), *PackageName, *Assetname);
	//오브젝트 경로를 가져온다.
	static ConstructorHelpers::FObjectFinder<UStudent> UASSET_TopStudent(*TopSoftObjectPath);
	//생성자에서 에셋을 로딩할땐 ConstructorHelpers라는 독특한 함수를 사용함
	if (UASSET_TopStudent.Succeeded()) //ConstructorHelpers에 의해 로딩이 됬는지는 Succeeded() 함수를 통해 알 수 있다.
	{
		PrintStudentInfo(UASSET_TopStudent.Object, TEXT("Construct"));
		//UASSET_TopStudent를 바로 적용은 못하고 Object라는 멤버변수를 통해 사용 가능
	}
}

void UMyGameInstance::Init()
{
	Super::Init();

	FStudentData RawDataSrc(16, TEXT("이득우"));

	const FString SavedDir = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	/*
	FPlatformMisc이라는 플랫폼 관련 함수에서 프로젝트 드렉터리를 가져오는 ProjectDir()를 유용하게 사용할 수 있다.
	위 뜻은 Saved 폴더의 경로를 가져온다.
	내컴퓨터 기준으로 "E:\HighProject\UnrealSerialization\Saved" << 여기가 절대 경로로 잡힌다.
	*/
	UE_LOG(LogTemp, Log, TEXT("저장할 파일 폴더 : %s"), *SavedDir);
	{

		const FString RawDataFileName(TEXT("RawData.bin"));
		FString RawDataAbsolutePath = FPaths::Combine(*SavedDir, *RawDataFileName);
		UE_LOG(LogTemp, Log, TEXT("저장할 파일 전체 경로 : %s"), *RawDataAbsolutePath);
		//여기서 로그 찍으면 알겠지만 폴더가 굉장히 이상하게 나온다.
		/*
		 이상한걸 올바르게 하는 함수가 있는다. MakeStandardFileName()
		*/
		FPaths::MakeStandardFilename(RawDataAbsolutePath);
		UE_LOG(LogTemp, Log, TEXT("저장할 파일 전체 경로 : %s"), *RawDataAbsolutePath);
		
		FArchive* RawFileWriterAr = IFileManager::Get().CreateFileWriter(*RawDataAbsolutePath);
		//모든 아카이브 클래스는 FArchive를 상속 받는다.
		/*
		FileManager라는 인터페이스에서 CreateFileWriter라는 함수로 
		RawDataAbsolutePat에 있는 RawData.bin파일을 쓸수 있는 
		아카이브 파일을 생성할 수 있게 된다.
		*/
		if(nullptr != RawFileWriterAr)
		{
			*RawFileWriterAr << RawDataSrc;
			RawFileWriterAr->Close();
			delete RawFileWriterAr;
			RawFileWriterAr = nullptr;
		}

		FStudentData RawDataDest;
		FArchive* RawFileReaderAr = IFileManager::Get().CreateFileReader(*RawDataAbsolutePath);
		if (nullptr != RawFileReaderAr)
		{
			
			//우리가 불러오든 저장하든간에 쉬프트 연산을 사용하면 된다.
			*RawFileReaderAr << RawDataDest; // 이건 리더 아카이브 때문에 불러 들이는 거임. (데이터 집어 넣는게 아님)
			RawFileReaderAr->Close();
			delete RawFileReaderAr;
			RawFileReaderAr = nullptr;

			UE_LOG(LogTemp, Log, TEXT("[RawData] 이름 %s 순번 %d"), *RawDataDest.Name, RawDataDest.Order);
		}

	}

	/*멤버변수에 언리얼 오브젝트 생성*/
	StudentSrc = NewObject<UStudent>();
	StudentSrc->SetName(TEXT("이득우"));
	StudentSrc->SetOrder(59);
	
	/*언리얼 오브젝트를 저장하고 불러들이는 코드*/
	{
		//파일 이름 지정
		const FString ObjectDataFileName(TEXT("ObjectData.bin"));
		FString ObjectDataAbsolutePath = FPaths::Combine(*SavedDir, *ObjectDataFileName);
		FPaths::MakeStandardFilename(ObjectDataAbsolutePath);
		//이후 언리얼 오브젝트를 저장할때 우선 메모리에 오브젝트 내용을 저장해보자

		TArray<uint8> BufferArray;
		FMemoryWriter MemoryWriterAr(BufferArray);
		StudentSrc->Serialize(MemoryWriterAr);
		// TArray<uint8>은 직렬화를위한 일종의 버퍼다

		/*
		여기서부터 다른 방식으로 
		파일을 읽고 저장할때 번거로운 delete와 null값을 사용하는데
		이를 한번에 처리 가능한 스마트포인터를 사용해보자
		*/

		if(TUniquePtr<FArchive> FileWriterAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*ObjectDataAbsolutePath)))
		{
			//만약에 성공하게 될경우 유니크 포인터를선언한 로직이 벗어나면 알아서 해지가 된다.
			//따라서 close 하는것만 신경써주자.

			*FileWriterAr << BufferArray;
			FileWriterAr->Close();
		}

		/*여기서부턴 파일로부터 읽어올것이다.*/
		TArray<uint8> BufferArrayFromFile;
		if (TUniquePtr<FArchive> FileReaderAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*ObjectDataAbsolutePath)))
		{
			*FileReaderAr << BufferArrayFromFile;
			FileReaderAr->Close();
		}
		/*여기서부터 다시 메모리에다가 전송할것이다.*/
		FMemoryReader MemoryReaderAr(BufferArrayFromFile);
		UStudent* StudentDest = NewObject<UStudent>();
		StudentDest->Serialize(MemoryReaderAr);
		PrintStudentInfo(StudentDest, TEXT("ObjectData"));
	}

	/****Json을 읽고 쓰는 로직****/
	{
		const FString JsonDataFileName(TEXT("StudentData.txt"));
		FString JsonDataAbsolutePath = FPaths::Combine(*SavedDir, *JsonDataFileName);
		FPaths::MakeStandardFilename(JsonDataAbsolutePath);
		//Json의 경우에 FJson오브젝트라고 하는 특별한 객체를 우리가 생성해 줘야 한다.
		//언리얼 오브젝트를 Json오브젝트를 바꿔줘야 하는데 헤더 추가를 해주자
		//#include "JsonObjectConverter.h" 변환을 위한 헬퍼 라이브러리들이 들어가 있다.

		TSharedRef<FJsonObject> JsonObjectSrc = MakeShared<FJsonObject>();
		//생성자를 통해 만든 객체를 쉐어드 레퍼런스에 지정해주면 JsonOnjectSrc은 Null이 아님을 보장 받는다.

		FJsonObjectConverter::UStructToJsonObject(StudentSrc->GetClass(), StudentSrc, JsonObjectSrc);
		/*
		프로퍼티들을 Json을 변환할것이기에 UStruct정보를 넘긴다라고 이해하자.
		그리고 FJsonObjectConverter::UStructToJsonObject(StudentSrc->GetClass(), StudentSrc, JsonObjectSrc);
		까지만 하고 빌드하면 심볼이 없다 에러가 뜰텐데 이것은 우리가 사용하는 Json 라이브러리들을
		연동하여 구현부를 제공해줘야 한다.
		UnrealSerialization.Build<< 여기서 하는거임
		위 소스파일에서 "Json", "JsonUtilities" 모듈을 추가해줘야함.
		*/

		FString JsonOutString;
		TSharedRef<TJsonWriter<TCHAR>> JsonWriterAr = TJsonWriterFactory<TCHAR>::Create(&JsonOutString);
		//JsonWriterFactory에 의해서 Json으로 써주는 아카이브가 만들어진다.

		if (FJsonSerializer::Serialize(JsonObjectSrc, JsonWriterAr))
		{
			FFileHelper::SaveStringToFile(JsonOutString, * JsonDataAbsolutePath);
			//SaveStringToFile은 인코딩을 신경쓰지 않아도 운영체제에 맞게 알아서 잘 저장해준다.

		}

		FString JsonInString;
		FFileHelper::LoadFileToString(JsonInString, *JsonDataAbsolutePath);


		/*불러들인 스트링을 활용하여 Reader 아카이브를 만들어본다. */
		TSharedRef<TJsonReader<TCHAR>> JsonReaderAr = TJsonReaderFactory<TCHAR>::Create(JsonInString);
		//Create() 안에 Reader값을 인풋으로 넣는다.

		/*Reader로부터 실제로 변환할 Json 오브젝트를 만드는데 안만들어질수도 있기 때문에 Ptr 포인터로 선언한다. */
		TSharedPtr<FJsonObject> JsonObjectDest;
		if (FJsonSerializer::Deserialize(JsonReaderAr, JsonObjectDest))
		{
			UStudent* JsonStudentDest = NewObject<UStudent>();
			if (FJsonObjectConverter::JsonObjectToUStruct(JsonObjectDest.ToSharedRef(), JsonStudentDest->GetClass(), JsonStudentDest))
			{
				PrintStudentInfo(JsonStudentDest, TEXT("JsonData"));
			}
		}
	}

	SaveStudentPackage();
	//LoadStudentPackage();
	//LoadStudentObject();

	/*1-14 후반 에셋비동기 로딩 구현*/
	const FString TopSoftObjectPath = FString::Printf(TEXT("%s.%s"), *PackageName, *Assetname);
	Handle = StreamableManager.RequestAsyncLoad(TopSoftObjectPath,
		[&]()
		{
			if (Handle.IsValid() && Handle->HasLoadCompleted())
			{
				UStudent* TopStudent = Cast<UStudent>(Handle->GetLoadedAsset());
				if (TopStudent)
				{
					PrintStudentInfo(TopStudent, TEXT("AsyncLoad"));
					Handle->ReleaseHandle();
					Handle.Reset();
				}
			}
		}
		
	);

}

void UMyGameInstance::SaveStudentPackage() const
{
	/*1-14강에서 패키지 로드시 추가할 내용*/
	UPackage* StudentPackage = ::LoadPackage(nullptr, *PackageName, LOAD_None);
	//만약 패키지 저장 할때 이미 패키지가 있다면 이것을 다 로딩하고 저장해주는게 좋다. 따라서 로드 패키지를 먼저 해준다.

	if (StudentPackage)//패키지가 있는경우
	{
		StudentPackage -> FullyLoad();
	}


	/*1-14강에서 패키지 로드시 추가할 내용*/



	StudentPackage = CreatePackage(*PackageName);
	/*패키지 저장 옵션 지정해보기*/
	EObjectFlags ObjectFlag = RF_Public | RF_Standalone;
	//패키지를 저장하는 EObjectFlags라고 하는 enum값을 미리 지정한다. 
	//보통  F_Public | RF_Standalone 이라는 값을 사용하는게 일반적임

	UStudent* TopStudent = NewObject<UStudent>(StudentPackage, UStudent::StaticClass(), *Assetname,ObjectFlag);
	//StudentPackage 안에다가 집어 넣어야 하기 때문에 첫번째 인자로 지정
	//UStudent에 대해서 이름을 지정해주기 위해 두번째 인자를 클래스 인자로 강제로 채워준다.
	//오브젝트를 생성할대 고유한 이름을 넣어줄수 있는데 앞서 지정한 에셋네임을 세번째 인자로 넣는다.
	//네번째 인자에는 저장 플래그를 넣는다.

	TopStudent->SetName(TEXT("이득우"));
	TopStudent->SetOrder(36);

	/*TopStudent에 대한 서브 오브젝트 10개 생성*/
	const int32 Numofsubs = 10;
	for (int32 ix = 1; ix <= Numofsubs; ++ix)
	{
		FString SubobjectName = FString::Printf(TEXT("Student%d"), ix);
		//Student1,2,3,4,5~~~ 라는 이름이 만들어짐

		UStudent* SubStudent = NewObject<UStudent>(TopStudent, UStudent::StaticClass(), *SubobjectName, ObjectFlag);
		//첫번째 인자로는 패키지 바로 아래 패키지의 애셋인 TopStudent아래 들어가야 하기 때문에 TopStudnet 지정
		//이름을 지정해주기 위해서 세번째 인자로 들어가기 때문에 두번재 인자를 넣어준다.
		//세번째 인자에는 printf 생성한 SubObjectName을 지정한다.
		//마지막으로 플래그를 설정
		SubStudent->SetName(FString::Printf(TEXT("학생%d"), ix));
		SubStudent->SetOrder(ix);
	}	
	/*패키지 저장*/
	//저장을 위해서 패키지가 저장된 경로를 지정한다.
	//패키지에 대한 확장자를 부여해줘야 한다.
	//이것을 위해서 패키지에 관련된 PackageFileName을 생성할수 있는 함수를 사용한다.
	const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	//GetAssetPackageExtension() 함수는 언리얼 엔진에서 지정한 uasset이라는 확장자를 의미한다. 
	//따라서 /Game이라고 하면은 해당 프로젝트 정보를 바탕으로  현재 프로젝트의 content 폴더를 지정하게 되고 패키지의 Student가 파일 이름이 된다. 
	//그리고 Uasset이라는 확장자가 붙어 최종 경로가 된다.

	FSavePackageArgs SaveArgs;
	//위에 에러 뜰텐데 SavePackage 헤더를 추가해줘야 한다.
	SaveArgs.TopLevelFlags = ObjectFlag;

	if (UPackage::SavePackage(StudentPackage, nullptr, *PackageFileName, SaveArgs))
	{
		UE_LOG(LogTemp, Log, TEXT("패키지가 성공적으로 저장되었습니다."));

	}

	/*패키지 저장*/
}

/*패키지 정보 로딩하기*/
void UMyGameInstance::LoadStudentPackage() const
{
	UPackage* StudentPackage = ::LoadPackage(nullptr, *PackageName, LOAD_None);
	if (nullptr == StudentPackage) //패키지를 찾지 못한 경우
	{
		UE_LOG(LogTemp, Warning, TEXT("패키지를 찾을 수 없습니다."));
		return;
	}
	
	StudentPackage->FullyLoad();
	//패키지가 로딩이 되면 로딩된 패키지 안에서 원하는 에셋을 찾을 수가 있다.

	UStudent* TopStudent = FindObject<UStudent>(StudentPackage, *Assetname);
	//FindObject()라고 하는 API를 사용하여 우리가 찾을 패키지를 지정해준다.
	//에셋이름, 앞서서 우리가 설계했던 에셋이름 TopStudent를 넣어주면 TopStudent이름을 가지고 UStudent타입의오브젝트를 찾아 반환을 해준다.
	
	PrintStudentInfo(TopStudent, TEXT("FindObjectAsset"));
	

}

void UMyGameInstance::LoadStudentObject() const
{
	const FString TopSoftObjectPath = FString::Printf(TEXT("%s.%s"), *PackageName, *Assetname);
	// 패키지의 이름과 . 에셋이름을 같이 조합하여 문자열로 만들어준다.

	UStudent* TopStudent = LoadObject<UStudent>(nullptr, *TopSoftObjectPath);
	//패키지를 로딩하지 않기 때문에 첫번쨰로 nullptr을 넣어주고 그 다음에 경로 정보를 넣어주면 된다.

	PrintStudentInfo(TopStudent, TEXT("LoadObject"));
	//제대로 로딩이 되면 로그가 찍힘
}
