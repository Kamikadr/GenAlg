#include <iostream>
#include <ctime>

constexpr int MaxGens = 3;					// Количество генов (R, G, B)
constexpr int MaxGenerations = 20;			// Количество популяций
constexpr int MaxPopulationUnits = 30;		// Количество особей в популяции
constexpr int MutationDeflect = 2;			// Отклонение при мутировании генов
constexpr double CrossingOverProb = 0.8;	// Вероятность скрещивания
constexpr double MutationProb = 0.18;		// Вероятность мутации

/*
Класс Color 			- основной класс, с которым мы работаем. Представляет собой особь.

Функция Main 			- основная часть программы.

Функция CountAdaptivities 	- функция подсчета критериев оценки популяции для каждой особи.

Функция SortPopulation 	- сортировка популяции: от лучших особей, к худшим.

Функция PrintInfo 		- вывод наглядной информации.

Функция FindBestUnit 		- выделение лучшей особи, и сравнение её с лучшей найденной ранее.

Функция CrossingOver 		- функция скрещивания.

Функция Mutation 		- функция мутации.
*/

class Color
{
public:
	int Chromosome[MaxGens];

	int& R = *Chromosome;
	int& G = *(Chromosome + 1);
	int& B = *(Chromosome + 2);

	void operator=(Color & Col2)
	{
		Chromosome[0] = Col2.Chromosome[0];
		Chromosome[1] = Col2.Chromosome[1];
		Chromosome[2] = Col2.Chromosome[2];

		Adaptivity = Col2.Adaptivity;
	}

	Color() {};
	Color(int R, int G, int B) {
		Chromosome[0] = R;
		Chromosome[1] = G;
		Chromosome[2] = B;
	};

	double Adaptivity = 0;
};

int Clamp(int input, int min, int max)
{
	if (input >= max) return max;
	if (input <= min) return min;

	return input;
}

double randDouble()		// Функция получения случайного числа от 0 до 1
{
	return ((double)rand() / (double)RAND_MAX);
}

Color Population[MaxGenerations][MaxPopulationUnits];	// Массив популяций
Color Goal(96, 96, 159);								// Цель задания
double BestUnitsAdaptivities[MaxGenerations];			// Массив лучших особей


int BestR, BestG, BestB;			// Гены лучшей особи по отдельности
int BestUnitGen;					// Номер поколения лучшей особи
double BestUnitAdaptivity = 3.0;	// Коэф. схожести лучшей особи

int CurrGen = 0;					// Номер текущей популяции

void PrintInfo()
{
	std::cout << "Номер текущей популяции - " << CurrGen << std::endl;

	for (int i = 0; i < MaxPopulationUnits; i++)
	{
		std::cout << "№" << i << ": "
			<< Population[CurrGen][i].R << ", " << Population[CurrGen][i].G << ", " << Population[CurrGen][i].B
			<< " - " << Population[CurrGen][i].Adaptivity << std::endl;
	}
}

void CountAdaptivities()
{
	for (int i = 0; i < MaxPopulationUnits; i++)
	{
		Population[CurrGen][i].Adaptivity = 0;

		for (int j = 0; j < MaxGens; j++)
			Population[CurrGen][i].Adaptivity += fabs((Goal.Chromosome[j] - Population[CurrGen][i].Chromosome[j]) / 255.0);

		//Population[CurrGen][i].Adaptivity = fabs((Goal.R - Population[CurrGen][i].R) / 255.0)
		//	+ fabs((Goal.G - Population[CurrGen][i].G) / 255.0) + fabs((Goal.B - Population[CurrGen][i].B) / 255.0);
	}
}

bool FindBestUnit()
{
	BestUnitsAdaptivities[CurrGen] = Population[CurrGen][0].Adaptivity;

	if (BestUnitAdaptivity > Population[CurrGen][0].Adaptivity)
	{
		BestUnitAdaptivity = Population[CurrGen][0].Adaptivity;

		BestR = Population[CurrGen][0].R;
		BestG = Population[CurrGen][0].G;
		BestB = Population[CurrGen][0].B;

		BestUnitGen = CurrGen;
	}

	return (BestUnitAdaptivity == 0.0);
}

void SortPopulation()
{
	int i = 1, j = 2;
	Color temp(0, 0, 0);

	while (i < MaxPopulationUnits)
	{
		if (Population[CurrGen][i - 1].Adaptivity < Population[CurrGen][i].Adaptivity)
		{
			i = j; j++;
		}
		else
		{
			temp = Population[CurrGen][i - 1];
			Population[CurrGen][i - 1] = Population[CurrGen][i];
			Population[CurrGen][i] = temp;
			i--;
			if (i == 0)
			{
				i = j; j++;
			}
		}
	}
}

void CrossingOver()
{
	int ParentPairsAmount = (int)(randDouble() * int(MaxPopulationUnits * 0.5));

	for (int i = 0; i < ParentPairsAmount; i++)
	{
		if (randDouble() <= CrossingOverProb)
		{
			for (int Gen = 0; Gen < MaxGens; Gen++)
			{
				int rd = rand() % 2;

				Population[CurrGen + 1][i * 2].Chromosome[Gen] =
					rd * Population[CurrGen][i * 2].Chromosome[Gen]
					+ (1 - rd) * Population[CurrGen][i * 2 + 1].Chromosome[Gen];

				Population[CurrGen + 1][i * 2 + 1].Chromosome[Gen] =
					(1 - rd) * Population[CurrGen][i * 2].Chromosome[Gen]
					+ rd * Population[CurrGen][i * 2 + 1].Chromosome[Gen];
			}
		}
		else
		{
			Population[CurrGen + 1][i * 2] = Population[CurrGen][i * 2];
			Population[CurrGen + 1][i * 2 + 1] = Population[CurrGen][i * 2 + 1];
		}
	}

	for (int i = ParentPairsAmount * 2; i < MaxPopulationUnits; i++)
		Population[CurrGen + 1][i] = Population[CurrGen][i];
}

void Mutation()
{
	for (int i = 0; i < MaxPopulationUnits; i++)
	{
		if (randDouble() <= MutationProb)
		{
			for (int j = 0; j < MaxGens; j++)
				if ((rand() % 2))
					Population[CurrGen + 1][i].Chromosome[j] = Clamp(Population[CurrGen + 1][i].Chromosome[j] +
						rand() % MutationDeflect * 2 - MutationDeflect, 0, 255);
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(std::time(0));

	for (int i = 0; i < MaxPopulationUnits; i++)			// Генерируем первое поколение
		for (int j = 0; j < MaxGens; j++)
			Population[CurrGen][i].Chromosome[j] = rand() % 256;

	for (CurrGen = 0; CurrGen < MaxGenerations; CurrGen++)
	{
		CountAdaptivities();

		SortPopulation();

		PrintInfo();

		if (FindBestUnit()) break;

		CrossingOver();

		Mutation();
	}

	std::cout << "\nКоэффициенты схожести лучших особей каждого поколения:\n";

	for (int i = 0; i < MaxGenerations; i++)
		std::cout << i << " поколение: " << BestUnitsAdaptivities[i] << std::endl;

	std::cout << "Общее количество поколений - " << CurrGen << std::endl
		<< "Номер поколения лучшей особи - " << BestUnitGen << std::endl
		<< "Лучшая особь имеет параметры: R = " << BestR
		<< ", G = " << BestG << ", B = " << BestB
		<< ", её коэффициент схожести = " << BestUnitAdaptivity;

	return 0;
}