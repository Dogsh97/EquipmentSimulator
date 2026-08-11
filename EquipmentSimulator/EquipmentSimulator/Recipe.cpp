#include <iostream>
#include "Recipe.h"

Recipe::Recipe()
	: recipeId(-1),
	processTime(-1),
	temperature(-1),
	isSet(false)
{
}

void Recipe::SetRecipe(int id, float time, float temperature) {
	recipeId = id;
	processTime = time;
	this->temperature = temperature;
	isSet = true;
}

void Recipe::PrintInfo() const{
	std::cout << "RecipeId : " << recipeId << "\n";
	std::cout << "ProcessTime : " << processTime << "\n";
	std::cout << "Temperature : " << temperature << "\n";
}

bool Recipe::IsSetting() const{
	return isSet;
}

bool Recipe::IsValid(float time, float temperature) const {
	if (time == -1) {
		return false;
	}

	if (temperature == -1) {
		return false;
	}

	return true;
}