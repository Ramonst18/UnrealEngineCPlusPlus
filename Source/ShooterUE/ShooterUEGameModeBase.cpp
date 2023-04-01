// Copyright Epic Games, Inc. All Rights Reserved.

//.cpp clase de c++, donde realizaremos las funciones del programa, estara toda la logica

//incluimos la cabecera
#include "ShooterUEGameModeBase.h"

#include "PlayerCharacter.h"

//Constructor del archivo
AShooterUEGameModeBase::AShooterUEGameModeBase()
{
	/*Notas
	 *no podemos pasar directamente la clase, tenemos que pasar su StaticClass
	*/
	//valores del selected GameMode

	//cambiamos el default pawn class por el nuestro
	//ConstructorHelpers es un elementos que nos va a ayudar con :: decimos que clase ocupamos
	//en este caso buscar una clase, ponemos entre parentesis un texto, este texto deberá de ser
	//la ruta donde está nuestro Pawn o blueprint
	static ConstructorHelpers::FClassFinder<APawn> PlayerCharacterBP (TEXT("/Game/Blueprints/PlayerCharacterBP"));

	//Comprobamos si encontró la clase del Pawn
	if (PlayerCharacterBP.Class != NULL)
	{
		//declaramos el DefautlPawnClass
		DefaultPawnClass = PlayerCharacterBP.Class;
	}
	
}
