#pragma once
#include <vector>

using namespace std;

struct job {
	int id;
	int length;
};

struct citizen {
	int id;
	int fitness;
	int *schedule;
};

class GeneticScheduler {
public:

    GeneticScheduler(
        int population_size,
        int number_of_iterations,
        int number_of_jobs,
        int number_of_processors,
        double mutation_rate,
		vector<job> all_jobs);
	
    void create_optimal_time_schedule();
    void print_result();

private:

    void print_population(vector<citizen> pop);
    bool create_population(vector<citizen> &pop);
    void calc_fitness_population(vector<citizen> &pop);
    int calc_fitness_citizen(citizen a);
    double calc_average_fitness(vector<citizen> &pop);
    void tournament_selection(vector<citizen> pop, vector<citizen> &selected);
    void mate(vector<citizen> pop, vector<citizen> &children);
    void print_schedule(int *schedule);
    citizen create_offspring(citizen a, citizen b);
    citizen find_best_fitness(vector<citizen> population);
    void mutate(vector<citizen> population);

	vector<job> job_pool;
    citizen result;

	int random_processor_number();
    int random_job_number();
	int random_citizen_number();
    int random_parent_number();
    int random_mutation_chance();

	int POP_SIZE;
	int MAX_ITER;

	int nJobs;
	int nProcs;
    double mutation_rate;
};