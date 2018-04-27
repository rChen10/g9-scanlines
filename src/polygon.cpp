#include "../include/polygon.h"

polygon::polygon(matrix& mat){
	m = mat.m;
	s = mat.s;
}

polygon::polygon(){
	m = std::vector< std::vector<double> >();
	s = 0;
}

polygon& polygon::operator+=(line l){
	return (*this);
}

void polygon::draw(screen& scr){
	for(int i = 0; i < s-2; i+=3){
		my::vector v1 = my::vector(m[i], m[i+1]);
		my::vector v2 = my::vector(m[i], m[i+2]);

		my::vector cross = v1.cross(v2);
		if(cross[2] > 0){
		//	line l1 = line(m[i], m[i+1]);
		//	line l2 = line(m[i+1], m[i+2]);
		//	line l3 = line(m[i+2], m[i]);

		//	l1.draw(scr);
		//	l2.draw(scr);
		//	l3.draw(scr);

			pixel p = pixel();
			p.r = (i+1) * 242 % 255;//((int)((i+1) + m[i][0] + m[i][1] + m[i][2])) % 255;
			p.g = (i+1) * 8775 % 255;//((int)((i+1) + m[i+1][0] + m[i+1][1] + m[i+1][2])) % 255;
			p.b = (i+1) * 2222 % 255;//((int)((i+1) + m[i+2][0] + m[i+2][1] + m[i+2][2])) % 255;

			fill_poly(m[i], m[i+1], m[i+2], scr, p);
		}
	}	
}

polygon& polygon::operator+=(std::vector<int>& p){
	matrix poly = (*this);
	poly+=p;
	return (*this);
}

polygon& polygon::operator+=(std::vector<double>& p){
	matrix poly = (*this);
	poly+=p;
	return (*this);
}

polygon& polygon::operator+=(matrix& p){
	matrix poly = (*this);
	poly+=p;
	return (*this);
}

polygon operator*(matrix& m, polygon& p){
	matrix poly = p;
	matrix mult = m * poly;
	polygon ret = polygon(mult);
	return ret;
}

void swap(std::vector<double>& p1, std::vector<double>& p2){
	std::vector<double> temp = p1;
	p1 = p2;
	p2 = temp;
}

void fill_poly(std::vector<double> t, std::vector<double> m, std::vector<double> b, screen& s, pixel color){
	//filtering
	//if (t[1] < m[1] || m[1] < b[1]){
		//std::cout<<"lol: "<<t[1]<<"\n";
		if(m[1] < b[1]){
			swap(m, b);
		}
		if(t[1] < b[1]){
			swap(t, b);
			//std::cout<<"lul: "<<t[1]<<"\n";
		}
		if(t[1] < m[1]){
			swap(t, m);
		}
	//}
//	if (t[1] < m[1] || m[1] < b[1] || t[1] < b[1]){
//		std::cout<<"here\n"<<t[1]<<"\n\n";
//	}
//	std::cout<<t[1]<<"\n";
//	std::cout<<m[1]<<"\n";
//	std::cout<<b[1]<<"\n";
//	std::cout<<"\n";

//	std::cout<<"nice\n";
//	std::cout<<t[0]<<" "<<t[1]<<" "<<t[2]<<"\n";
//	std::cout<<m[0]<<" "<<m[1]<<" "<<m[2]<<"\n";
//	std::cout<<b[0]<<" "<<b[1]<<" "<<b[2]<<"\n";


	//setup
	double x0 = (double)b[0];
	double x1 = (double)b[0];
	double z0 = (double)b[2];
	double z1 = (double)b[2];

	double dx0 = (t[0]-b[0])/(t[1]-b[1]);
	double dx1_current;
	double dx1_swap;

	double dz0 = (t[2]-b[2])/(t[1]-b[1]);
	double dz1_current;
	double dz1_swap;

	if (m[1]-b[1] >= 1){
		dx1_current = (m[0]-b[0])/(m[1]-b[1]);
		dz1_current = (m[2]-b[2])/(m[1]-b[1]);
	}
	else{
		dx1_current = 0;
		dz1_current = 0;
		//if(t[0] == b[0])
		//	std::cout<<"here\n"<<t[0]<<" "<<m[0]<<" "<<b[0]<<"\n"<<dx0<<" "<<dx1_current<<" "<<dx1_swap<<"\n\n";
	}
	if (t[1]-m[1] >= 1){
		dx1_swap = (t[0]-m[0])/((t[1]-m[1]));
		dz1_swap = (t[2]-m[2])/((t[1]-m[1]));
	}
	else{
		dx1_swap = 0;
		dz1_swap = 0;
	}

	if(dz1_current > 80 || dz1_swap > 80){
		std::cout<<dz1_current<<" "<<dz1_swap<<"\n\n";
	}

	//std::cout<<"here\n"<<dx0<<" "<<dx1_current<<" "<<dx1_swap<<"\n\n";
	//algorithm
	int y0 = (int)b[1];

	for(; y0 <= (int)m[1]; y0++){
		//std::cout<<z1<<"\n";
		std::vector<double> p0 = std::vector<double>();
		p0.push_back((int)x0);
		p0.push_back(y0);
		p0.push_back(z0);

		std::vector<double> p1 = std::vector<double>();
		p1.push_back((int)x1);
		p1.push_back(y0);
		p1.push_back(z1);

		line temp = line(p0, p1);
		temp.set_color(color);
		temp.draw(s);

		x0+=dx0;
		z0+=dz0;

		x1+=dx1_current;
		z1+=dz1_current;
	}

	x1 = m[0];
	y0 = (int)m[1];
	z1 = m[2];

	for(; y0 <= (int)t[1]; y0++){
		//std::cout<<x1<<"\n";
		std::vector<double> p0 = std::vector<double>();
		p0.push_back((int)x0);
		p0.push_back(y0);
		p0.push_back(z0);

		std::vector<double> p1 = std::vector<double>();
		p1.push_back((int)x1);
		p1.push_back(y0);
		p1.push_back(z1);

		line temp = line(p0, p1);
		temp.set_color(color);
		temp.draw(s);

		x0+=dx0;
		z0+=dz0;

		x1+=dx1_swap;
		z0+=dz1_swap;
	}
//`	std::cout<<"nice\n"<<haha<<"\n"<<y0<<"\n\n";
}

