#include "ShopController.h"
#include <format>
#include "Color.h"
#include "GameManager.h"

void ShopController::openShop(const std::string& prompt, PlayerUpgrade upgrade, int cost)
{
	this->prompt = prompt;
	this->upgrade = upgrade;
	this->cost = cost;

	notEnoughMoney = false;
	selection = 0;
}

void ShopController::handleInput(const Input& input)
{
	if (input.isBindingPressed(KeyBinding::Left))
	{
		selection = 0;
		notEnoughMoney = false;
	}
	else if (input.isBindingPressed(KeyBinding::Right))
	{
		selection = 1;
		notEnoughMoney = false;
	}

	if (input.isBindingPressed(KeyBinding::A))
	{
		notEnoughMoney = false;
		auto& gm = GameManager::only();
		if (selection == 1)
		{
			// player can't afford
			if (gm.getGold() < cost)
			{
				notEnoughMoney = true;
				return;
			}
			// unlock upgrade
			gm.addGold(-cost);
			gm.unlockUpgrade(upgrade);
		}
		gm.closeShop();
	}
}

void ShopController::update(Uint64 delta)
{
}

void ShopController::render(const Renderer& renderer) const
{
	if (selection == 0)
	{
		static auto noSprite = Sprite{ "Sprites/shop_background_no.png", SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT } };
		renderer.drawSprite(noSprite, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	}
	else if (selection == 1)
	{
		static auto noSprite = Sprite{ "Sprites/shop_background_yes.png", SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT } };
		renderer.drawSprite(noSprite, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	}

	if (notEnoughMoney)
	{
		renderer.drawText("You don't have enough", 2, 17, colors::highlight);
		renderer.drawText("money!", 2, 35, colors::highlight);
	}
	else
	{
		renderer.drawText(std::format("Buy upgrade for {}", cost), 2, 17, colors::highlight);
		renderer.drawText("gold?", 2, 35, colors::highlight);
	}

	switch (upgrade)
	{
	case PlayerUpgrade::DoubleRadius:
		renderer.drawText("[Double clean radius]", 5, 61, colors::base);
		break;
	case PlayerUpgrade::DoubleCleanliness:
		renderer.drawText("[Double clean amount]", 5, 61, colors::base);
		break;
	case PlayerUpgrade::DoubleCleanSpeed:
		renderer.drawText("[Faster clean speed]", 5, 61, colors::base);
		break;
	case PlayerUpgrade::DoubleMoveSpeed:
		renderer.drawText("[Faster move speed]", 5, 61, colors::base);
		break;
	}
}
