#include <stdio.h>
#include <iostream>
#include "ProjectHub.h"

int main(int, char**) {
	new ProjectHub();
	return ProjectHub::Instance->Init();
}