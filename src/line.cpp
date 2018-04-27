#include <iostream>
#include <vector>
#include <fstream>
#include "../include/line.h"

struct line::Private{
	static void draw_oct1(point p0, point p1, screen& s, pixel color, int a, int b){
		point pn = point(p0[0], p0[1], p0[2]);
		int d = a + b/2;
		while (pn[0] < p1[0]){
			s.plot(pn, color, 0);
			if (d > 0){
				pn[1] +=  1;
				d += (b);	
			}	
			pn[0] += 1;
			d += (a);
		}
		s.plot(pn, color, 0);
	}

	
	static void draw_oct2(point p0, point p1, screen& s, pixel color, int a, int b){
		point pn = point(p0[0], p0[1], p0[2]);
		int d = a/2 + b;
		while (pn[1] < p1[1]){
			s.plot(pn, color, 0);
			if (d < 0){
				pn[0] += 1;
				d += (a);	
			}	
			pn[1] +=  1;
			d += (b);
		}
			s.plot(pn, color, 0);
	}
	static void draw_oct7(point p0, point p1, screen& s, pixel color, int a, int b){
		point pn = point(p0[0], p0[1], p0[2]);
		int d = a/2 + b;
		while (pn[1] > p1[1]){
			s.plot(pn, color, 0);
			if (d > 0){
				pn[0] += 1;
				d += (a);	
			}	
			pn[1] += -1;
			d -= (b);
		}
			s.plot(pn, color, 0);
	}
	static void draw_oct8(point p0, point p1, screen& s, pixel color, int a, int b){
		point pn = point(p0[0], p0[1], p0[2]);
		int d = a - b/2;
		while (pn[0] < p1[0]){
			s.plot(pn, color, 0);
			if (d < 0){
				pn[1] -= 1;
				d -= (b);	
			}	
			pn[0] += 1;
			d += (a);
		}
			s.plot(pn, color, 0);
	}
};


line::line(){
		p0 = std::vector<double>();
		p1 = std::vector<double>();
		color = pixel(0, 0, 0);
}

line::line(int x0, int y0, int x1, int y1){
	std::cout<<"deprecated\n";
		if(x1 -x0 < 0){
			p0 = std::vector<double>();
			p0.push_back(x1);
		        p0.push_back(y1);
			p1 = std::vector<double>();
		       	p1.push_back(x0);
		       	p1.push_back(y0);
		}
		else{
			p0 = std::vector<double>();
			p0.push_back(x0);
		        p0.push_back(y0);
			p1 = std::vector<double>();
		       	p1.push_back(x1);
		       	p1.push_back(y1);
		}
		color = pixel(0, 0, 0);
}

line::line(point pi, point pf){
		if(pf[0] - pi[0] < 0){
			p0 = std::vector<double>();
			p0.push_back(pf[0]);
		        p0.push_back(pf[1]);
			p0.push_back(pf[2]);
			p1 = std::vector<double>();
		       	p1.push_back(pi[0]);
		       	p1.push_back(pi[1]);
		       	p1.push_back(pi[2]);
		}
		else{
			p0 = std::vector<double>();
			p0.push_back(pi[0]);
		        p0.push_back(pi[1]);
		        p0.push_back(pi[2]);
			p1 = std::vector<double>();
		       	p1.push_back(pf[0]);
		       	p1.push_back(pf[1]);
		       	p1.push_back(pf[2]);
		}
		color = pixel(0, 0, 0);
}

line::line(std::vector<double> pi, std::vector<double> pf){
		if(pf[0] - pi[0] < 0){
			p0 = std::vector<double>();
			p0.push_back(pf[0]);
		        p0.push_back(pf[1]);
		        p0.push_back(pf[2]);
			p1 = std::vector<double>();
		       	p1.push_back(pi[0]);
		       	p1.push_back(pi[1]);
		       	p1.push_back(pi[2]);
		}
		else{
			p0 = std::vector<double>();
			p0.push_back(pi[0]);
		        p0.push_back(pi[1]);
		        p0.push_back(pi[2]);
			p1 = std::vector<double>();
		       	p1.push_back(pf[0]);
		       	p1.push_back(pf[1]);
		       	p1.push_back(pf[2]);
		}
		color = pixel(0, 0, 0);
}

void line::set_color(pixel p){
		color = p;
}

void line::draw(screen& s){
	
	int A = 2 * (p1[1]-p0[1]); int B = -2 * (p1[0]-p0[0]);
	int d;
	int x = p0[0]; int y = p0[1];
	int wide = 0; int tall = 0;
	int dy_east, dy_northeast, dx_east, dx_northeast, d_east, d_northeast;
  	int loop_start, loop_end;

	if ( std::abs(p1[0] - p0[0]) >= std::abs(p1[1] - p0[1]) ){
		wide = 1;
   		loop_start = x;
   		loop_end = p1[0];
   		dx_east = dx_northeast = 1;
   		dy_east = 0;
   		d_east = A;
   		if ( A > 0 ) { //octant 1
   			d = A + B/2;
   			dy_northeast = 1;
   			d_northeast = A + B;
   		}
   		else { //octant 8
   			d = A - B/2;
   			dy_northeast = -1;
   			d_northeast = A - B;
    		}
	}
	else { //octant 2/7
    		tall = 1;
    		dx_east = 0;
    		dx_northeast = 1;
    		if ( A > 0 ) {     //octant 2
    			d = A/2 + B;
    			dy_east = dy_northeast = 1;
    			d_northeast = A + B;
    			d_east = B;
    			loop_start = y;
    			loop_end = p1[1];
    		}
    		else {     //octant 7
    			d = A/2 - B;
    			dy_east = dy_northeast = -1;
    			d_northeast = A - B;
    			d_east = -1 * B;
    			loop_start = p1[1];
    			loop_end = y;
    		}
	}

	double z = p0[2];
	double dz = ((double)(p1[2] - p0[2])) / ((double)(loop_end - loop_start));
  	while ( loop_start < loop_end ) {

    		s.plot(x, y, z, color);
    		if ( (wide && ((A > 0 && d > 0) ||
        	        (A < 0 && d < 0)))
        	||
        	(tall && ((A > 0 && d < 0 ) ||
        	           (A < 0 && d > 0) ))) {
      			y+= dy_northeast;
      			d+= d_northeast;
      			x+= dx_northeast;
    		}
    		else {
      			x+= dx_east;
      			y+= dy_east;
			d+= d_east;
    		}
		z+=dz;
    		loop_start++;
  	} //end drawing loop
  	s.plot(x, y, z, color);
}


