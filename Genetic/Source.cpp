#include <vector>
#include <list>
#include <iostream>
#include <time.h>

#define POP_SIZE 10
#define MAX_ITER 10

int *jobs;
int max_time = 0;
int nProc;

using namespace std;

struct job {
	int id;
	int length;
	int cur_time;
};

struct table {
	double fitness;
	int **schedule;
};

void shuffle(vector<job> *arr, int n)
{
	for (int i = 0; i < n - 1; i++) {
		int j = i + rand() / (RAND_MAX / (n - i) + 1);
		job t = (*arr)[j];
		(*arr)[j] = (*arr)[i];
		(*arr)[i] = t;
	}
}
table create_element(int nProcs, vector<job> job_order) {
	auto cur_job = job_order.begin();

	// allocate memory for the schedule — a double array
	table elem;
	elem.schedule = new int*[max_time];
	for (int j = 0; j < max_time; j++) {
		elem.schedule[j] = new int[nProcs];
		for (int h = 0; h < nProcs; h++) elem.schedule[j][h] = -1;
	}

	// fill every time tick...
	for (int t = 0; t < max_time; t++) {

		// ...for every process
		for (int p = 0; p < nProcs; p++) {

			// is previous job still running?
			if (elem.schedule[t][p] >= 0) continue;
			else {

				// nope, start next one; fill in all next ticks for the duration of the job
				for (int l = 0; l < (*cur_job).length; l++) {
					elem.schedule[t + l][p] = (*cur_job).id;
				}
				cur_job++;
				if (cur_job == job_order.end()) {
					return elem;
				}
			}
		}
	}
}
void create_population(vector<table> *pop, int nProcs, int nJobs, vector<job> job_order) {

	for (int i = 0; i < POP_SIZE; i++) {
		// for each element in the population job order will be different
		shuffle(&job_order, nJobs);
		cout << "creating element " << i << ":\n";
		for (auto i = job_order.begin(); i < job_order.end(); i++) {
			cout << (*i).id << ' ' << (*i).length << endl;
		}

		(*pop).push_back(create_element(nProcs, job_order));

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

void print_schedule(int **schedule) {
	int limit = find_last_column(schedule, max_time, nProc);
	for (int i = 0; i < nProc; i++) {
		cout << "PROC " << i << ": ";
		for (int t = 0; t < limit; t++) {
			cout << schedule[t][i] << ' ';
		}
		cout << endl;
	}
}

void print_population(vector<table> pop) {
	for (auto i = pop.begin(); i < pop.end(); i++) {
		print_schedule(i->schedule);
		cout << i->fitness << endl;
	}

}

void calc_fitness(vector<table> *pop, int width, int nProc) {

	for (auto i = pop->begin(); i < pop->end(); i++) {
		i->fitness = find_last_column(i->schedule, max_time, nProc);
	}
}

table mate(table a, table b) {

	return a;
	// create mask for the genome
	// if "1" — the proc line is inhereted from parent #1
	// if "0" — the proc line is inhereted from parent #2
	bool *mask = new bool[nProc];
	for (int i = 0; i < nProc; i++) mask[i] = rand() % 2;

	
}

void produce_offspring(vector<table> parents, vector<table> *children) {

	for (int i = 0; i < parents.size(); i++) {
		// choose 2 random elements, produce offspring
		int i1 = rand() % parents.size();
		int i2 = rand() % parents.size();
		children->emplace_back(mate(parents[i1], parents[i2]));
	}
}

void tournament_selection(vector<table> pop, vector<table> *selected) {

	for (int i = 0; i < pop.size(); i++) {
		// choose 2 random elements, compare their fitness
		int i1 = rand() % pop.size();
		int i2 = rand() % pop.size();
		table winner = (pop[i1].fitness < pop[i2].fitness) ? pop[i1] : pop[i2];
		selected->emplace_back(winner);
	}
}

void mutate() {

}

int main(void) {
	nProc = 4;
	int nJobs = 10;
	srand(time(NULL));

	// PARCER
	vector<job> job_order;
	for (int i = 0; i < nJobs; i++) {
		job temp;
		temp.id = i;
		temp.length = rand() % 10 + 1;
		job_order.push_back(temp);
		max_time += job_order[i].length;
	}

	/*cout << "printing jobs: \n";
	for (int i = 0; i < nJobs; i++) {
		cout << job_order[i].id << ' ' << job_order[i].length << endl;

	}*/
	// ------


	vector<table> population;
	vector<table> selected_parents;
	vector<table> children;
	create_population(&population, nProc, nJobs, job_order);
	calc_fitness(&population, max_time, nProc);

	tournament_selection(population, &selected_parents);
	produce_offspring(selected_parents, &children);
	calc_fitness(&children, max_time, nProc);
	//print_population(children);
	//cout << children.size() << endl;


	getchar();
}