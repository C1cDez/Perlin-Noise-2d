package src.examples;

import src.main.com.cicdez.perlin.PerlinNoise;

public class Main {
    public static void main(String[] args) {
        PerlinNoise noise = new PerlinNoise(12345);
        double x = 0.5212;  // Some rand numbers
        double y = 0.59;
        double result = noise.getNoise(x, y);
        System.out.println(result);
    }
}
