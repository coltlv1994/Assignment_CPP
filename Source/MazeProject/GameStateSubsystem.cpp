// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateSubsystem.h"
#include "SnakeWorld.h"
#include "enum.h"

void UGameStateSubsystem::ChangeGameState(SnakeGameState p_gameState)
{
	if (m_gameState == p_gameState)
	{
		// Don't change
		return;
	}

	SnakeGameState oldGameState = m_gameState;

	m_gameState = p_gameState;

	GameStateChanged.Broadcast(oldGameState, p_gameState);
}

void UGameStateSubsystem::RotatePlayerNumber()
{
	m_noOfPlayers = (m_noOfPlayers + 1) % 4;
}

int UGameStateSubsystem::GetPlayerNumber()
{
	return (m_noOfPlayers % 4 + 1);
}

void UGameStateSubsystem::SetPVPGame()
{
	m_playMode = SnakePlayMode::PvP;
}

void UGameStateSubsystem::SetPVEGame()
{
	m_playMode = SnakePlayMode::PvE;
}

SnakePlayMode UGameStateSubsystem::GetGameplayMode()
{
	return m_playMode;
}

void UGameStateSubsystem::SetGridMap(GridMap* p_gridMap)
{
	m_gridMapPointer = p_gridMap;
}

GridMap* UGameStateSubsystem::GetGridMap(void)
{
	return m_gridMapPointer;
}

void UGameStateSubsystem::GoToNextLevel()
{
	nextLevel += 1;
}