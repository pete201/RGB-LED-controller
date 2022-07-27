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
            rgb(0,0,0,0);
            // writer.releaseLock();
        });
    }
};

function rgb(light,red,green,blue){
    writer.write(encoder.encode(`${light},${red},${green},${blue}\n`));
}