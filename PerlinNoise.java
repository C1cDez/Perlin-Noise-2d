package com.cicdez.perlinnnoise1test;

import java.util.Random;

//Perlin Noise class
public class PerlinNoise {
    public final Random random;   //Main Random

    private final byte[] permutation;   //Permutation for Randomizing in Generation

    //Constructors
    public PerlinNoise(Random random) {
        this.random = random;
        this.permutation = new byte[1 << 10];    //1024
        this.random.nextBytes(this.permutation);   //Randomize permutation Bytes
    }
    public PerlinNoise(long seed) {
        this(new Random(seed));
    }

    //Generate Noise on definite position
    public double getNoise(double x, double y) {

        //Rounded
        int left = (int) x;
        int top = (int) y;

        //Local Point Poses
        double localX = x - left;
        double localY = y - top;

        //Random Vectors from rounded points
        Vector2 topLeftGradient = generatePseudoRandomVector(left, top);
        Vector2 topRightGradient = generatePseudoRandomVector(left + 1, top);
        Vector2 bottomLeftGradient = generatePseudoRandomVector(left, top + 1);
        Vector2 bottomRightGradient = generatePseudoRandomVector(left + 1, top + 1);

        //Distances to Local points
        Vector2 distanceToTopLeft = new Vector2(localX, localY);
        Vector2 distanceToTopRight = new Vector2(localX - 1, localY);
        Vector2 distanceToBottomLeft = new Vector2(localX, localY - 1);
        Vector2 distanceToBottomRight = new Vector2(localX - 1, localY - 1);

        //Scalar Product
        double topLeftProd = Vector2.dot(topLeftGradient, distanceToTopLeft);
        double topRightProd = Vector2.dot(topRightGradient, distanceToTopRight);
        double bottomLeftProd = Vector2.dot(bottomLeftGradient, distanceToBottomLeft);
        double bottomRightProd = Vector2.dot(bottomRightGradient, distanceToBottomRight);

        //Linear Interpolation
        double topInter = lerp(topLeftProd, topRightProd, curve(localX));
        double bottomInter = lerp(bottomLeftProd, bottomRightProd, curve(localX));
        
        return lerp(topInter, bottomInter, curve(localY));
    }

    //Get some Perlin Noises put on each other
    public double getSimplexNoise(double x, double y, int octaves, double persistence) {
        double altitude = 1;     //Altitude of Noise
        double res = 0;    //Result
        double max = 0;    //Max

        //Use all Octaves
        while (octaves-- > 0) {
            max += (altitude * persistence);
            res += getNoise(x, y) * altitude;     //Add noise to Result
            altitude *= persistence;   //decreasing altitude
            x /= persistence;    //Increasing x
            y /= persistence;    //Increasing y
        }

        return res / max;
    }

    //Linear interpolation
    private double lerp(double a, double b, double t) {
        return a + (b - a) * t;
    }

    //Curve function 'https://www.desmos.com/calculator/8avcjiaggb?lang=en'
    private double curve(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    //Generating Pseudo-Random Vector depending on x, y
    private Vector2 generatePseudoRandomVector(int x, int y) {
        int h = ((((x * 56747323) >> 7) | 652363673 * (y << 4)) -
                12245232 * (x ^ y) & (102383002 | x >> 5)) & (permutation.length - 1);
        //Some crazy operations with integers

        int p = permutation[h] & 3;   //Compress number in diapason 0..3
        switch (p) {
            case 0: return new Vector2(1, 0);
            case 1: return new Vector2(-1, 0);
            case 2: return new Vector2(0, 1);
            case 3: return new Vector2(0, -1);
            default: throw new IllegalStateException("Wtf with the Universe!?");
        }
    }


    //Vector 2d
    public static class Vector2 {
        public final double x, y;
        public Vector2(double x, double y) {
            this.x = x;
            this.y = y;
        }

        //Scalar Product
        public static double dot(Vector2 a, Vector2 b) {
            return a.x * b.x + a.y * b.y;
        }
    }
}
