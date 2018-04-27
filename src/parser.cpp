#include "../include/parser.h"
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

char ** parse_args( char *line ){
  char **args = (char **)calloc(10, sizeof(char *));
  char *temp = 0;
  int i = 0;
  while (temp = strsep(&line, " ")){
    args[i] = temp;
    i++;
  }
  return args;
}

void parse_line(char *str, std::vector<matrix>& world, screen& s){
	char **args = parse_args(str);
	point p0 = point(atoi(args[0]), atoi(args[1]), atoi(args[2]));
	point p1 = point(atoi(args[3]), atoi(args[4]), atoi(args[5]));

	matrix temp = matrix();
	temp+=p0;
	temp+=p1;

	temp = world.back() * temp;
	
	line l = line(temp[0], temp[1]);
	l.draw(s);
}

void parse_scale(char *str, std::vector<matrix>& world){
	char **args = parse_args(str);
	std::vector<double> p;
	for (int j = 0; j < 3; j++){
		p.push_back(atof(args[j]));
	}
	matrix scale = create_scale(p);
	world.back() = world.back() * scale;
}

void parse_translate(char *str, std::vector<matrix>& world){
	char **args = parse_args(str);
	std::vector<double> p;
	for (int j = 0; j < 3; j++){
		p.push_back(atof(args[j]));
	}
	matrix translate = create_translate(p);
	world.back() = world.back() * translate;
}

void parse_rotate(char *str, std::vector<matrix>& world){
	char **args = parse_args(str);
	matrix rotate = matrix(4, 4);
	rotate.ident();
	if (strncmp(args[0], "x", 1) == 0){
		rotate = create_rotateX(atof(args[1]));
	}
	else if (strncmp(args[0], "y", 1) == 0){
		rotate = create_rotateY(atof(args[1]));
	}
	else if (strncmp(args[0], "z", 1) == 0){
		rotate = create_rotateZ(atof(args[1]));
	}
	world.back() = world.back() * rotate;

}

void parse_display(screen& s){
	FILE *f;
	f = popen("display", "w");
	s.render(f);
	pclose(f);
}

