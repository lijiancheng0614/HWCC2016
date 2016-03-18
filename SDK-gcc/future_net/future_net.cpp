#include "route.h"
#include "lib_io.h"
#include "lib_time.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
    print_time("Begin");

    char *topo_file = argv[1];
    vector<vi> topo = read_file(topo_file);
    char *demand_file = argv[2];
    vector<vi> demand = read_file(demand_file);
    
    search_route(topo, demand[0]);

    char *result_file = argv[3];
    write_result(result_file);
    
    print_time("End");

	return 0;
}
