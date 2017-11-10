#pragma once
#include "tinyxml2.h"
#include <string>

using namespace std;
using namespace tinyxml2;

void GenerateMyTests(int number_of_tests) {

    int number_of_jobs = 10;
    int number_of_procs = 2;
    int job_length = 1;
    for (int i = 0; i < number_of_tests; i++) {
        XMLDocument xmlDoc;

        XMLNode * pRoot = xmlDoc.NewElement("Root");
        xmlDoc.InsertFirstChild(pRoot);

        XMLElement * pElement = xmlDoc.NewElement("Number_of_jobs");
        pElement->SetText(number_of_jobs);
        pRoot->InsertEndChild(pElement);

        pElement = xmlDoc.NewElement("Number_of_processors");
        pElement->SetText(number_of_procs);
        pRoot->InsertEndChild(pElement);

        pElement = xmlDoc.NewElement("List");
        for (int i = 0; i < number_of_jobs; i++)
        {
            XMLElement * pListElement = xmlDoc.NewElement("Item");
            pListElement->SetText(job_length);

            pElement->InsertEndChild(pListElement);
        }
        pRoot->InsertEndChild(pElement);

        string name = "Test-" + to_string(i) + ".xml";
        XMLError eResult = xmlDoc.SaveFile(name.c_str());
             
        number_of_jobs *= 2;
        number_of_procs *= 2;
        job_length++;
    }

}