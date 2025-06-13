#include "RessourceManager.h"

void RessourceManager::freeRessources(void)
{
	for (auto& pair : m_ressources)
	{
		if (pair.second)
		{
			delete pair.second;
			pair.second = nullptr;
		}
	}
	m_ressources.clear();
}
