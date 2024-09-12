#ifndef MINECRAFTCLONE_PERLINNOISE_H
#define MINECRAFTCLONE_PERLINNOISE_H


#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>

class PerlinNoise {
public:
    // Constructor that seeds the noise with a default seed
    PerlinNoise();

    // Constructor that allows custom seeding
    explicit PerlinNoise(unsigned int seed);

    // Generate Perlin noise at coordinates x, y, z
    double noise(double x, double y, double z) const;

private:
    std::vector<int> p;  // Permutation vector

    double fade(double t) const;
    double lerp(double t, double a, double b) const;
    double grad(int hash, double x, double y, double z) const;
};


#endif //MINECRAFTCLONE_PERLINNOISE_H
