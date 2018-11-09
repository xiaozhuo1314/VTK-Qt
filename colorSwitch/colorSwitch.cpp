#include <iostream>
#include <math.h>
#include <map>

using namespace std;
double min(double o, double p, double q);
double max(double o, double p, double q);
void rgbToHs(int c, double o, double p, double q);   //rgb to hsv
void hsToRgb(int c, double o, double p, double q);   //hsv or hsl to rgb
const double eps = 1e-6;
map<char, double> result;

int main() {
	int ch;
	double r, g, b, h, s, v;
	//vector<float> v;
	std::cout << "please select your choice 1:RGB to HSV; 2:RGB to HSL; 3:HSV to RGB; 4:HSL to RGB!" << std::endl;
	std::cin >> ch;
	std::cin.clear();
	if (ch == 1 || ch == 2) {
		std::cout << "please insert color float number,notice:Value must be in range { R,G,B in [0,1]}" << std::endl;
		std::cin >> r >> g >> b;
		rgbToHs(ch, r, g, b);
	}
	
	if (ch == 3 || ch == 4) {  //hsl or hsv to rgb
		std::cout << "please insert color float number,notice:Value must be in range h in [0,360], s and l or v in [0,1]" << std::endl;
		std::cin >> h >> s >> v;
		hsToRgb(ch, h, s, v);
	}
	for (map<char, double>::iterator it = result.begin(); it != result.end(); it++) {
		std::cout << it->first << " " << it->second << std::endl;
	}
	result.clear();
	system("pause");
	return 0;
}

double min(double o, double p, double q) {
	if (p >= o && q >= o) {
		return o;
	}
	else if (o >= p && q >= p)
	{
		return p;
	}
	else {
		return q;
	}
}

double max(double o, double p, double q) {
	if (o >= p && o >= q) {
		return o;
	}
	else if (p >= o && p >= q)
	{
		return p;
	}
	else {
		return q;
	}
}

void rgbToHs(int c, double o, double p, double q) {
	double mi, ma, h, s, v, l;
	mi = min(o, p, q);
	ma = max(o, p, q);
	if (abs(ma - mi) < eps) { //get h in hs
		h = 0.0;
	}
	else if (abs(o - ma) < eps)
	{
		h = 60.0 * (0.0 + (p - q) / (ma - mi));
	}
	else if (abs(p - ma) < eps) {
		h = 60.0 * (2.0 + (q - o) / (ma - mi));
	}
	else {
		h = 60.0 * (4.0 + (o - p) / (ma - mi));
	}
	if (h < 0.0) {
		h += 360.0;
	}

	if (c == 1) {
		if (ma < 0.0) { //get s in hsv
			s = 0.0;
		}else{
			s = (ma - mi) / ma;
		}
		v = ma; //get v in hsv
		result.insert(pair<char, double>('h', h));
		result.insert(pair<char, double>('s', s));
		result.insert(pair<char, double>('v', v));
	}else {
		if (abs(ma - 0.0) < eps || abs(mi - 1.0) < eps) {  //get s in hsl
			s = 0.0;
		}else{
			if (ma + mi >= 1.0) {
				s = (ma - mi) / (2.0 - mi - ma);
			}else{
				s = (ma - mi) / (mi + ma);
			}
		}
		l = (ma + mi) / 2.0; //get l in hsl
		result.insert(pair<char, double>('h', h));
		result.insert(pair<char, double>('s', s));
		result.insert(pair<char, double>('l', l));
	}
}

void hsToRgb(int c, double o, double p, double q) {
	double tmp, m, hh, x, r, g, b;
	if (c == 4) {
		if (2.0 * q > 1) {
			tmp = (2.0 - 2.0 * q)*p;
		}
		else {
			tmp = (2.0 * q)*p;
		}
		m = q - tmp / 2.0;
	}
	else {
		tmp = q * p;
		m = q - tmp;
	}
	hh = o / 60.0;

	if (hh >= 0 && hh <= 1) {
		x = tmp * hh;
		r = tmp + m;
		g = x + m;
		b = m;
	}
	else if (hh >= 1 && hh <= 2)
	{
		x = tmp * (2 - hh);
		r = x + m;
		g = tmp + m;
		b = m;
	}
	else if (hh >= 2 && hh <= 3)
	{
		x = tmp * (hh - 2);
		r = m;
		g = tmp + m;
		b = x + m;
	}
	else if (hh >= 3.0 && hh <= 4.0)
	{

		x = tmp * (4 - hh);
		r = m;
		b = tmp + m;
		g = x + m;
	}
	else if (hh >= 4 && hh <= 5)
	{
		x = tmp * (hh - 4);
		g = m;
		b = tmp + m;
		r = x + m;
	}
	else
	{
		x = tmp * (6 - hh);
		g = m;
		r = tmp + m;
		b = x + m;
	}

	result.insert(pair<char, double>('r', r));
	result.insert(pair<char, double>('g', g));
	result.insert(pair<char, double>('b', b));
}

