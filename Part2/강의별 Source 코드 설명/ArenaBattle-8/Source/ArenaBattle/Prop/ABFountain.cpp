// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABFountain.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AABFountain::AABFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	//�̶��� ���漱���� �� �̻� ����� ���� ���� Ȯ���� ����� �Ѵ�.
	//���漱���� ���� �ʴ� ������ CreateDefaultSubobject�� ��� UStaticMeshComponent�� ���� ������ �˾ƾ� �ϱ� �����̴�.
	//UStaticMeshComponent�� �ᱹ Ŭ������ ��ü ���ǰ� �ʿ��ѵ� ��ü ���ǰ� "Components/StaticMeshComponent.h"�� �����Ѵ�. ���� �߰��ؾ߸� �Ѵ�.
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));


	RootComponent = Body;
	Water->SetupAttachment(Body);
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));
	//�̷��� ������ ��������� ������Ʈ �ȿ��ٰ� ����ƽ �޽����� ���� ���������� �̋� ConstructorHelpers�� ���ȴ�.
	
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

