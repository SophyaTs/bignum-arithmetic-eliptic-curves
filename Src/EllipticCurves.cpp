#include <EllipticCurves.hpp>

namespace lab {
	EllipticCurve::EllipticCurve(Field* f, const BigNum& a, const BigNum& b): _f(f),_a(a),_b(b){}

	bool operator==(const EllipticCurve& left, const EllipticCurve& right) {
		return (*left._f == *right._f) && (left._a == right._a) && (left._b == right._b);
	}

	bool operator!=(const EllipticCurve& left, const EllipticCurve& right) {
		return !(left == right);
	}

	Point EllipticCurve::inverted(const Point& p) {
		return { p.x,_f->generator - p.y };
	}

	Point EllipticCurve::add(const Point& first, const Point& second) {
		if ((first.x == second.x && first.y != second.y)
			|| (first == second && first.y == 0_bn))
		{
			return { 10000000000_bn,1000000000000_bn }; //neutral point (infinity)
		} else {
			BigNum tmp1;
			BigNum tmp2;
			BigNum m;
			if (first.x != second.x) {
				tmp1 = second.y >= first.y ? 
					subtract(second.y, first.y, _f->generator) 
					: lab::add(subtract(_f->generator, second.y, _f->generator), first.y, _f->generator); //y2-y1
				
				tmp2 = second.x >= first.x ? 
					second.x - first.x, _f->generator 
					: lab::add(subtract(_f->generator, second.x, _f->generator), first.x, _f->generator); //x2-x1
			}
			else {
				tmp1 = multiply(first.x, first.x, _f->generator);
				tmp1 = multiply(3_bn, tmp1, _f->generator);
				tmp1 = lab::add(tmp1, _a, _f->generator);
				tmp2 = multiply(2_bn, first.y, _f->generator);
			}
			m = multiply(tmp1,lab::inverted(tmp2,_f->generator, BigNum::InversionPolicy::Euclid),_f->generator);

			tmp1 = multiply(m, m, _f->generator);
			tmp2 = lab::add(first.x, second.x, _f->generator);
			
			tmp1 = tmp1 >= tmp2 ?
				subtract(tmp1, tmp2, _f->generator)
				: lab::add(subtract(_f->generator, tmp1, _f->generator), tmp2, _f->generator); //= x3

			tmp2 = first.x >= tmp1 ? 
				subtract(first.x, tmp2, _f->generator) 
				: lab::add(subtract(_f->generator, first.x, _f->generator), tmp1, _f->generator); //= x1-x3			
			
			tmp2 = multiply(m, tmp2, _f->generator); //= m*(x1-x3)
			
			tmp2 = tmp2 >= first.y ? 
				subtract(tmp2, first.y, _f->generator) 
				: lab::add(subtract(_f->generator, tmp2, _f->generator), first.y, _f->generator); //= y3

			return{ tmp1,tmp2 }; //{x3,y3} - answer
		}
	}
}