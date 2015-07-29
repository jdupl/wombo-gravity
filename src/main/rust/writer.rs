use std::collections::VecDeque;

use frame::Frame;

pub trait Writer {

    fn flush(&self, frames: &mut VecDeque<Frame>) {
        while frames.len() > 0 {
            self.write(&frames.pop_front().unwrap());
        }
    }

    fn write(&self, frame: &Frame);
}
