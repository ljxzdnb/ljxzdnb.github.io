let balls = [];
let flush = 0;

const myKnobs = new Knobs({
  knobs: [
    "Simulation",
    {
      name: "maxParticles",
      label: "Max. particles",
      type: "number",
      value: 4096,
      min: 0,
      onChange: knobChange
    },
    {
      name: "minBallSize",
      label: "Min. ball size",
      type: "number",
      value: 2,
      min: 0,
      onChange: knobChange
    },
    {
      name: "maxBallSize",
      label: "Max. ball size",
      type: "number",
      value: 16,
      min: 1,
      onChange: knobChange
    },
    {
      name: "ballGrowth",
      label: "Ball growth",
      type: "number",
      value: 0.125,
      min: 0,
      step: 0.001,
      onChange: knobChange
    },
    {
      name: "ballShrinkth",
      label: "Ball shrinkth",
      type: "number",
      value: 0.25,
      min: 0,
      step: 0.001,
      onChange: knobChange
    },
    "Physics",
    {
      name: "airFriction",
      label: "Air friction",
      type: "number",
      value: 0.001,
      min: 0,
      max: 1,
      step: 0.001,
      onChange: knobChange
    },
    {
      name: "gravity",
      label: "Gravity",
      type: "number",
      value: 0.05,
      min: 0,
      step: 0.001,
      onChange: knobChange
    },
    {
      name: "followMouse",
      label: "Attractive mouse",
      type: "checkbox",
      checked: true,
      value: true,
      onChange: knobChange
    },
    "Rendering",
    {
      name: "minFps",
      label: "Min. framerate",
      type: "number",
      value: 60,
      min: 1,
      onChange: knobChange
    },
    {
      name: "blur",
      label: "Blur",
      type: "number",
      value: 16,
      min: 1,
      onChange: knobChange
    },
    {
      name: "hueStart",
      label: "Hue start",
      type: "number",
      value: 180,
      min: -360,
      max: 360,
      onChange: knobChange
    },
    {
      name: "hueEnd",
      label: "Hue end",
      type: "number",
      value: 240,
      min: -360,
      max: 360,
      onChange: knobChange
    }
  ]
});

function init() {
  while (balls.length < 1024) {
    balls.push(
      new Ball(Math.random() * canvas.width, Math.random() * canvas.height, 8)
    );
  }
  setInterval(() => {
    mouse.x = canvas.width / 2;
    mouse.y = canvas.height / 2;
  }, 1024 * 64);
}

function step(t) {
  if (flush) {
    balls.shift();
    flush--;
  }
  if (fps >= minFps) {
    if (balls.length < maxParticles) {
      let a = Math.random() * 2 * Math.PI;
      balls.push(
        new Ball(
          canvas.width / 2 + Math.sin(a) * 1024,
          canvas.height / 2 + Math.cos(a) * 1024
        )
      );
    }
  } else if (balls.length > 2) {
    balls.pop();
  }
  for (let ball of balls) {
    ball.update();
  }
  for (let i = 0; i < balls.length; i++) {
    for (let j = i + 1; j < balls.length; j++) {
      if (collides(balls[i], balls[j])) {
        balls[i].collide(balls[j]);
        balls[j].collide(balls[i]);
      }
    }
  }
  for (let ball of balls) {
    ball.draw();
  }
  g.fillStyle = "white";
  g.fillText("FPS: " + fps, 8, 16);
  g.fillText("Particles: " + balls.length, 8, 32);
}

function collides(a, b) {
  return Math.sqrt(sq(b.x - a.x) + sq(b.y - a.y)) < a.r + b.r;
}

function sq(n) {
  return n * n;
}

