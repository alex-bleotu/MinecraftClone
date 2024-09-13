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
    explicit PerlinNoise(unsigned int seed);

    // Noise function
    [[nodiscard]] double noise(double x, double y, double z) const;

private:
    std::vector<int> p;  // Permutation vector

    [[nodiscard]] double fade(double t) const;
    [[nodiscard]] double lerp(double t, double a, double b) const;
    [[nodiscard]] double grad(int hash, double x, double y, double z) const;

    void reseed(unsigned int newSeed);
};


#endif