void parse_hermite(char *str, std::vector<matrix>& world, screen& s){	
	std::vector<double> x = std::vector<double>(4, 0); std::vector<double> y = std::vector<double>(4, 0); int step = 25;
	char **args = parse_args(str);
	int j = 0;
	for(int i = 0; i < 8; i+=2){
		x[j] = atof(args[i]);
		y[j] = atof(args[i+1]);
		j++;
	}
	matrix intermediate = matrix(); intermediate+=x; intermediate+=y;
	matrix coeffs = generate_curve_coefs(intermediate, 0);
	matrix hermite = make_curve(coeffs, step);
	
	hermite = world.back() * hermite;

	hermite.draw(s);
}
void parse_bezier(char *str, std::vector<matrix>& world, screen& s){
	std::vector<double> x = std::vector<double>(4, 0); std::vector<double> y = std::vector<double>(4, 0); int step = 25;
	char **args = parse_args(str);
	int j = 0;
	for(int i = 0; i < 8; i+=2){
		x[j] = atof(args[i]);
		y[j] = atof(args[i+1]);
		j++;
	}
	matrix intermediate = matrix(); intermediate+=x; intermediate+=y;
	matrix coeffs = generate_curve_coefs(intermediate, 1);
	matrix bezier = make_curve(coeffs, step);

	bezier = world.back() * bezier;

	bezier.draw(s);
}
void parse_circle(char *str, std::vector<matrix>& world, screen& s){
	double r; std::vector<double> pc = std::vector<double>(4, 0); int step = 25;
	char **args = parse_args(str);
	for(int i = 0; i < 3; i++){
		pc[i] = atof(args[i]);
	}
	r = atof(args[3]);
	matrix circle = make_circle(r, pc, step);

	circle = world.back() * circle;

	circle.draw(s);
}
void parse_sphere(char *str, std::vector<matrix>& world, screen& s){
	double r; std::vector<double> pc = std::vector<double>(4, 0); int step = 25;
	char **args = parse_args(str);
	for(int i = 0; i < 3; i++){
		pc[i] = atof(args[i]);
	}
	r = atof(args[3]);
	matrix sphere = create_sphere(pc, r, step);
	sphere = world[world.size()-1] * sphere;
	polygon p_sphere = sphere;
	p_sphere.draw(s);
}
void parse_torus(char *str, std::vector<matrix>& world, screen& s){
	double r; double R; std::vector<double> pc = std::vector<double>(4, 0); int step = 25;
	char **args = parse_args(str);
	for(int i = 0; i < 3; i++){
		pc[i] = atof(args[i]);
	}
	r = atof(args[3]);
	R = atof(args[4]);
	matrix torus = create_torus(pc, r, R, step);
	torus = world[world.size()-1] * torus;
	polygon p_torus = torus;
	p_torus.draw(s);
}
void parse_box(char *str, std::vector<matrix>& world, screen& s){
	double w; double h; double d; std::vector<double> pc = std::vector<double>(4, 0);
	char **args = parse_args(str);
	for(int i = 0; i < 3; i++){
		pc[i] = atof(args[i]);
	}
	w = atof(args[3]);
	h = atof(args[4]);
	d = atof(args[5]);
	matrix box = create_box(pc, w, h, d);
	box = world[world.size()-1] * box;
	polygon p_box = box;
	p_box.draw(s);
}
void parse_save(screen& s, char *str){
	s.render(str);
}
void parse_push(std::vector<matrix>& world){
	matrix copy = world.back().copy();
	world.push_back(copy);
}
void parse_pop(std::vector<matrix>& world){
	world.pop_back();
}
void parse_file ( const char * filename, 
		  std::vector<matrix>&  world, 
		  matrix&  edges,
		  polygon& poly,
		  screen& s){
	std::ifstream f;
	f.open(filename);
	std::cout<<"file read\n";
	char * str = (char *) calloc(250, sizeof(char));
	
	f.getline(str, 250);


	while ( (f.rdstate() & std::ifstream::eofbit ) == 0 ){
		if(strncmp(str, "line", 250) == 0){
			f.getline(str, 250);
			parse_line(str, world, s);
		}
		
		else if(strncmp(str, "scale", 250) == 0){
			f.getline(str, 250);
			parse_scale(str, world);
		}

		else if(strncmp(str, "move", 250) == 0){
			f.getline(str, 250);
			parse_translate(str, world);
		}

		else if(strncmp(str, "rotate", 250) == 0){
			f.getline(str, 250);
			parse_rotate(str, world);
		}

		else if(strncmp(str, "clear", 250) == 0){
			s.clear();
		}

		else if(strncmp(str, "push", 250) == 0){
			parse_push(world);
		}

		else if(strncmp(str, "pop", 250) == 0){
			parse_pop(world);
		}

		else if(strncmp(str, "display", 250) == 0){
			parse_display(s);
		}

		else if(strncmp(str, "save", 250) == 0){
			f.getline(str, 250);
			parse_save(s, str);
		}

		else if(strncmp(str, "hermite", 250) == 0){
			f.getline(str, 250);
			parse_hermite(str, world, s);
		}
		
		else if(strncmp(str, "bezier", 250) == 0){
			f.getline(str, 250);
			parse_bezier(str, world, s);
		}

		else if(strncmp(str, "circle", 250) == 0){
			f.getline(str, 250);
			parse_circle(str, world, s);
		}

		else if(strncmp(str, "sphere", 250) == 0){
			f.getline(str, 250);
			parse_sphere(str, world, s);
		}

		else if(strncmp(str, "torus", 250) == 0){
			f.getline(str, 250);
			parse_torus(str, world, s);
		}

		else if(strncmp(str, "box", 250) == 0){
			f.getline(str, 250);
			parse_box(str, world, s);
		}

		f.getline(str, 250);
	}
}