matrix generate_curve_coefs(matrix& a, int type){
	matrix coefs;
	if(type == 0){
		matrix m = matrix();
		point r1 = point(2, -3, 0, 1);
		point r2 = point(-2, 3, 0, 0);
		point r3 = point(1, -2, 1, 0);
		point r4 = point(1, -1, 0, 0);
		m+=r1;m+=r2;m+=r3;m+=r4;

		coefs = m * a;
	}

	else{	
		matrix m = matrix();
		point r1 = point(-1, 3, -3, 1);
		point r2 = point(3, -6, 3, 0);
		point r3 = point(-3, 3, 0, 0);
		point r4 = point(1, 0, 0, 0);
		m+=r1;m+=r2;m+=r3;m+=r4;
		
		coefs = m * a;
	}
	return coefs;
}

matrix make_circle(double r, std::vector<double>& pc, int step){
	matrix m = matrix();
	double add = 1 / ((double) step);
	std::vector<double> *pi;
	std::vector<double> *pi2;
	for(double t = 0; t <= 1; t+=add){
		pi = new std::vector<double>(4, 0);
		(*pi)[0] = r * cos( 2 * PI * t) + pc[0];
		(*pi)[1] = r * sin( 2 * PI * t) + pc[1];
		(*pi)[2] = pc[2];
		(*pi)[3] = 1;
		m += (*pi);

		pi2 = new std::vector<double>(4, 0);
		(*pi2)[0] = r * cos( 2 * PI * (t+add)) + pc[0];
		(*pi2)[1] = r * sin( 2 * PI * (t+add)) + pc[1];
		(*pi2)[2] = pc[2];
		(*pi2)[3] = 1;
		m += (*pi2);
	}
	return m;
}

matrix make_curve(matrix& coefs, int step){
	matrix m = matrix();
	double add = 1 / ((double) step);
	std::vector<double> *pi;
	std::vector<double> *pi2;
	for(double t = 0; t <= 1; t+=add){
		pi = new std::vector<double>(4, 0);
		(*pi)[0] = (coefs[0][0] * pow(t,3)) + (coefs[0][1] * pow(t,2)) + (coefs[0][2] * t) + coefs[0][3];
		(*pi)[1] = (coefs[1][0] * pow(t,3)) + (coefs[1][1] * pow(t,2)) + (coefs[1][2] * t) + coefs[1][3];
		(*pi)[2] = 0;
		(*pi)[3] = 1;
		m += (*pi);

		pi2 = new std::vector<double>(4, 0);
		(*pi2)[0] = (coefs[0][0] * pow(t+add, 3)) + (coefs[0][1] * pow(t+add, 2)) + (coefs[0][2] * (t+add)) + coefs[0][3];
		(*pi2)[1] = (coefs[1][0] * pow(t+add, 3)) + (coefs[1][1] * pow(t+add, 2)) + (coefs[1][2] * (t+add)) + coefs[1][3];
		(*pi2)[2] = 0;
		(*pi2)[3] = 1;
		m += (*pi2);
	}
	return m;
}

matrix create_rotateY(double theta){

	double angle = convert(theta);
	matrix m = matrix(4, 4);
	m.ident();
	m[2][2] = cos(angle);
	m[0][2] = -sin(angle);
	m[2][0] = sin(angle);
	m[0][0] = cos(angle);
	return m;

}

matrix create_rotateX(double theta){
	
	double angle = convert(theta);
	matrix m = matrix(4, 4);
	m.ident();
	m[1][1] = cos(angle);
	m[2][1] = -sin(angle);
	m[1][2] = sin(angle);
	m[2][2] = cos(angle);
	return m;

}

