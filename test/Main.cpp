#include "Axios/Instance.h"
#include "Axios/DataManager.h"
#include <SFML/Window/Keyboard.hpp>

int main()
{
	ax::DataManager::GameKey([](bool force){
		ax::DataManager::GameKey().check("jump", {sf::Keyboard::Space, -1 }, force);
		ax::DataManager::GameKey().check("left", {sf::Keyboard::A, -1 }, force);
		ax::DataManager::GameKey().check("right",{sf::Keyboard::D, -1 }, force);
	});

	ax::Instance::getInstance().start();

	return 0;
}