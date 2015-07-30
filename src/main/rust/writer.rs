use std::collections::VecDeque;

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
    }
}
