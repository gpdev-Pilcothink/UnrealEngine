// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TFItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/TFCollision.h"
#include "Interface/TFCharacterItemInterface.h"


// Sets default values
ATFItemBox::ATFItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggetBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	//CreateDefaultSubobject를 실행하여 위 세가지 포인터에 대한 객체 생성


	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATFItemBox::OnOverlapBegin);
	//트리거의 경우 우리가 앞서 설정했던 트리거영 전용 콜리전 프로필 사용
	//OnComponentBeginOverlap은 블루프린트에서도 사용가능하도록 다이나믹 델리게이트로 설정이 되어 있다.
	//따라서 우리가 연결하는 함수의 경우에는 UFUNCTION으로 지정을 해줘야 한다.

	/*Mesh Component 설정*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Cave_Ruins/Meshes/SM_Dungeon_chest.SM_Dungeon_chest'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision")); //상자는 보여주기만 하고 충돌이 일어나지 않도록 NoCollision 프로필 지정

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/P_Destruction_Building_A.P_Destruction_Building_A'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		//SetTemplate을 통해  EffectRef.Object 지정
		Effect->bAutoActivate = false;
		//처음에 바로 발동되지 않도록 bAutoActivate=false로 하여 비활성화
	}

}

void ATFItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (nullptr == Item)
	{
		Destroy();
		return;
	}

	ITFCharacterItemInterface* OverlappingPawn = Cast< ITFCharacterItemInterface>(OtherActor); //여기서 OtherActor가 캐릭터인데  ITFCharacterItemInterface를 상속받았음.
	//부딪친 액터에 대해서 형변환 진행
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}

	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &ATFItemBox::OnEffectFinished);

	/*아이템 습득
	* 우리가 아이템을 습득 했을 때 캐릭터로부터 아이템을 습득했다고 전달할 수 있는 인터페이스를 하나 추가하자.
	* 아이템은 미들웨어고 캐릭터는 게임 레이어이기 때문에 우리가 인터페이스를 통해서 간접적으로 호출해줘야 한다.
	*/
}

void ATFItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}


