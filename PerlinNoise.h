#pragma once

struct Vec2d;

class PerlinNoise
{
public:
	PerlinNoise(unsigned int pSeed);
	PerlinNoise();

	double at(double x, double y);

	unsigned int getSeed() const;
	void updateSeed(unsigned int pSeed);

private:
	unsigned int mSeed;
	unsigned char mPermutations[1 << 8];

	Vec2d genPRGradient(int x, int y) const;
};

class FractalPerlinNoise
{
public:
	FractalPerlinNoise(unsigned int pSeed, unsigned int pOctaves, double pLacunarity = 2.0, double pPersistence = 0.5);

	double at(double x, double y);

private:
	PerlinNoise mBase;
	unsigned int mOctaves;
	double mLacunarity;   // How much frequency increases with each octave
	double mPersistence;   // How much each successive octave influences the result
};
