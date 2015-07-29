use writer::Writer;
use frame::Frame;

pub struct BinaryWriter;

impl Writer for BinaryWriter {

    fn write(&self, frame: &Frame) {
        println!("writing frame {}", frame.id);
    }
}
