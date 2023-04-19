// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERUE_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

	//Variables a utilizar
	//Creamos una textura
	UTexture2D* CrossTexture;

public:
	//Constructor
	AShooterHUD();

	//Funcion para dibujar el HUD
	virtual void DrawHUD() override;
};
