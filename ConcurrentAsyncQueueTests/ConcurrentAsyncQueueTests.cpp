// ConcurrentAsyncQueueTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConcurrentAsyncQueueMock.hpp"


int main(int argc, char** argv)
{
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
    return 0;
}

