#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <vector>
#include "matrix.h"
#include "polygon.h"

void parse_file ( const char * filename, 
		  std::vector<matrix>& world, 
		  matrix&  edges,
		  polygon& polygons,
		  screen& s);

#endif
