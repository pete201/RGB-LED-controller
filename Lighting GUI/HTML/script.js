window.onload=function(){
    if ("serial" in navigator) {
        console.log(true);
        connectButton = document.getElementById('connect');
        connectButton.addEventListener('click', async () => {
            // Prompt user to select any serial port.
            port = await navigator.serial.requestPort();
            console.log(port);
            await port.open({ baudRate: 115200 });

            encoder = new TextEncoder();
            writer = port.writable.getWriter();
            // writer.write(encoder.encode("0,0,0,0\n"));
            connectButton.remove();
            rgb(0,0,0,0);
            // writer.releaseLock();
        });
    }
};

function rgb(light,red,green,blue){
    redVal = red;
    greenVal = green;
    blueVal = blue;
    writer.write(encoder.encode(`${light},${red},${green},${blue}\n`));
}

function fade(light,red,green,blue,fadeLength){
    differenceRed = (redVal-red)/(fadeLength*10)
    differenceGreen = (greenVal-green)/(fadeLength*10)
    differenceBlue = (blueVal-blue)/(fadeLength*10)
    interval = setInterval(function(){
        redVal -= Math.round(differenceRed)
        greenVal -= Math.round(differenceGreen)
        blueVal -= Math.round(differenceBlue)
        rgb(light,redVal,greenVal,blueVal);
    },100);
    setTimeout(function(){clearInterval(interval);rgb(light,red,green,blue)},fadeLength*1000);
};