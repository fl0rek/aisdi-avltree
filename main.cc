/**
@file main.cc

Plik z funkcja main() do cwiczenia 4 na laboratoriach z AISDI.

@author
Pawel Cichocki, Michal Nowacki

@date
last revision
- 2005.12.01 Michal Nowacki: lab #4
- 2005.10.27 Pawel Cichocki: added some comments
- 2005.10.26 Michal Nowacki: creation - separated from another file

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <iostream>

#ifdef _SUNOS
#include "/materialy/AISDI/tree/TreeMap.h"
#include "timer.h"
#else
#include "TreeMap.h"
#endif

int CCount::count=0;

std::string print_tree(TreeMap& t);

int Test2();
void test();

int main() {
        TreeMap tm;
        tm.insert(std::make_pair(1, "1"));
        tm.insert(std::make_pair(2, "2"));
        tm.insert(std::make_pair(3, "3"));
        std::cout << print_tree(tm);

        for(auto it = tm.begin(); it != tm.end(); it++)
                std::cout << it->first << std::endl;
        tm.erase(2);
        std::cout << print_tree(tm);
}

#if 0
int main2() {
   struct time_m czasstart;
   double czas;
   std::cout << "AISDI tree: wchodze do funkcji main." << std::endl;
   test();
   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
   czasstart = timer_start();
   Test2();
   czas = timer_stop( czasstart );
   std::cout << std::endl << "Czas wykonania Test2() : " << czas << " s." << std::endl;
   //system("PAUSE");
   return EXIT_SUCCESS;
}
#endif
