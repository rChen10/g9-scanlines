#include "../include/parser.h"

int main(int argc, char **argv){


	screen s = screen(500, 500);
	matrix transform = matrix(4, 4);
	transform.ident();
	std::vector<matrix> world = std::vector<matrix>();
	world.push_back(transform);
	matrix edges = matrix();
	polygon poly = polygon();
	if(argc > 0){
		std::cout << "reading script...";
		parse_file(argv[1], world, edges, poly, s);
	}
	else
		return 1;
}
