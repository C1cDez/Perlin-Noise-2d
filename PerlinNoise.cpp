#include "PerlinNoise.h"

#include <random>
#include <cmath>


static double cint(double a, double b, double t)
{
	return t * t * (b - a) * (3 - 2 * t) + a;
}

struct Vec2d
{
	double x, y;
};
static double dot(const Vec2d& a, const Vec2d& b)
{
	return a.x * b.x + a.y * b.y;
}

static constexpr unsigned int UNSIGNED_INT_SIZE = sizeof(unsigned int);
static constexpr unsigned int UNSIGNED_INT_SIZE_2 = UNSIGNED_INT_SIZE / 2;
static unsigned int hash(int x, int y)
{
	unsigned int a = static_cast<unsigned int>(x), b = static_cast<unsigned int>(y);
	a *= 3284157443;
	b ^= a << UNSIGNED_INT_SIZE_2 | a >> UNSIGNED_INT_SIZE - UNSIGNED_INT_SIZE_2;
	b *= 1911520717;
	a ^= b << UNSIGNED_INT_SIZE_2 | b >> UNSIGNED_INT_SIZE - UNSIGNED_INT_SIZE_2;
	a *= 2048419325;
	return a;
}


PerlinNoise::PerlinNoise(unsigned int pSeed)
{
	updateSeed(pSeed);
}
PerlinNoise::PerlinNoise()
{
	mSeed = 0;
	memset(mPermutations, 0, 1 << 8);
}
unsigned int PerlinNoise::getSeed() const
{
	return mSeed;
}
void PerlinNoise::updateSeed(unsigned int pSeed)
{
	mSeed = pSeed;
	std::mt19937 rand{ mSeed };
	std::uniform_int_distribution<int> bytes{ 0, 0xff };
	for (int i = 0; i < (1 << 8); i++) mPermutations[i] = bytes(rand) & 0xff;
}

Vec2d PerlinNoise::genPRGradient(int x, int y) const
{
	unsigned int h = hash(x, y) & 0xff;
	int v = mPermutations[mPermutations[h]];
	double t = (double)v / 128 * 3.14159265358;
	return { cos(t), sin(t) };
}

double PerlinNoise::at(double x, double y)
{
	int x0 = (int)floor(x), y0 = (int)floor(y);
	double lx = x - x0, ly = y - y0;

	Vec2d tlv = genPRGradient(x0, y0);
	Vec2d trv = genPRGradient(x0 + 1, y0);
	Vec2d blv = genPRGradient(x0, y0 + 1);
	Vec2d brv = genPRGradient(x0 + 1, y0 + 1);

	Vec2d tld{ lx, ly };
	Vec2d trd{ lx - 1, ly };
	Vec2d bld{ lx, ly - 1 };
	Vec2d brd{ lx - 1, ly - 1 };

	double tlg = dot(tlv, tld);
	double trg = dot(trv, trd);
	double blg = dot(blv, bld);
	double brg = dot(brv, brd);

	double ti = cint(tlg, trg, lx);
	double bi = cint(blg, brg, lx);

	double v = cint(ti, bi, ly);

	return v;
}


FractalPerlinNoise::FractalPerlinNoise(unsigned int pSeed, unsigned int pOctaves, double pLacunarity, double pPersistence)
{
	mBase.updateSeed(pSeed);
	mOctaves = pOctaves;
	mLacunarity = pLacunarity;
	mPersistence = pPersistence;
}
double FractalPerlinNoise::at(double x, double y)
{
	double value = 0.0;
	double scale = 1.0;
	for (int i = 0; i < (int)mOctaves; i++)
	{
		value += mBase.at(x, y) * scale;
		x *= mLacunarity;
		y *= mLacunarity;
		scale *= mPersistence;
	}
	return value;
}
