#include "Internet.h"
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <Sensapi.h>
#pragma comment(lib, "Sensapi.lib")

using namespace std;

bool checkInternetConnection()
{
	bool connected;

	DWORD dwSens;
	if (IsNetworkAlive(&dwSens) == FALSE)
	{
		connected = false;
	}
	else
	{
		connected = true;
	}

	return connected;
}

