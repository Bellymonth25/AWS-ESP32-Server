const app = require('./app');

const port = 8000;

//#region Rutas

app.listen(port, ()=>{
    console.log("Servidor corriendo correctamente en el puerto 8000");
})

//#endregion