extern crate rustc_serialize;

use rustc_serialize::json;
use std::fs::File;
use std::path::Path;
use std::io::Read;

#[derive(Debug)]
pub struct Body {
    id: u32,
    name: String,
    rx: f64,
    ry: f64,
    rz: f64,
    vx: f64,
    vy: f64,
    vz: f64,
    ax: f64,
    ay: f64,
    az: f64
}

fn main() {
    let max_frames = 50;
    // How many computations in a frame
    let instant_count = 60 * 60 * 24;

    // Duration in second of an instant
    let instant_duration_sec = 1;

    let bodies = read_json("data/data.json");

    for body in bodies.iter() {
        println!("{:?}", body);
    }
}

pub fn read_json(name: &str) -> Vec<Body> {
    #[derive(RustcDecodable, RustcEncodable, Debug)]
    struct BodyData {
        name: String,
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
            id: count, name: body_data.name.clone(), rx: body_data.rx, ry: body_data.ry,
            rz: body_data.rz, vx: body_data.vx, vy: body_data.vy, vz: body_data.vz,
            ax: 0.0, ay: 0.0, az: 0.0
        };
        
        count += 1;
        bodies.push(body);
    }

    return bodies;
}
