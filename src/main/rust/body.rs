#[derive(Debug, Clone)]
pub struct Body {
    pub id: u32,
    pub name: String,
    pub m: f64,
    // position
    pub rx: f64,
    pub ry: f64,
    pub rz: f64,
    // velocity
    pub vx: f64,
    pub vy: f64,
    pub vz: f64,
    // acceleration
    pub ax: f64,
    pub ay: f64,
    pub az: f64,
}

impl Body {
    pub fn update_pos(&mut self, duration_sec: f64) {
        self.ax /= self.m;
        self.ay /= self.m;
        self.az /= self.m;

        self.rx += 0.5 * self.ax * duration_sec * duration_sec + self.vx * duration_sec;
        self.ry += 0.5 * self.ay * duration_sec * duration_sec + self.vy * duration_sec;
        self.rz += 0.5 * self.az * duration_sec * duration_sec + self.vz * duration_sec;

        // Calcul de la nouvelle vitesse
        self.vx += self.ax * duration_sec;
        self.vy += self.ay * duration_sec;
        self.vz += self.az * duration_sec;

        self.ax = 0.0;
        self.ay = 0.0;
        self.az = 0.0;
    }
}

pub struct Frame {
    pub id: u32,
    pub bodies: Vec<Body>,
}
