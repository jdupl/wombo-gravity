package wombo.gravity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class Main {

	public static final double G = (-6.6738480 * Math.pow(10, -11));

	public final static long computeResSec = 1;
	public final static long resPow = computeResSec * computeResSec;

	public final static long frameResSec = 24 * 60 * 60;

	public static void main(String[] args) {
		long maxFrames = 365;

		ArrayList<Particle> particles = getParticles("data/data.txt");
		System.out.println("Loaded " + particles.size() + " particles");

		long ms = System.currentTimeMillis();
		crunch(maxFrames, particles);
		System.out.println("Simulation took " + (System.currentTimeMillis() - ms) + " ms");
	}

	public static void crunch(long maxFrames, ArrayList<Particle> particles) {
		long frameCount = 0;

		while (frameCount++ < maxFrames) {
			long ms = System.currentTimeMillis();
			computeFrame(particles);
			System.out.println("Frame took " + (System.currentTimeMillis() - ms) + " ms");
			System.out.println(particles.get(3));
		}
	}

	public static void computeFrame(ArrayList<Particle> particles) {
		long currentFrameIt = 0;

		while (currentFrameIt++ < frameResSec) {
			for (Particle particle : particles) {
				particle.resetAccel();

				for (Particle otherParticle : particles) {
					if (particle != otherParticle) { // compare by address in memory
						// compute accel
						double distx = particle.coordinates[0] - otherParticle.coordinates[0];
						double disty = particle.coordinates[1] - otherParticle.coordinates[1];
						double distz = particle.coordinates[2] - otherParticle.coordinates[2];

						double squares = distx * distx + disty * disty + distz * distz;

						// using Math.pow makes this 10 times slower (WHY?)
						double norm = otherParticle.mass / Math.pow(squares, 1.5);

						particle.accel[0] += norm * distx;
						particle.accel[1] += norm * disty;
						particle.accel[2] += norm * distz;
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
