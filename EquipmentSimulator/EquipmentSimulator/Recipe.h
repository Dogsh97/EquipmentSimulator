#pragma once

class Recipe {
	private:
		int recipeId;
		float processTime;
		float temperature;
		bool isSet;

	public:
		Recipe();

		void SetRecipe(int id, float time, float temperature);
		void PrintInfo() const;
		bool IsSetting() const;
		bool IsValid(int id, float time, float temperature) const;
};
