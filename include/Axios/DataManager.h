#pragma once
#include <nlohmann/json.hpp>

#include <fstream>
#include <unordered_map>

using KeyItem = std::array<int, 2>;

namespace ax
{	
	class DataManager
	{
	private:
		using callbackFunction = void(*)(const bool overwrite);
		using json = nlohmann::json;
		
		template <class T>
		class Data
		{
		private:
			callbackFunction m_extraCheck;
	
		public:
			std::unordered_map<std::string, T> data;
			
			// Call this function in the callback function (extraCheck)
			void check(const char* name, const T &value, const bool overwrite = false);
			
			// Used to check whole file
			// Called when the file is loaded to 
			// see if nothing is corrupted or missing
			void _check(const bool overwrite = false);
			
			// Import data from path
			void _load(const char* path);

			// Export data to path
			void _save(const char* path);

			// Override this to add custom configurations to check
			// Use the check function in the callback body
			// The callbackFunction should return void and take one bool as parameter
			// The bool is used when two configurations overlap, overwrite will
			// overwrite the configuration
			Data(const callbackFunction &extraCheck);
		};
		
	public:
		// This container contains all the conigurations of the game
		static Data<std::string>& Config(const callbackFunction &extraCheck = nullptr);
		
		// This container contains all the keybinding of the user
		static Data<KeyItem>& GameKey(const callbackFunction &extraCheck = nullptr);
	};

	template<class T>
	inline void DataManager::Data<T>::check(const char * name, const T &value, const bool overwrite)
	{
		if (overwrite || data.count(name) == 0)
			data[name] = value;
	}

	template<class T>
	inline void DataManager::Data<T>::_check(const bool overwrite)
	{
		m_extraCheck(overwrite);
	}

	template<class T>
	inline void DataManager::Data<T>::_load(const char * path)
	{
		std::ifstream configFile(path);
		json config;
		if (configFile.is_open())
		{
			std::stringstream stream;
			stream << configFile.rdbuf();
			config = json::parse(stream.str());
		}

		for (json::iterator it = config.begin(); it != config.end(); ++it)
			data[it.key()] = it.value().get<T>();
	}
	template<class T>
	inline void DataManager::Data<T>::_save(const char * path)
	{
		std::ofstream configFile(path, std::ios::trunc);
		json config(data);
		configFile << config.dump(2);
		configFile.close();
	}

	template<class T>
	inline DataManager::Data<T>::Data(const callbackFunction & extraCheck):
		m_extraCheck(extraCheck)
	{
	}
}