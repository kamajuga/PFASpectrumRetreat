#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "IRessource.h"


class RessourceManager
{
public:
	RessourceManager() = default;
	~RessourceManager() = default;
	
	template <typename T>
	T* create (std::string name);

	template <typename T>
	T* get(std::string name);

	void Delete(std::string name);

	void freeRessources(void);

private:
	std::unordered_map<std::string, IRessource*> m_ressources;
};

template<typename T>
inline T* RessourceManager::create(std::string name)
{
	T* newRessource = new T();
	m_ressources[name] = std::move(newRessource);

	return newRessource;
}

template<typename T>
inline T* RessourceManager::get(std::string name)
{
	auto toReturn = m_ressources.find(name);

	if (toReturn != m_ressources.end())
	{
		return dynamic_cast<T*>(toReturn->second);
	}
	return nullptr;
}

inline void RessourceManager::Delete(std::string name)
{
	auto toDelete = m_ressources.find(name);

	if (toDelete != m_ressources.end())
	{
		m_ressources.erase(toDelete);
	}

}


class TestResource : public IRessource {
public:
	TestResource(const std::string& filename) {
		std::cout << "Loading resource: " << filename << "\n";
	}
};

