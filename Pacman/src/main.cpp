#include <iostream>
#include "CMenu.h"

CMenu * menu = nullptr;
int main()
{

   menu = new CMenu();

   menu->start();

   delete menu;

}