matrix create_rotateZ(double theta){
	double angle = convert(theta);
	matrix m = matrix(4, 4);
	m.ident();
	m[0][0] = cos(angle);
	m[1][0] = -sin(angle);
	m[0][1] = sin(angle);
	m[1][1] = cos(angle);
	return m;

}
matrix create_scale(std::vector<double> a){
	matrix m = matrix(4, 4);
	m.ident();
	for (int i = 0; i < 3; i++){
		(m[i])[i] = a[i];
	}
	return m;
}
matrix create_translate(std::vector<double> p){
	matrix m = matrix(4, 4);
	m.ident();
	for (int i = 0; i < 3; i++){
		m[m.size()-1][i] = p[i];
	}
	return m;
}

matrix create_sphere(std::vector<double>& pc, double r, int step){
	matrix points = generate_sphere(pc, r, step);
	matrix sphere = polygon();
	for(int i = 0; i < points.size(); i++){
		if (i % (step+1) == 0 || i % (step+1) == (step)){
		}
		else{
			sphere += points[i];
			sphere += points[((i + step+1) % points.size()) - 1];
			sphere += points[(i + step+1) % points.size()];

			sphere += points[i];
			sphere += points[(i + step+1) % points.size()];
			sphere += points[i+1];
		}
	}
	matrix translate = create_translate(pc); 
	matrix ret = translate * sphere;
	return ret;
}

matrix generate_sphere(std::vector<double>& pc, double r, int step){
	matrix m = matrix();
	std::vector<double>* v;
	for(int i = 0; i < step; i++){
		double ti = (double)i / step;
		for(int j = 0; j <= step; j++){
			double tj = (double)j / step;
			v = new std::vector<double>(4, 0);
			(*v)[0] = r * cos(PI * tj);
			(*v)[1] = r * sin(PI * tj) * cos(2 * PI * ti);
			(*v)[2] = r * sin(PI * tj) * sin(2 * PI * ti);
			(*v)[3] = 1;
			m += (*v);
		}
	}
	return m;
}

matrix create_torus(std::vector<double>& pc, double r, double R, int step){
	matrix points = generate_torus(pc, r, R, step);
	matrix torus = polygon();
	for(int i = 0; i < points.size(); i++){
		if(i+step == points.size()){
			torus += points[i];
			torus += points[step];
			torus += points[(i + step) % points.size()];

			torus += points[i];
			torus += points[(i + step) % points.size()];
			torus += points[((i+1) % points.size())];
		}
		else{
			torus += points[i];
			torus += points[((i + step) % points.size()) - 1];
			torus += points[(i + step) % points.size()];

			torus += points[i];
			torus += points[(i + step) % points.size()];
			torus += points[((i+1) % points.size())];
		}
	}
	matrix translate = create_translate(pc); 
	matrix ret = translate * torus;
	return ret;
}
matrix generate_torus(std::vector<double>& pc, double r, double R, int step){
	matrix m = matrix();
	std::vector<double>* v;
	for(int i = 0; i < step; i++){
		double ti = (double)i/ step;
		for(int j = 0; j < step; j++){
			double tj = (double)j / (step-1);
			v = new std::vector<double>(4, 0);
			(*v)[0] = cos(2 * PI * ti) * (r * cos(2 * PI * tj) + R);
			(*v)[1] = r * sin(2 * PI * tj);
			(*v)[2] = -sin(2 * PI * ti) * (r * cos(2 * PI * tj) + R);
			(*v)[3] = 1;
			m += (*v);
		}
	}
	return m;
	
}

matrix create_box(std::vector<double>& pc, double w, double h, double d){
	matrix m = polygon();

	point f1 = point( pc[0], pc[1], pc[2] );
	point f2 = point( pc[0] + w, pc[1], pc[2]);
	point f3 = point( pc[0] + w, pc[1] - h, pc[2]);
	point f4 = point( pc[0], pc[1] - h, pc[2]);

	point b1 = point( pc[0], pc[1], pc[2] - d);
	point b2 = point( pc[0] + w, pc[1], pc[2] - d);
	point b3 = point( pc[0] + w, pc[1] - h, pc[2] - d);
	point b4 = point( pc[0], pc[1] - h, pc[2] - d);
	//front face
	m+=f1;
	m+=f3;
	m+=f2;

	m+=f1;
	m+=f4;
	m+=f3;

	//4 edges to back
	m+=f1;
	m+=f2;
	m+=b1;

	m+=f2;
	m+=b2;
	m+=b1;

	m+=f2;
	m+=f3;
	m+=b2;

	m+=f3;
	m+=b3;
	m+=b2;

	m+=f4;
	m+=b4;
	m+=b3;

	m+=f4;
	m+=b3;
	m+=f3;

	m+=f4;
	m+=b1;
	m+=b4;

	m+=f4;
	m+=f1;
	m+=b1;

	//back face
	m+=b2;
	m+=b4;
	m+=b1;

	m+=b2;
	m+=b3;
	m+=b4;

	return m;	

}
