// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Inspect.generated.h"

/**
 * 
 */
UCLASS()
class UW_Inspect : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FText ItemName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FText ItemDescription;
	
};
