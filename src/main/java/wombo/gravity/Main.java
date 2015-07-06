package wombo.gravity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class Main {

	public static long resolution = 1;

	public static long resPow = resolution * resolution;

	public static void main(String[] args) {
		try (FileReader fr = new FileReader(new File("data.txt")); BufferedReader reader = new BufferedReader(fr)) {
			String line = null;
			
			while ((line = reader.readLine()) != null) {
				Particle p = new Particle(line);
				System.out.println(p.name);
			}	
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
