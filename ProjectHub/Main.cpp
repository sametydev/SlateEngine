#include <stdio.h>
#include <iostream>
#include "ProjectHub.h"

int main(int, char**) {
	new ProjectHub();
	ProjectHub::Instance->Init();
	return ProjectHub::Instance->OnRender();
}