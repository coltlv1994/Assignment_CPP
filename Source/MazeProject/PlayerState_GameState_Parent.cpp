// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_GameState_Parent.h"

SnakePlayMode APlayerState_GameState_Parent::GetSnakePlayMode()
{
	return m_snakePlayMode;
}

void  APlayerState_GameState_Parent::SetSnakePlayMode(SnakePlayMode p_snakePlayMode)
{
	m_snakePlayMode = p_snakePlayMode;
}