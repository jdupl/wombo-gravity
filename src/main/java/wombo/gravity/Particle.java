package wombo.gravity;

import java.text.DecimalFormat;
import java.text.NumberFormat;

public class Particle {

	/**
	 * Mass in Kg
	 */
	public double mass;

	/**
	 * Accelerations in m/s x,y,z
	 */
	public double[] accel;

	/**
	 * Velocities in m/s x,y,z
	 */
	public double[] velocity;
	/**
	 * Coordinates in m x,y,z
	 */
	public double[] coordinates;

	public String name;

	public Particle(String line) {
		String[] data = line.split(" ");
		this.accel = new double[3];
		this.velocity = new double[3];
		this.coordinates = new double[3];

		if (data.length == 8) {
			this.name = data[0];
			this.mass = Double.parseDouble(data[1]);
			this.coordinates[0] = Double.parseDouble(data[2]);
			this.coordinates[1] = Double.parseDouble(data[3]);
			this.coordinates[2] = Double.parseDouble(data[4]);
			this.velocity[0] = Double.parseDouble(data[5]);
			this.velocity[1] = Double.parseDouble(data[6]);
			this.velocity[2] = Double.parseDouble(data[7]);
		}
	}

	public void resetAccel() {
		this.accel[0] = 0;
		this.accel[1] = 0;
		this.accel[2] = 0;
	}

	public void updateSpeed() {
		this.velocity[0] += accel[0] * Main.computeResSec;
		this.velocity[1] += accel[1] * Main.computeResSec;
		this.velocity[2] += accel[2] * Main.computeResSec;
	}

	public void updateCoordinates() {
		this.coordinates[0] += 0.5 * this.accel[0] * Main.resPow + velocity[0] * Main.computeResSec;
		this.coordinates[1] += 0.5 * this.accel[1] * Main.resPow + velocity[1] * Main.computeResSec;
		this.coordinates[2] += 0.5 * this.accel[2] * Main.resPow + velocity[2] * Main.computeResSec;

	}

	public double distance(Particle other) {
		return Math.sqrt(Math.pow(this.coordinates[0] - other.coordinates[0], 2)
				+ Math.pow(this.coordinates[1] - other.coordinates[1], 2)
				+ Math.pow(this.coordinates[2] - other.coordinates[2], 2));
	}

	public String toCsv() {
		return String.format("%d;%d;%d%n", coordinates[0], coordinates[1], coordinates[2]);
	}

	@Override
	public String toString() {
		NumberFormat formatter = new DecimalFormat("0.##E0");
		return String.format("[Particle]: %s Kg%nCoordinates: x %s, y %s, z %s%nVelocity: x %s, y %s, z %s%n",
				formatter.format(mass), formatter.format(coordinates[0]), formatter.format(coordinates[1]),
				formatter.format(coordinates[2]), formatter.format(velocity[0]), formatter.format(velocity[1]),
				formatter.format(velocity[2]));
	}
}
