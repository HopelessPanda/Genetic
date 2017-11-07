#include <vector>
#include <list>
#include <iostream>
#include <time.h>

#define POP_SIZE 10
#define MAX_ITER 10

int *jobs;
int max_time = 0;
int nProc;
int nJobs;

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


void print_schedule(int *schedule) {
	for (int i = 0; i < nJobs; i++) {
		cout << schedule[i] << ' ';
	}
	cout << endl;
}

void print_population(vector<citizen> pop) {
	for (auto i = pop.begin(); i < pop.end(); i++) {
		cout << "citizen #" << i->id << ": ";
		print_schedule(i->schedule);
		cout << "fitness: " << i->fitness << endl << endl;
	}

}

int random_processor_number() {
	return rand() % nProc;
}

int random_citizen_number() {
	return rand() % POP_SIZE;
}

void create_population(vector<citizen> *pop) {

	for (int i = 0; i < POP_SIZE; i++) {
		// Allocate memory for new citizen
		citizen tmp;
		tmp.id = i;
		tmp.schedule = new int[nJobs];

		// Give each job a random processor number
		for (int j = 0; j < nJobs; j++) {
			tmp.schedule[j] = random_processor_number();
		}
		pop->emplace_back(tmp);
	}
}

int find_last_column(int ** schedule, int width, int height) {
	for (int i = 0; i < width; i++) {
		bool filled = false;
		for (int j = 0; j < height; j++)
			if (schedule[i][j] >= 0) {
				filled = true;
				break;
			}
		if (!filled) return i;
	}
	return width;
}

void calc_fitness(vector<citizen> *pop, vector<job> job_pool) {

	int *proc_time = new int[nProc];
	for (auto i = pop->begin(); i < pop->end(); i++) {

		// number of time ticks each processor was busy
		for (int t = 0; t < nProc; t++) proc_time[t] = 0;

		// add job length
		for (int j = 0; j < nJobs; j++)
			proc_time[i->schedule[j]] += job_pool[j].length;
		
		// find max processor time
		int max = 0;
		for (int i = 0; i < nProc; i++) {
			if (max < proc_time[i]) max = proc_time[i];
		}

		i->fitness = max;
	}
}

citizen mate(citizen a, citizen b) {

	return a;
	// create mask for the genome
	// if "1" — the proc line is inhereted from parent #1
	// if "0" — the proc line is inhereted from parent #2
	bool *mask = new bool[nProc];
	for (int i = 0; i < nProc; i++) mask[i] = rand() % 2;


}

void produce_offspring(vector<citizen> parents, vector<citizen> *children) {

	for (int i = 0; i < parents.size(); i++) {
		// choose 2 random elements, produce offspring
		int i1 = rand() % parents.size();
		int i2 = rand() % parents.size();
		children->emplace_back(mate(parents[i1], parents[i2]));
	}
}

void tournament_selection(vector<citizen> pop, vector<citizen> *selected) {

	for (int i = 0; i < pop.size(); i++) {
		// choose 2 random elements, compare their fitness
		int i1 = random_citizen_number();
		int i2 = random_citizen_number();
		citizen winner = (pop[i1].fitness < pop[i2].fitness) ? pop[i1] : pop[i2];
		cout << pop[i1].fitness << " vs " << pop[i2].fitness << endl;
		selected->emplace_back(winner);
	}
}

void mutate() {

}

int main(void) {
	nProc = 4;
	nJobs = 10;
	srand(time(NULL));

	// PARCER
	vector<job> job_pool;
	for (int i = 0; i < nJobs; i++) {
		job temp;
		temp.id = i;
		temp.length = rand() % 10 + 1;
		job_pool.push_back(temp);
		max_time += job_pool[i].length;
	}

	/*cout << "printing jobs: \n";
	for (int i = 0; i < nJobs; i++) {
		cout << job_order[i].id << ' ' << job_order[i].length << endl;

	}*/
	// ------


	vector<citizen> population;
	vector<citizen> selected_parents;
	vector<citizen> children;
	create_population(&population);
	calc_fitness(&population, job_pool);

	tournament_selection(population, &selected_parents);
	print_population(selected_parents);

	produce_offspring(selected_parents, &children);
	calc_fitness(&children, job_pool);
	//print_population(children);
	//cout << children.size() << endl;


	getchar();
}