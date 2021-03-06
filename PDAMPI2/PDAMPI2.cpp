// PDAMPI2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mpi.h"
#include <iostream>
#include <random>

using namespace std;

#define MAXNR 20

int main(int argc, char *argv[])
{
	int rank, len, rnd, buf[2], actlead[2];
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &len);

	rnd = rand() % MAXNR;
	buf[0] = rnd;
	buf[1] = rank;
	actlead[0] = rnd;
	actlead[1] = rank;

	if (rank == 0)
	{
		MPI_Send(&actlead, 2, MPI_INT, rank + 1, 1234, MPI_COMM_WORLD);
		MPI_Recv(&buf, 2, MPI_INT, MPI_ANY_SOURCE, 1234, MPI_COMM_WORLD, &status);
		if (actlead[0] < buf[0])
		{
			actlead[0] = buf[0];
			actlead[1] = buf[1];
		}
		else if (actlead[0] == buf[0])
		{
			if (actlead[1] < buf[1])
			{
				actlead[1] = buf[1];
			}
		}
		MPI_Send(&actlead, 2, MPI_INT, rank + 1, 1234, MPI_COMM_WORLD);
	}
	else if (rank != (len-1) )
	{
		MPI_Recv(&buf, 2, MPI_INT, MPI_ANY_SOURCE, 1234, MPI_COMM_WORLD, &status);
		if (actlead[0] < buf[0])
		{
			actlead[0] = buf[0];
			actlead[1] = buf[1];
		}
		else if (actlead[0] == buf[0])
		{
			if (actlead[1] < buf[1])
			{
				actlead[1] = buf[1];
			}
		}
		MPI_Send(&actlead, 2, MPI_INT, rank + 1, 1234, MPI_COMM_WORLD);
		MPI_Recv(&actlead, 2, MPI_INT, MPI_ANY_SOURCE, 1234, MPI_COMM_WORLD, &status);
	}
	else
	{
		MPI_Recv(&buf, 2, MPI_INT, MPI_ANY_SOURCE, 1234, MPI_COMM_WORLD, &status);
		if (actlead[0] < buf[0])
		{
			actlead[0] = buf[0];
			actlead[1] = buf[1];
		}
		else if (actlead[0] == buf[0])
		{
			if (actlead[1] < buf[1])
			{
				actlead[1] = buf[1];
			}
		}
		MPI_Send(&buf, 2, MPI_INT, 0, 1234, MPI_COMM_WORLD);
		MPI_Recv(&actlead, 2, MPI_INT, MPI_ANY_SOURCE, 1234, MPI_COMM_WORLD, &status);
	}

	cout << "Process nr. " << rank << ": Leader -> " << actlead[1] << ", Generated nr -> " << actlead[0] << endl;

	MPI_Finalize();

    return 0;
}

