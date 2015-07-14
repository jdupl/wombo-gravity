var container, stats;
var scene, camera, renderer;
var geometry, material, mesh;
var bodies = {};
var frames = {};
var currentFrameIndex = 0;

var fps = 10;

var scaleFactor = 10000000;

init();
loadData();
render();

function loadData() {
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
