#include "pch.h"
#include "Game.h"
#include "memoryFunctions.h"

const char** aswEngine = nullptr;

Game game;

bool Game::onDllMain() {
	bool error = false;

	char aswEngineSig[] = "\x85\xC0\x78\x74\x83\xF8\x01";
	aswEngine = (const char**)sigscanOffset("GuiltyGearXrd.exe",
		aswEngineSig,
		_countof(aswEngineSig),
		{-4, 0},
		&error, "aswEngine");

	char gameDataPtrSig[] = "\x33\xC0\x38\x41\x44\x0F\x95\xC0\xC3\xCC";
	gameDataPtr = (const char**)sigscanOffset(
		"GuiltyGearXrd.exe",
		gameDataPtrSig,
		_countof(gameDataPtrSig),
		{-4, 0},
		NULL, "gameDataPtr");

	return !error;
}

GameMode Game::getGameMode() const {
	if (!gameDataPtr || !(*gameDataPtr)) return GAME_MODE_TRAINING;
	return (GameMode)*(*gameDataPtr + 0x45);
}

bool Game::isGameModeOnline() const {
	GameMode gameMode = getGameMode();
	return !(gameMode == GAME_MODE_ARCADE
		|| gameMode == GAME_MODE_CHALLENGE
		|| gameMode == GAME_MODE_REPLAY
		|| gameMode == GAME_MODE_STORY
		|| gameMode == GAME_MODE_TRAINING
		|| gameMode == GAME_MODE_TUTORIAL
		|| gameMode == GAME_MODE_VERSUS);
}
