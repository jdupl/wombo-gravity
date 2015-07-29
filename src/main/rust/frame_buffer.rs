use std::collections::VecDeque;

use frame::Frame;
use writer::Writer;

pub struct FrameBuffer<T> {
    writer: T,
    frames: VecDeque<Frame>,
}

impl<T: Writer> FrameBuffer<T> {
    pub fn new(writer: T) -> FrameBuffer<T> {
        FrameBuffer {
            frames: VecDeque::<Frame>::new(),
            writer: writer,
        }
    }

    pub fn add(&mut self, frame: Frame) {
        self.frames.push_back(frame);
        if self.frames.len() > 10 {
            self.writer.flush(&mut self.frames);
        }
    }

    pub fn close(&mut self) {
        self.writer.flush(&mut self.frames);
    }
}
