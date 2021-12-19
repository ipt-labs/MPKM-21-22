#pragma once

#include <math.h>
#include <map>

typedef unsigned int uint;
typedef unsigned short ushort;

using namespace std;

struct TestResult  // Результата теста
{
	double x2;  // X^2
	bool right;  // + / -
};

const uint r = 10u;

// Тест равномерности
TestResult testEquiprobability(byte* sequence, const uint m, uint l, double x21a)
{
	const auto max = 255u;
	auto nj = m / (double)max;
	auto x2 = 0.0;
	uint v[max + 1];
	memset(v, 0, (max + 1) * sizeof(uint));

	for (uint i = 0; i < m; i++)
		v[sequence[i]]++;
	for (int j = 0; j <= max; j++)
		x2 += pow(v[j] - nj, 2.0) / nj;

	return { x2, x2 <= x21a };
}

// Тест независимости
TestResult testIndependence(byte* sequence, uint m, uint l, double x21a)
{
	auto n = m / 2u;
	const auto max = 255u;
	auto x2 = 0.0;
	uint vji[max + 1][max + 1];
	uint v[max + 1];
	uint a[max + 1];

	memset(vji, 0, (max + 1) * (max + 1) * sizeof(uint));
	memset(v, 0, (max + 1) * sizeof(uint));
	memset(a, 0, (max + 1) * sizeof(uint));

	for (uint i = 1; i < n; i++)
	{
		vji[sequence[2 * i - 1]][sequence[2 * i]]++;
		v[sequence[2 * i - 1]]++;
		a[sequence[2 * i]]++;
	}
	for (int i = 0; i <= max; i++)
		for (int j = 0; j <= max; j++)
			if (v[i] * a[j] > 0.0)
				x2 += pow(vji[i][j], 2.0) / (v[i] * a[j]);
	x2 = n * (x2 - 1.0);

	return { x2, x2 <= x21a };
}

// Тест однородности
TestResult testUniformity(byte* sequence, uint m, uint l, double x21a)
{
	auto mi = m / r;
	const auto max = 255u;
	auto x2 = 0.0;
	uint vji[r][max + 1];
	uint v[max];

	memset(vji, 0, r * (max + 1) * sizeof(uint));
	memset(v, 0, (max + 1) * sizeof(uint));

	for (auto j = 0u; j < r; j++)
	{
		for (auto i = 0u; i < mi; i++)
		{
			vji[j][sequence[j * mi + i]]++;
			v[sequence[j * mi + i]]++;
		}
	}
	for (auto j = 0u; j <= max; j++)
		for (auto i = 0u; i < r; i++)
			if (((double)v[j] * mi) > 0)
				x2 += pow(vji[i][j], 2.0) / ((double)v[j] * mi);
	x2 = m * (x2 - 1.0);

	return { x2, x2 <= x21a };
}