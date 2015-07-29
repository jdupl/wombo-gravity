extern crate rustc_serialize;

use rustc_serialize::json;
use std::fs::File;
use std::path::Path;
use std::io::Read;

const G: f64 = -6.6738480e-11;

#[derive(Debug)]
pub struct Body {
    id: u32,
    name: String,
    m: f64,
    // position
    rx: f64,
    ry: f64,
    rz: f64,
    // velocity
    vx: f64,
    vy: f64,
    vz: f64,
    // acceleration
    ax: f64,
    ay: f64,
    az: f64
}

fn main() {
    let max_frames = 50;
    // How many computations in a frame
    let instant_count = 24 * 60 * 60;

    // Duration in second of an instant
    let instant_duration_sec = 1;

    let mut bodies = read_json("data/data.json");

    let mut current_frame = 0;

    while current_frame < max_frames {
        current_frame += 1;
        compute_frame(instant_count, instant_duration_sec, &mut bodies);
    }
}

fn compute_frame(instant_count: u32, instant_duration_sec: u32, bodies: &mut Vec<Body>) {
    let mut instant_counter = 0;

    while instant_counter < instant_count {
        instant_counter += 1;
        compute_instant(instant_duration_sec, bodies);
    }
}

fn compute_instant(instant_duration_sec: u32, bodies: &mut Vec<Body>) {
    let body_count = bodies.len();

    let mut i = 0;
    let mut j = 0;

    while i < body_count {
        j = i + i;
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

        // body.update_pos();
        // body.ax = 0;
        // body.ay = 0;
        // body.az = 0;
    }
}


fn accel(bodies: &mut Vec<Body>, b1_index: usize, b2_index: usize) {
    let mut tmp_x = bodies[b1_index].rx - bodies[b2_index].rx;
    let mut tmp_y = bodies[b1_index].ry - bodies[b2_index].ry;
    let mut tmp_z = bodies[b1_index].rz - bodies[b2_index].rz;

    let mut tmp = tmp_x * tmp_x + tmp_y * tmp_y + tmp_z * tmp_z;

    tmp = ((bodies[b1_index].m) * (bodies[b2_index].m)) / (tmp.sqrt());

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
        let mut body = Body {
            id: count, name: body_data.name.clone(), m: body_data.m, rx: body_data.rx,
            ry: body_data.ry, rz: body_data.rz, vx: body_data.vx, vy: body_data.vy,
            vz: body_data.vz, ax: 0.0, ay: 0.0, az: 0.0
        };

        count += 1;
        bodies.push(body);
    }

    return bodies;
}
