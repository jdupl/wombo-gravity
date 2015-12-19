extern crate rustc_serialize;
extern crate bincode;
extern crate num_cpus;

mod body;
mod writer;
mod frame_buffer;

use rustc_serialize::json;
use std::fs::File;
use std::path::Path;
use std::io::Read;

use body::Body;
use body::Frame;
use writer::BinaryWriter;
use frame_buffer::FrameBuffer;

const G: f64 = -6.6738480e-11;

fn main() {
    let max_frames = 365;
    // How many computations in a frame
    let instant_count = 24 * 60 * 60;

    // Duration in second of an instant
    let instant_duration_sec: f64 = 1.0;

    let mut bodies = read_json("data/data.json");

    let mut current_frame = 0;

    let writer = BinaryWriter;

    let mut buffer = FrameBuffer::new(writer);
    divide_problem_for_cores(11, 2);

    while current_frame < max_frames {
        compute_frame(instant_count, instant_duration_sec, &mut bodies);

        let frame = Frame {
            id: current_frame,
            bodies: bodies.clone(),
        };
        buffer.add(frame);
        current_frame += 1;
    }
    buffer.close();

    for body in bodies.iter() {
        println!("{:?}", body);
    }
}

fn get_step_count_for_parrallel_perms(body_count: u32, cpu_count: u32) -> u32 {
    // TODO FIXME
    let perms_count = (body_count * body_count) / 2;

    let perms_per_cpu_step = (body_count as f64 / cpu_count as f64).ceil() as u32;
    let perms_per_step = (perms_per_cpu_step - 1) * cpu_count;

    return (perms_count as f64 / perms_per_step as f64).ceil() as u32;
}

fn divide_problem_for_cores(body_count: u32, cpu_count: u32) {
    let step_count = get_step_count_for_parrallel_perms(body_count, cpu_count);
    for x in 0..step_count {
        divide_step(x, body_count, cpu_count);
    }
}

fn divide_step(step: u32, body_count: u32, cpu_count: u32) -> Vec<Vec<u32>> {
    let bodies_per_cpu = (body_count as f64 / cpu_count as f64).ceil() as usize;

    let mut core_jobs: Vec<Vec<u32>> = vec![vec![]; cpu_count as usize];

    for x in 0..core_jobs.len() {
        core_jobs[x] = vec![0; bodies_per_cpu];

        for y in 0..bodies_per_cpu {
            core_jobs[x][y] = (y as u32 + (x * bodies_per_cpu) as u32 + step) % (body_count + 1) as u32;
        }
    }
    return core_jobs;
}

fn compute_frame(instant_count: u32, instant_duration_sec: f64, bodies: &mut Vec<Body>) {
    let mut instant_counter = 0;

    while instant_counter < instant_count {
        instant_counter += 1;
        compute_instant(instant_duration_sec, bodies);
    }
}

fn compute_instant(instant_duration_sec: f64, bodies: &mut Vec<Body>) {
    let body_count = bodies.len();

    let mut i = 0;
    let mut j;

    while i < body_count {
        j = i + 1;
        while j < body_count {
            accel(bodies, i, j);
            j += 1;
        }
        i += 1;
    }

    for mut body in bodies.iter_mut() {
        body.ax *= G;
        body.ay *= G;
        body.az *= G;

        body.update_pos(instant_duration_sec);
    }
}


fn accel(bodies: &mut Vec<Body>, b1_index: usize, b2_index: usize) {
    let mut tmp_x = bodies[b1_index].rx - bodies[b2_index].rx;
    let mut tmp_y = bodies[b1_index].ry - bodies[b2_index].ry;
    let mut tmp_z = bodies[b1_index].rz - bodies[b2_index].rz;

    let mut tmp = (tmp_x * tmp_x) + (tmp_y * tmp_y) + (tmp_z * tmp_z);
    tmp = ((bodies[b1_index].m) * (bodies[b2_index].m)) / (tmp * tmp.sqrt());

    tmp_x *= tmp;
    tmp_y *= tmp;
    tmp_z *= tmp;

    {
        let ref mut b1 = &mut bodies[b1_index];
        b1.ax += tmp_x;
        b1.ay += tmp_y;
        b1.az += tmp_z;
    }

    {
        let ref mut b2 = &mut bodies[b2_index];
        b2.ax -= tmp_x;
        b2.ay -= tmp_y;
        b2.az -= tmp_z;
    }
}

pub fn read_json(name: &str) -> Vec<Body> {
    #[derive(RustcDecodable, RustcEncodable, Debug)]
    struct BodyData {
        name: String,
        m: f64,
        rx: f64,
        ry: f64,
        rz: f64,
        vx: f64,
        vy: f64,
        vz: f64,
    }

    #[derive(RustcDecodable, RustcEncodable, Debug)]
    struct Root {
        bodies: Vec<BodyData>
    }

    let path = Path::new(name);

    let mut file = match File::open(&path) {
        Err(e) => panic!("{} {}", path.display(), e),
        Ok(file) => file,
    };

    let mut data = String::new();
    file.read_to_string(&mut data).unwrap();

    let decoded: Root = json::decode(&data).unwrap();

    let mut bodies = Vec::new();
    let mut count  = 0;

    for body_data in decoded.bodies.iter() {
        let body = Body {
            id: count, name: body_data.name.clone(), m: body_data.m, rx: body_data.rx,
            ry: body_data.ry, rz: body_data.rz, vx: body_data.vx, vy: body_data.vy,
            vz: body_data.vz, ax: 0.0, ay: 0.0, az: 0.0
        };

        count += 1;
        bodies.push(body);
    }

    return bodies;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_get_step_count_for_parrallel_perms() {
        assert_eq!(super::get_step_count_for_parrallel_perms(12, 4), 4);
    }
}
