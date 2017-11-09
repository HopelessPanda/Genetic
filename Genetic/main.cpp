#include <vector>
#include <list>
#include <iostream>
#include <time.h>
#include "GeneticScheduler.h"

using namespace std;

void GeneticScheduler::create_optimal_time_schedule() {
    vector<citizen> population;
    vector<citizen> selected_parents;
    vector<citizen> children;

    double stop = 0.01;
    citizen previous_best;

    if(!create_population(population))
        return;

    for (int i = 0; i < MAX_ITER; i++) {
        calc_fitness_population(population);
        previous_best = find_best_fitness(population);

        tournament_selection(population, selected_parents);

        mate(selected_parents, children);
        mutate(children);
        selected_parents.clear();
        calc_fitness_population(children);
        
        population = move(children); 
        result = find_best_fitness(population);
        cout << i << ' ' << previous_best.fitness << ' ' << result.fitness << endl;
        if (previous_best.fitness == result.fitness) {
            //break;
        }
     }
    result = find_best_fitness(population);
   // result.fitness = calc_fitness_citizen(result);
}

int main(void) {

	srand(time(NULL));
	int number_of_jobs = 20;
	int number_of_processors = 5;
    int number_of_iterations = 100;
    int population_size = 1000;

	// PARCER ----------------------------------------------
	vector<job> job_pool;
	for (int i = 0; i < number_of_jobs; i++) {
		job temp;
		temp.id = i;
		temp.length = rand() % 10 + 1;
		job_pool.push_back(temp);
	}
	// ------------------------------------------------------


	GeneticScheduler GS(population_size, number_of_iterations, number_of_jobs, number_of_processors, 0, job_pool);
    GS.create_optimal_time_schedule();
    GS.print_result();

    cout << "Done!\n";
    
	getchar();
}