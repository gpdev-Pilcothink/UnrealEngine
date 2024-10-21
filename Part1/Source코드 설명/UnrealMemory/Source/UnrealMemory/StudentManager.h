// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREALMEMORY_API FStudentManager : public FGCObject //일반 C++오브젝트는 F라는 접두사를 붙여주는게 좋다.
{
public:
	FStudentManager(class UStudent* InStudent) : SafeStudent(InStudent) {}
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override
	{
		return TEXT("FStudentManager");
	}

	const class UStudent* GetStudent() const { return SafeStudent; }

private:
	class UStudent* SafeStudent = nullptr;

};
