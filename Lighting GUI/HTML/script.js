window.onload=function(){
    if ("serial" in navigator) {
        connectButton = document.getElementById('connect');
        dataText = document.getElementById('data');
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
            colourPicker = [255,255,255];
            lightPicker = 0;
            data = ["Connected"];
            rgb(0,0,0,0);
            // writer.releaseLock();
        });
    } else {
        document.getElementById('data').innerHTML = "Serial Port Web API unavailable in this browser"
    };
};

function rgb(light,red,green,blue){
    redVal = red;
    greenVal = green;
    blueVal = blue;
    writer.write(encoder.encode(`${light},${red},${green},${blue}\n`));
    // if(data.length>=3){
    //     data.splice(0,1);
    // };
    data.push([light,red,green,blue])
    if(data.length>500){
        data.splice(0,1);
    };
    // dataText.innerHTML = data[0] + "\n" + data[1] + "\n" + data[2]
    dataText.innerHTML = ""
    for(let i=0;i<data.length;i++){
        dataText.innerHTML += data[i] + "\n";
    };
    dataText.scrollTop=dataText.scrollHeight;
    // dataText.innerHTML = data;
}

function fade(light,red,green,blue,fadeLength){
    if(fadeLength!=undefined){
        fadeVal = [light,red,green,blue,fadeLength];
        differenceRed = (redVal-red)/(fadeLength*10)
        differenceGreen = (greenVal-green)/(fadeLength*10)
        differenceBlue = (blueVal-blue)/(fadeLength*10)
        interval = setInterval(function(){
            redVal -= Math.round(differenceRed)
            greenVal -= Math.round(differenceGreen)
            blueVal -= Math.round(differenceBlue)
            if(redVal<=0){
                redVal = 0;
            }
            if(greenVal<=0){
                greenVal = 0;
            }
            if(blueVal<=0){
                blueVal = 0;
            }
            if(redVal>=255){
                redVal = 255;
            }
            if(greenVal>=255){
                greenVal = 255;
            }
            if(blueVal>=255){
                blueVal = 255;
            }
            rgb(light,redVal,greenVal,blueVal);
        },100);
        setTimeout(function(){
            if(interval!=undefined
                &&fadeVal[0]==light
                &&fadeVal[1]==red
                &&fadeVal[2]==green
                &&fadeVal[3]==blue
                &&fadeVal[4]==fadeLength){
                    clearInterval(interval);
                    interval=undefined;
                    rgb(light,red,green,blue)
            }
        },fadeLength*1000);
    } else {
        fadeVal = [light,red,green,blue,fadeTime];
        differenceRed = (redVal-red)/(fadeTime*10)
        differenceGreen = (greenVal-green)/(fadeTime*10)
        differenceBlue = (blueVal-blue)/(fadeTime*10)
        interval = setInterval(function(){
            redVal -= Math.round(differenceRed)
            greenVal -= Math.round(differenceGreen)
            blueVal -= Math.round(differenceBlue)
            if(redVal<=0){
                redVal = 0;
            }
            if(greenVal<=0){
                greenVal = 0;
            }
            if(blueVal<=0){
                blueVal = 0;
            }
            if(redVal>=255){
                redVal = 255;
            }
            if(greenVal>=255){
                greenVal = 255;
            }
            if(blueVal>=255){
                blueVal = 255;
            }
            rgb(light,redVal,greenVal,blueVal);
        },100);
        setTimeout(function(){
            if(interval!=undefined
                &&fadeVal[0]==light
                &&fadeVal[1]==red
                &&fadeVal[2]==green
                &&fadeVal[3]==blue
                &&fadeVal[4]==fadeTime){
                    clearInterval(interval);
                    interval=undefined;
                    rgb(light,red,green,blue)
            }
        },fadeTime*1000);
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

function hexToRGB(hex){
    hex = hex.slice(1)
    rgbhex = hex.match(/.{1,2}/g)
    rgbValues = [
        parseInt(rgbhex[0], 16),
        parseInt(rgbhex[1], 16),
        parseInt(rgbhex[2], 16)
    ];
    return rgbValues
};