# Perlin-Noise-2d


## PerlinNoise: Generating Perlin Noise in 2D

The PerlinNoise class provides a simple way to generate Perlin noise in 2D. Perlin noise is a type of procedural noise that is often used in games and simulations to create realistic natural-looking patterns.

To generate Perlin noise, you can create an instance of the PerlinNoise class and then call the `getNoise` method, passing in the x and y coordinates of the point you want to sample:

```java

public static void main(String[] args) {
    PerlinNoise noise = new PerlinNoise(12345);
    double x = 0.5;
    double y = 0.5;
    double result = noise.getNoise(x, y);
    System.out.println(result);
}

```
