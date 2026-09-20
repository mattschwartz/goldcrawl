#pragma once

#include "Prelude.h"
#include "DialogController.h"

class GameManager
{
public:
	static GameManager& only();

	DialogController& getDialogController() const;

	bool isInDialog() const;
	void openDialog(const std::string& dialogText);
	void closeDialog();

private:
	bool inDialog;
	std::unique_ptr<DialogController> dialogController;

	GameManager();
};
