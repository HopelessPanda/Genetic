#include <vector>
#include <list>
#include <iostream>
#include <time.h>
#include "GeneticScheduler.h"
#include "tinyxml2.h"
#include "TestGenerator.h"

using namespace std;
using namespace tinyxml2;

void GeneticScheduler::create_optimal_time_schedule() {
    vector<citizen> population;
    vector<citizen> selected_parents;
    vector<citizen> children;
    citizen global_best;
    int count = 0;

    if(!create_population(population))
        return;
    calc_fitness_population(population);
    global_best = find_best_fitness(population);

    for (int i = 0; i < MAX_ITER; i++) {    
        tournament_selection(population, selected_parents);

        mate(selected_parents, children);
        mutate(children);
        selected_parents.clear();               
        
        population = move(children); 
        calc_fitness_population(population);
        result = find_best_fitness(population);
        //cout << i << ' ' << global_best.fitness << ' ' << result.fitness << endl;

        switch (compare_citizen(global_best, result)) {
        case EQUAL: {count++; break; }
        case SMALLER: {break; }
        case BIGGER: {global_best = result; count = 0; break; }
        }
        if (count == 20) break;
     }
    result = global_best;
}


int main(void) {
    srand(time(NULL));
    int number_of_tests = 10;

    if (false) {
        GenerateMyTests(number_of_tests);
        return 0;
    }

    int number_of_jobs;
    int number_of_processors;
    int population_size = 100;
    

    for (int i = 0; i < number_of_tests; i++) {
        XMLDocument xmlDoc;
        string name = "Test-" + to_string(i) + ".xml";
        XMLError eResult = xmlDoc.LoadFile(name.c_str());
        XMLNode * pRoot = xmlDoc.FirstChild();

        XMLElement * pElement = pRoot->FirstChildElement("Number_of_jobs");
        eResult = pElement->QueryIntText(&number_of_jobs);

        pElement = pRoot->FirstChildElement("Number_of_processors");
        eResult = pElement->QueryIntText(&number_of_processors);

        pElement = pRoot->FirstChildElement("List");
        XMLElement * pListElement = pElement->FirstChildElement("Item");
        int j = 0;
        vector<job> job_pool;
        while (pListElement != nullptr)
        {
            job tmp;
            tmp.id = j++;
            eResult = pListElement->QueryIntText(&tmp.length);

            job_pool.push_back(tmp);
            pListElement = pListElement->NextSiblingElement("Item");
        }


        int number_of_iterations = 100 + sqrt(number_of_jobs * number_of_processors);
        GeneticScheduler GS(population_size, number_of_iterations, number_of_jobs, number_of_processors, 0.1, job_pool);
        GS.create_optimal_time_schedule();
        GS.print_result();
        job_pool.clear();
    }
    cout << "Done!\n";
    
	getchar();
}