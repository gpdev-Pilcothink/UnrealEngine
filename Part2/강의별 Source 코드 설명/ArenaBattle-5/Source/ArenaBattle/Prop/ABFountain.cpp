// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABFountain.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AABFountain::AABFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	//이때는 전방선언을 더 이상 사용할 수가 없고 확정을 지어야 한다.
	//전방선언이 되지 않는 이유는 CreateDefaultSubobject의 경우 UStaticMeshComponent의 내부 구현을 알아야 하기 때문이다.
	//UStaticMeshComponent는 결국 클래스의 전체 정의가 필요한데 전체 정의가 "Components/StaticMeshComponent.h"에 존재한다. 따라서 추가해야만 한다.
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));


	RootComponent = Body;
	Water->SetupAttachment(Body);
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));
	//이렇게 구조가 만들어지면 컴포넌트 안에다가 스태틱 메쉬들을 직접 지정해주자 이떄 ConstructorHelpers가 사용된다.
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodhMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));
	if (BodhMeshRef.Object)
	{
		Body->SetStaticMesh(BodhMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));

	if (WaterMeshRef.Object)
	{
		Water->SetStaticMesh(WaterMeshRef.Object);
	}
}

// Called when the game starts or when spawned
void AABFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

