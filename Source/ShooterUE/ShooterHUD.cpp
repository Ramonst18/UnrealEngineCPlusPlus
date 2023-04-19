// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUD.h"

#include "Engine/Canvas.h"

AShooterHUD::AShooterHUD()
{
	//obtenemos la textura cross
	static ConstructorHelpers::FObjectFinder<UTexture2D> Crosshair (TEXT("/Game/HUD/Cross/Cross"));

	//Comprobamos si encontró la textura
	if (Crosshair.Object)
	{
		//pasamos el objeto a la variable
		CrossTexture = Crosshair.Object;
	}
}

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();

	//Verificamos si CrossTexture no es nulo
	if (CrossTexture)
	{
		//Ponemos la textura al centro de la ventana
		float ScreenXSize = (Canvas->SizeX/2) - 32;	//Obtenemos la mitdad del eje x de la pantalla
		float ScreenYSize = ((Canvas->SizeY/2)-50) - 32;	//Obtenemos la mitad del eje y de la pantalla

		//dibujamos la textura
		DrawTexture(CrossTexture, ScreenXSize, ScreenYSize, 64, 64, 0,0,1,1);
	}
}
