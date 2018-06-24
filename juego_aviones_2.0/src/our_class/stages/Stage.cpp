#include "Stage.h"

Stage* Stage::instance = NULL;
Stage* Stage::current = NULL;

std::map<std::string, Stage*> Stage::stages;

Stage::Stage() {
	Stage::instance = this;
}

Stage::~Stage() { }

void Stage::render() {}
void Stage::update(float deltaTime) {}

void Stage::onChange(std::string stageName){

	std::map<std::string, Stage*>::iterator it = stages.find(stageName);
	if (it != stages.end()) {
		Stage::current = it->second;
	}
}
void Stage::addStage(std::string stageName, Stage* stage) {
	std::map<std::string, Stage*>::iterator it = Stage::stages.find(stageName);
	if (it == Stage::stages.end()) {
		Stage::stages.insert(std::pair<std::string, Stage*>(stageName, stage));
	}
}

void Stage::removeStage(std::string stageName) {
	std::map<std::string, Stage*>::iterator it = Stage::stages.find(stageName);
	if (it != Stage::stages.end()) {
		Stage::stages.erase(it);
	}
}