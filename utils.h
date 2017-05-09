#pragma once

long double eps = 0.00001;

bool solveQuadratic(const long double &a, const long double &b, const long double &c, long double &x0, long double &x1)
{
    long double discr = b * b - 4 * a * c;
    if (discr < 0 - eps) return false;
    else if (-eps < discr && discr < eps) x0 = x1 = -0.5 * b / a;
	else {
        x0 = (-b + sqrt(discr)) / (2 * a);
        x1 = (-b - sqrt(discr)) / (2 * a);
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}
