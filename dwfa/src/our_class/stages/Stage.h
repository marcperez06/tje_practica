#ifndef STAGE_H
#define STAGE_H
#pragma once

#include <map>
#include <string>

class Stage {
	public:

		static Stage* instance;
		static std::map<std::string, Stage*> stages;
		static Stage* current;

		Stage();
		~Stage();

		virtual void render();
		virtual void update(float deltaTime);

		static void onChange(std::string stageName);
		
		static void addStage(std::string stageName, Stage* stage);
		static void removeStage(std::string stageName);
};

#endif
