// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//la clase .h header, es la cabecera del proyecto, aqui agregamos las variables globales
//librerias a utilizar
//Funciones basicas de unreal
#include "CoreMinimal.h"
//Librerias del GameModeBase
#include "GameFramework/GameModeBase.h"
#include "ShooterUEGameModeBase.generated.h"

/**
 * Esto nos indica que es una clase de unreal, UCLASS()
 */
UCLASS()		
class SHOOTERUE_API AShooterUEGameModeBase : public AGameModeBase
{
	//NOTA: : public <nombre> nos dice que esta heredando de una clase en este caso
	//hereda de AGameModeBase

	//macro que genera el cuerpo de la clase
	GENERATED_BODY()

public:
	//Constructor de la cabecera
	AShooterUEGameModeBase();
};
