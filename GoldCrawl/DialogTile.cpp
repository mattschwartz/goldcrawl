#include "DialogTile.h"
#include "GameManager.h"

void DialogTile::interact(Player&)
{
	GameManager::only().openDialog(dialogText);
}
