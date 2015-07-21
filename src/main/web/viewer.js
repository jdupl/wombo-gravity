var container, stats;
var scene, camera, renderer;
var geometry, material, mesh;
var bodies = {};
var frames = {};
var currentFrameIndex = 0;

var binData;
var currentByteIndex = 0;

var fps = 10;

var scaleFactor = 10000000;

init();
loadBinData();
render();

function loadJsonData() {
    $.getJSON("data/frames.json", function(data) {
        frames = data.frames;
        var firstFrame = frames[currentFrameIndex++];

        jsonBodies = firstFrame.bodies;
        var geometry = new THREE.SphereGeometry(200, 8, 6);

        for (var i = 0; i < jsonBodies.length; i++) {
            var material = new THREE.PointCloudMaterial({size: 25, sizeAttenuation: false, color:Math.random() * 0xffffff});
            var body = new THREE.Mesh(geometry, material);

            body.position.x = jsonBodies[i].rx / scaleFactor;
            body.position.y = jsonBodies[i].ry / scaleFactor;
            body.position.z = jsonBodies[i].rz / scaleFactor;

            var name = jsonBodies[i].name
            bodies[name] = body;

            scene.add(body);
        }
        setInterval(updateData, 1000 / fps);
    });
}

function loadBinData() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/data/out.bin', true);
    xhr.responseType = 'arraybuffer';

    xhr.onload = function(e) {
        currentByteIndex = 0;
        binData = this.response;
        var firstFrame = readBinFrame();

        bodiesData = firstFrame.bodies;
        var geometry = new THREE.SphereGeometry(200, 8, 6);

        for (var i = 0; i < bodiesData.length; i++) {
            var material = new THREE.PointCloudMaterial({size: 25, sizeAttenuation: false, color:Math.random() * 0xffffff});
            var body = new THREE.Mesh(geometry, material);

            body.position.x = bodiesData[i].rx / scaleFactor;
            body.position.y = bodiesData[i].ry / scaleFactor;
            body.position.z = bodiesData[i].rz / scaleFactor;

            var id = bodiesData[i].id
            bodies[id] = body;

            scene.add(body);
        }
        setInterval(updateBinData, 1000 / fps);
    };

    xhr.send();
}

function readBinFrame() {
    var frame = {};
    frame.bodies = [];

    var frameHeader = new Uint32Array(binData, currentByteIndex, 2);
    currentByteIndex += 2 * 4;

    frame.id = frameHeader[0];
    frame.bodyCount = frameHeader[1];

    for (var i = 0; i < frameHeader[1]; i++) {
        var body = {};
        body.id = new Uint32Array(binData, currentByteIndex, 1)[0];
        currentByteIndex += 4;
        currentByteIndex += 4; // padding

        var coords = new Float64Array(binData, currentByteIndex, 3);
        currentByteIndex += 3 * 8;
        body.rx = coords[0];
        body.ry = coords[1];
        body.rz = coords[2];

        frame.bodies[body.id] = body;
    }
    return frame;
}

function updateData() {
    if (currentFrameIndex >= frames.length) {
        // reset animation
        currentFrameIndex = 0;
    }
    var frame = frames[currentFrameIndex++];
    jsonBodies = frame.bodies;

    for (var i = 0; i < jsonBodies.length; i++) {
        jsonBody = jsonBodies[i];
        var obj = bodies[jsonBody.name];

        if (obj) {
            obj.position.x = jsonBody.rx / scaleFactor;
            obj.position.y = jsonBody.ry / scaleFactor;
            obj.position.z = jsonBody.rz / scaleFactor;
        }
    }
}

function updateBinData() {
    if (currentByteIndex >= binData.byteLength) {
        // reset animation
        currentByteIndex = 0;
    }
    var frame = readBinFrame();

    for (var i = 0; i < frame.bodies.length; i++) {
        bodyData = frame.bodies[i];
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
