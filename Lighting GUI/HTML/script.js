window.onload=function(){
    if ("serial" in navigator) {
        connectButton = document.getElementById('connect');
        connectButton.addEventListener('click', async () => {
            // Prompt user to select any serial port.
            port = await navigator.serial.requestPort();
            await port.open({ baudRate: 115200 });

            encoder = new TextEncoder();
            writer = port.writable.getWriter();
            // writer.write(encoder.encode("0,0,0,0\n"));
            connectButton.remove();
            interval = undefined;
            timeout = undefined;
            fadeTime = 1;
            delayTime = 1;
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
    if(fadeLength!=undefined){
        differenceRed = (redVal-red)/(fadeLength*10)
        differenceGreen = (greenVal-green)/(fadeLength*10)
        differenceBlue = (blueVal-blue)/(fadeLength*10)
        interval = setInterval(function(){
            redVal -= Math.round(differenceRed)
            greenVal -= Math.round(differenceGreen)
            blueVal -= Math.round(differenceBlue)
            rgb(light,redVal,greenVal,blueVal);
        },100);
        setTimeout(function(){if(interval!=undefined){clearInterval(interval);rgb(light,red,green,blue)}},fadeLength*1000);
    } else {
        differenceRed = (redVal-red)/(fadeTime*10)
        differenceGreen = (greenVal-green)/(fadeTime*10)
        differenceBlue = (blueVal-blue)/(fadeTime*10)
        interval = setInterval(function(){
            redVal -= Math.round(differenceRed)
            greenVal -= Math.round(differenceGreen)
            blueVal -= Math.round(differenceBlue)
            rgb(light,redVal,greenVal,blueVal);
        },100);
        setTimeout(function(){if(interval!=undefined){clearInterval(interval);rgb(light,red,green,blue)}},fadeTime*1000);
    }
};

function motorbike(light,delay){
    if(delay!=undefined){
        if(light==0){
            interval = setInterval(function(){
                rgb(1,Math.round(Math.random()*255),Math.round(Math.random()*255),Math.round(Math.random()*255))
                rgb(2,Math.round(Math.random()*255),Math.round(Math.random()*255),Math.round(Math.random()*255))
            },delay*1000);
        } else {
            interval = setInterval(function(){
                rgb(light,Math.round(Math.random()*255),Math.round(Math.random()*255),Math.round(Math.random()*255))
            },delay*1000);
        }
    } else {
        if(light==0){
            interval = setInterval(function(){
                rgb(1,Math.round(Math.random()*255),Math.round(Math.random()*255),Math.round(Math.random()*255))
                rgb(2,Math.round(Math.random()*255),Math.round(Math.random()*255),Math.round(Math.random()*255))
            },delayTime*1000);
        } else {
            interval = setInterval(function(){
                rgb(light,Math.round(Math.random()*255),Math.round(Math.random()*255),Math.round(Math.random()*255))
            },delayTime*1000);
        }
    };
};

function stop(){
    clearInterval(interval);
    clearTimeout(timeout);
    interval = undefined;
    timeout = undefined;
};

function chase(light,array,delay,fadeLength){
    if(delay!=undefined){
        red = array[i][0];
        green = array[i][1];
        blue = array[i][2];
        if(fadeLength==undefined){
            fade(light,red,green,blue,fadeTime)
        } else {
            fade(light,red,green,blue,fadeLength)
        };
        timeout = setTimeout(function(){
            i++
            if (i<array.length) {
                chase(light,array,delay,fadeLength);
            };
        }, delay*1000)
    } else {
        red = array[i][0];
        green = array[i][1];
        blue = array[i][2];
        if(fadeLength==undefined){
            fade(light,red,green,blue,fadeTime)
        } else {
            fade(light,red,green,blue,fadeLength)
        };
        timeout = setTimeout(function(){
            i++
            if (i<array.length) {
                chase(light,array,delayTime,fadeLength);
            };
        }, delayTime*1000)
    }
};