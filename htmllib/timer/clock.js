function myTime(){
    const time = new Date();
    const hh = time.getHours();
    const mm = time.getMinutes();
    const ss = time.getSeconds();
    
    document.getElementById('clock').innerText = Math.floor(hh / 10) + (hh % 10 + ':') + Math.floor(mm / 10) + mm % 10 + ':' + Math.floor(ss / 10)+ss % 10;
}
myTime();
setInterval(myTime,1000);