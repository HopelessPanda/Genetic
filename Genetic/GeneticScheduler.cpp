#include "GeneticScheduler.h"
#include <iostream>
#include <limits.h>

using namespace std;

int GeneticScheduler::random_processor_number() {
	return rand() % nProcs;
}

int GeneticScheduler::random_job_number()
{
    return rand() % nJobs;
}

int GeneticScheduler::random_citizen_number() {
	return rand() % POP_SIZE;
}

int GeneticScheduler::random_parent_number()
{
    return rand() % 2;
}

int GeneticScheduler::random_mutation_chance() {
    return rand() / RAND_MAX;
}


void GeneticScheduler::tournament_selection(vector<citizen> pop, vector<citizen> &selected) {

	for (int i = 0; i < pop.size(); i ++) {
		// choose random elements, compare their fitness
        vector<citizen> competitors;
        int i1 = random_citizen_number();
		int i2 = random_citizen_number();
        int i3 = random_citizen_number();
        int i4 = random_citizen_number();
        competitors.emplace_back(pop[i1]);
        competitors.emplace_back(pop[i2]);
        competitors.emplace_back(pop[i3]);
        competitors.emplace_back(pop[i4]);
        citizen winner = find_best_fitness(competitors);
		selected.emplace_back(winner);
	}
}

citizen GeneticScheduler::create_offspring(citizen a, citizen b)
{
    citizen child;
    child.id = a.id;

    int *gene_mask = new int[nJobs];
    for (int i = 0; i < nJobs; i++) gene_mask[i] = random_parent_number();

    child.schedule = new int[nJobs];
    citizen donor;
    for (int i = 0; i < nJobs; i++) {
        if (gene_mask[i] == 0) donor = a;
        else donor = b;
        child.schedule[i] = donor.schedule[i];
    }
    delete[] gene_mask;

    return child;
}

citizen GeneticScheduler::find_best_fitness(vector<citizen> pop)
{
    int min = INT_MAX;
    citizen best;
    best.id = -1;
    for (auto i = pop.begin(); i < pop.end(); i++) {
        if (i->fitness < min) {
            min = i->fitness;
            best = *i;
        }
    }
    return best;
}

void GeneticScheduler::mutate(vector<citizen> population)
{
    for (auto i = population.begin(); i < population.end(); i++) {
        if (random_mutation_chance() <= mutation_rate) {
            int reverse_start = random_job_number();
            if (reverse_start == nJobs - 1) continue;
            int tmp = 1;
            for (int j = reverse_start; j < (nJobs-reverse_start)/2 + reverse_start; j++) {
                swap(i->schedule[j], i->schedule[nJobs - tmp]);
                tmp++;
            }
        }
    }
}

void GeneticScheduler::mate(vector<citizen> pop, vector<citizen> &children)
{
    for (auto i = pop.begin(); i < pop.end(); i++) {
        citizen second_parent = pop[random_citizen_number()];
        citizen child = create_offspring(*i, second_parent);
        children.emplace_back(child);
    }
}

int GeneticScheduler::calc_fitness_citizen(citizen a) {
    // number of time ticks each processor was busy
    int *proc_time = new int[nProcs];
    for (int t = 0; t < nProcs; t++) proc_time[t] = 0;

    // add job length
    for (int j = 0; j < nJobs; j++)
        proc_time[a.schedule[j]] += job_pool[j].length;

    // find max processor time
    int max = 0;
    for (int i = 0; i < nProcs; i++) {
        if (max < proc_time[i]) max = proc_time[i];
    }
    delete[] proc_time;
    return max;
}

double GeneticScheduler::calc_average_fitness(vector<citizen>& pop)
{
    double total = 0;
    for (auto i = pop.begin(); i < pop.end(); i++) {
        total += i->fitness;
    }
    return total / pop.size();
}

void GeneticScheduler::calc_fitness_population(vector<citizen> &pop) {
	for (auto i = pop.begin(); i < pop.end(); i++) {
		i->fitness = calc_fitness_citizen(*i);
	}
}

bool GeneticScheduler::create_population(vector<citizen> &pop) {

    if (nJobs < 1 || nProcs < 1 || MAX_ITER < 1 || POP_SIZE < 1) {
        cout << "Bad input" << endl;
        return false;
    }

	for (int i = 0; i < POP_SIZE; i++) {
		// Allocate memory for new citizen
		citizen tmp;
		tmp.id = i;
		tmp.schedule = new int[nJobs];

		// Give each job a random processor number
		for (int j = 0; j < nJobs; j++) {
            tmp.schedule[j] = random_processor_number();
		}
		pop.emplace_back(tmp);
	}
}

GeneticScheduler::GeneticScheduler(int population_size, int number_of_iterations, int number_of_jobs, int number_of_processors, double mutations, vector<job> all_jobs)
{
	POP_SIZE = population_size;
	MAX_ITER = number_of_iterations;
	nJobs = number_of_jobs;
	nProcs = number_of_processors;		 
	job_pool = all_jobs;
    mutation_rate = mutations;
    result.id = -1;
    result.fitness = -1;
}

void GeneticScheduler::print_schedule(int *schedule) {
	for (int i = 0; i < nJobs; i++) {
		cout << schedule[i] << ' ';
	}
	cout << endl;
}

void GeneticScheduler::print_population(vector<citizen> pop) {
	for (auto i = pop.begin(); i < pop.end(); i++) {
		cout << "citizen #" << i->id << ": ";
		print_schedule(i->schedule);
		cout << "fitness: " << i->fitness << endl << endl;
	}
}

void GeneticScheduler::print_result()
{
    /*cout << "Input jobs:\n";
    for (auto i = job_pool.begin(); i < job_pool.end(); i++) {
        cout << "   " << i->id << " " << i->length << endl;
    } */

    cout << endl << "Optimal schedule on " << nProcs << " processors: \n";
    if(result.schedule != NULL) print_schedule(result.schedule);
    cout << "Time required:\n" << result.fitness << endl << endl;
}