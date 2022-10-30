function gradient(object,event){
    object.style.background = `radial-gradient(circle at ${(event.offsetX/1000)*100}%, darkgray,gray)`
};

function removeGradient(object){
    object.style.background = "#333333"
};