class Ball {
  constructor(x, y, r, c) {
    let v = Math.random();
    this.vx = 0;
    this.vy = 0;
    this.x = x || 0;
    this.y = y || 0;
    this.r = r || Math.random(); // * 12 + 12
    this.c =
      c || `hsl(${hueStart + v * (hueEnd - hueStart)}, 100%, ${65 - v * 30}%)`;
    this.t_offset = Math.random() * Math.PI * 2;

    this._vx = Math.random() - 0.5;
    this._vy = Math.random() - 0.5;
    this._x = this.x;
    this._y = this.y;
  }

  gravitate(x, y, g = 1) {
    let dx = x - this.x;
    let dy = y - this.y;
    if (dx == 0 && dy == 0) {
      dx = Math.random();
      dy = Math.random();
    }
    let dist = Math.sqrt(sq(dx) + sq(dy));
    this.vx += (dx / dist) * g;
    this.vy += (dy / dist) * g;
  }

  update(t) {
    this.vx *= 1 - airFriction;
    this.vy *= 1 - airFriction;
    if (this.grav) {
      this.gravitate(mouse.x, mouse.y, gravity);
      if (this.r > minBallSize && this.r > ballShrinkth) this.r -= ballShrinkth;
    } else {
      if (this.r < maxBallSize) this.r += ballGrowth;
    }
    this.x += this.vx;
    this.y += this.vy;

    this.grav = true;
    this._vx = this.vx;
    this._vy = this.vy;
    this._x = this.x;
    this._y = this.y;
  }

  collide(othr) {
    let dx = this.x - othr.x;
    let dy = this.y - othr.y;
    if (dx == 0 && dy == 0) {
      dx = Math.random();
      dy = Math.random();
    }
    let dist = Math.sqrt(sq(dx) + sq(dy));
    let ovr = (this.r + othr.r - dist) / 2;
    this._x += (dx / dist) * ovr;
    this._y += (dy / dist) * ovr;

    this._vx += (dx / dist) * 0.125;
    this._vy += (dy / dist) * 0.125;
    this.grav = false;
  }

  draw(t) {
    this.vx = this._vx;
    this.vy = this._vy;
    this.x = this._x;
    this.y = this._y;

    g.strokeStyle = this.c;
    g.fillStyle = this.c;
    g.beginPath();
    g.arc(this.x, this.y, this.r, 0, Math.PI * 2, false);
    g.fill();
  }
}

// --- //
function resize() {
  _canvas.width = window.innerWidth;
  _canvas.height = window.innerHeight;
  _g.globalAlpha = 1;
  _g.fillStyle = "black";
  _g.fillRect(0, 0, _canvas.width, _canvas.height);
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;
  mouse.x = canvas.width / 2;
  mouse.y = canvas.height / 2;
}
function mouseMove(e) {
  if (followMouse) {
    mouse.x = e.clientX;
    mouse.y = e.clientY;
  }
}

const canvas = document.createElement("canvas");
const _canvas = document.querySelector("canvas");
const g = canvas.getContext("2d");
const _g = _canvas.getContext("2d");
const mouse = {};

let fps = 9000;
let _fps = 0;
let _sec;

addEventListener("resize", resize);
document.body.addEventListener("mousemove", mouseMove);
resize();
init();
const raf = (t = 0) => {
  requestAnimationFrame(raf);
  if (_sec !== Math.floor(t / 1000)) {
    fps = _fps;
    _fps = 0;
    _sec = Math.floor(t / 1000);
  }
  _fps++;
  g.fillStyle = "black";
  g.fillRect(0, 0, canvas.width, canvas.height);
  step(t);
  _g.globalAlpha = 1 / blur;
  _g.drawImage(canvas, 0, 0);
};
raf();

function knobChange(e, knob) {
  let val = typeof knob.checked == "boolean" ? knob.checked : knob.value;
  if (knob.type === "number") val = parseFloat(val);
  console.log(knob.name, "=", val);
  window[knob.name] = val;
  flush = balls.length;
  try {
    mouse.x = canvas.width / 2;
    mouse.y = canvas.height / 2;
  } catch (err) {}
}