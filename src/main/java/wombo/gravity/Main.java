package wombo.gravity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

public class Main {

	public final static long resolution = 1;

	public final static long resPow = resolution * resolution;

	public static final double G = (-6.6738480 * Math.pow(10, -11));

	public static void main(String[] args) {
		ArrayList<Particle> particles = getParticles("data.txt");
		int maxIt = 100;

	}

	public void crunch(int maxIt, ArrayList<Particle> particles) {
		long currentTime = 0;
		while (currentTime / resolution < maxIt) {

			for (Particle particle : particles) {
				particle.resetAccel();

				for (Particle otherParticle : particles) {
					if (particle != otherParticle) {
						// calculer accel
					}
				}
				particle.accel[0] *= G;
				particle.accel[1] *= G;
				particle.accel[2] *= G;
			}

			for (Particle particle : particles) {
				particle.updateCoordinates();
				particle.updateSpeed();
			}
		}
	}

	public static ArrayList<Particle> getParticles(String filename) {
		ArrayList<Particle> particles = new ArrayList<>();

		try (FileReader fr = new FileReader(new File(filename)); BufferedReader reader = new BufferedReader(fr)) {
			String line = null;

			while ((line = reader.readLine()) != null) {
				particles.add(new Particle(line));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return particles;
	}

}
