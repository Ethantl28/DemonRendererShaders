/** \file entryPoint.hpp*/
#pragma once

#include "core/application.hpp"

extern Application* startApplication();

int main(int argc, char** argv)
{
	auto application = startApplication();
	application->run();
	delete application;

	return 0;
}

