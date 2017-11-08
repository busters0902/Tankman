#include "GenericFunctions.h"

void GenericFunctions::ChangeFlag(bool& f)
{
	if (f) f = false;
	else f = true;
}