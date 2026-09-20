#include "GameManager.h"

GameManager::GameManager() :
	inDialog(false),
	dialogController(std::make_unique<DialogController>())
{
}

GameManager& GameManager::only()
{
	static GameManager instance;
	return instance;
}

DialogController& GameManager::getDialogController() const
{
	return *dialogController;
}

bool GameManager::isInDialog() const
{
	return inDialog;
}

void GameManager::openDialog(const std::string& dialogText)
{
	inDialog = true;
	dialogController->startDialog(dialogText);
}

void GameManager::closeDialog()
{
	inDialog = false;
}
