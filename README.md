wombo-gravity
=============

N-body rust simulator with webgl graphic rendering.

### Dependencies

* Rust
* Python 3+
* Bower

### Compiling

`cargo build --release` build wombo simulator with compiler optimisations

`cd src/main/web/ && bower install` install web dependencies


### Running
`cargo run --release` run wombo simulator with compiler optimisations

`bash webui.bash` start the python web server

`chromium http://localhost:8000` view index.html in a browser (chromium recommended)
