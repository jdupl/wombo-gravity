extern crate bincode;

use std::fs::OpenOptions;
use std::error::Error;
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;
use std::collections::VecDeque;
use bincode::SizeLimit;

use body::Frame;

pub trait Writer {

    fn flush(&self, frames: &mut VecDeque<Frame>) {
        while frames.len() > 0 {
            self.write(&frames.pop_front().unwrap());
        }
    }

    fn write(&self, frame: &Frame);
}

pub struct BinaryWriter;

impl Writer for BinaryWriter {

    fn write(&self, frame: &Frame) {
        println!("writing frame {}", frame.id);

        #[derive(RustcEncodable)]
        struct FrameHeader {
            frame_number: u32,
            body_count: u32,
        };

        #[derive(RustcEncodable)]
        struct BodyData {
            id: u32,
            rx: f64,
            ry: f64,
            rz: f64,
        };

        let mut file = match OpenOptions::new().write(true).append(true).create(true).open("foo.bin") {
            Err(why) => panic!("Couldn't open {}: {}", "foo.bin", Error::description(&why)),
            Ok(file) => file,
        };

        let header = FrameHeader {
            frame_number: 1,
            // frame_number: frame.id as u64,
            body_count: frame.bodies.len() as u32,
        };
        println!("{:?}", bincode::encode(&header, SizeLimit::Infinite).unwrap());
        match file.write_all(&bincode::encode(&header, SizeLimit::Infinite).unwrap()) {
            Err(why) => panic!("Error writing to file: {}", Error::description(&why)),
            Ok(ok) => ok,
        };

        for body in frame.bodies.iter() {
            let data = BodyData {
                id: body.id,
                rx: body.rx,
                ry: body.ry,
                rz: body.rz,
            };

            match file.write_all(&bincode::encode(&data, SizeLimit::Infinite).unwrap()) {
                Err(why) => panic!("Error writing file: {}", Error::description(&why)),
                Ok(ok) => ok,
            };
        }

        file.flush().unwrap();
    }
}
