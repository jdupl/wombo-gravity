var container, stats;
var scene, camera, renderer;
var geometry, material, mesh;
var bodies = {};

var fps = 60;

var scaleFactor = 10000000;


function JsonFrameProvider() {
    this.currentFrameIndex = 0;
    this.frames = {};
}

JsonFrameProvider.prototype.getNextFrame = function() {
    if (this.currentFrameIndex >= this.frames.length) {
        // reset animation
        this.currentFrameIndex = 0;
    }
    return this.frames[this.currentFrameIndex++];
};

JsonFrameProvider.prototype.load = function () {
    var self = this;
    $.getJSON("data/frames.json", function(data) {
        self.frames = data.frames;
        initFirstFrame(self.getNextFrame());
    });
};

function BinaryFrameProvider() {
    this.currentByteIndex = 0;
}

BinaryFrameProvider.prototype.getNextFrame = function() {
    if (this.currentByteIndex >= this.binData.byteLength) {
        // reset animation
        this.currentByteIndex = 0;
    }
    return this.readFrame();
};

BinaryFrameProvider.prototype.readFrame = function() {
    var frame = {};
    frame.bodies = [];
    var frameHeader = new Uint32Array(this.binData, this.currentByteIndex, 2);
    this.currentByteIndex += 2 * 4;

    frame.id = frameHeader[0];
    frame.bodyCount = frameHeader[1];

    for (var i = 0; i < frameHeader[1]; i++) {
        var body = {};
        body.id = new Uint32Array(this.binData, this.currentByteIndex, 1)[0];
        this.currentByteIndex += 4;
        this.currentByteIndex += 4; // padding

        var coords = new Float64Array(this.binData, this.currentByteIndex, 3);
        this.currentByteIndex += 3 * 8;
        body.rx = coords[0];
        body.ry = coords[1];
        body.rz = coords[2];

        frame.bodies[body.id] = body;
    }
    return frame;
}

BinaryFrameProvider.prototype.load = function() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/data/out.bin', true);
    xhr.responseType = 'arraybuffer';

    var self = this;
    xhr.onload = function(e) {
        self.currentByteIndex = 0;
        self.binData = this.response;

        initFirstFrame(self.readFrame());
    };

    xhr.send();
}

function initFirstFrame(frame) {
    var bodiesData = frame.bodies;
    var geometry = new THREE.SphereGeometry(200, 8, 6);
    console.log(frame);

    for (var i = 0; i < bodiesData.length; i++) {
        var material = new THREE.PointCloudMaterial({size: 25, sizeAttenuation: false, color: Math.random() * 0xffffff});
        var body = new THREE.Mesh(geometry, material);

        body.position.x = bodiesData[i].rx / scaleFactor;
        body.position.y = bodiesData[i].ry / scaleFactor;
        body.position.z = bodiesData[i].rz / scaleFactor;

        var id = bodiesData[i].id
        bodies[id] = body;

        scene.add(body);
    }
    setInterval(updateData, 1000 / fps);
}

function updateData() {
    var frame = provider.getNextFrame();
    bodiesData = frame.bodies;

    for (var i = 0; i < bodiesData.length; i++) {
        var bodyData = bodiesData[i];
        var obj = bodies[bodyData.id];

        if (obj) {
            obj.position.x = bodyData.rx / scaleFactor;
            obj.position.y = bodyData.ry / scaleFactor;
            obj.position.z = bodyData.rz / scaleFactor;
        }
    }
}

function init() {
    var mouse = new THREE.Vector2();
    container = document.createElement('div');
    document.body.appendChild(container);

    stats = new Stats();
    stats.domElement.style.position = 'absolute';
    stats.domElement.style.top = '0px';
    container.appendChild(stats.domElement);

    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1000, 100000000);
    camera.position.x = 100000;

    controls = new THREE.TrackballControls(camera);
    controls.rotateSpeed = 10.0;
    controls.zoomSpeed = 1.2;
    controls.panSpeed = 1;
    controls.noZoom = false;
    controls.noPan = false;
    controls.staticMoving = true;
    controls.dynamicDampingFactor = 1;

    scene = new THREE.Scene();
    // scene.fog = new THREE.Fog(0x050505, 2000, 3500);

    renderer = new THREE.WebGLRenderer({antialias: true, logarithmicDepthBuffer: true});
    // renderer = new THREE.WebGLRenderer();
    renderer.setSize(window.innerWidth, window.innerHeight);

    container.appendChild(renderer.domElement);
    container.appendChild(stats.domElement);

    window.addEventListener('resize', onWindowResize, false);
}

function render() {
    requestAnimationFrame(render);
    controls.update();

    renderer.render(scene, camera);
    stats.update();
}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();

    renderer.setSize(window.innerWidth, window.innerHeight);
}

function onMouseMove(e) {
    // hack for TrackballControls
    mouse.x = e.clientX;
    mouse.y = e.clientY;
}

init();

// var provider = new BinaryFrameProvider();
var provider = new JsonFrameProvider();
provider.load();

render();
