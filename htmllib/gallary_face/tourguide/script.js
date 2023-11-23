/* image created by wirestock from freepik 'https://www.freepik.com/photos/business'
*/

let container = document.querySelector(".container");
let left = document.querySelector(".left-side");
let right = document.querySelector(".right-side");
let center = document.querySelector(".center");
let state = "closed";

container.onclick = function () {
  let tl = gsap.timeline();
  if (state == "closed") {
    right.style.transform = "rotateY(-20deg)";
    state = "half";
    tl.set(".center-content", {opacity: 0});
    return state;
  } else if (state == "half") {
    left.style.transform = "rotateY(20deg)";
    state = "open";
    tl.to(".center-content", 1, {opacity: 1});
    return state;
  } else {
    right.style.transform = "rotateY(-180deg)";
    left.style.transform = "rotateY(180deg)";
    tl.set(".center-content", {opacity: 0});
    state = "closed";
    return state;
  }
};