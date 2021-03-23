const express = require('express');
const bodyParser = require('body-parser');
const path = require('path');
const numeros = require('./dummy');
const usuarios = require('./dummy2');
const { createGunzip } = require('zlib');
const { exec } = require('child_process');
const { stdout } = require('process');

const app = express();

//#region Middlewares

app.use(bodyParser.urlencoded({extended:false}));
app.use(bodyParser.json());

//#endregion

//#region CORS

//#region Rutas

app.get('/', (req, res)=> {
    res.status(200).send('<h1>Mi sitio web</h1> <div> <p>Este es mi primer servidor del curso ESP32 y AWS</p> </div>');
})

app.get('/homepage',(req, res)=> {
    res.sendFile(path.join(__dirname + '/index.html'));
})

const data = {
    message : 'datos',
    payload : {
        temperatura : '25',
        presion : '1 bar'
    }
}

app.get('/data', (req, res) => {
    // Operación de ordenamiento de numeros
    res.status(200).send(ordenarNumeros(numeros));
})

app.get('/usuarios', (req, res) => {
    // Operación de ordenamiento de usuarios por id
    res.status(200).send(usuarios.sort((a ,b) => {return a.id - b.id}));
})

app.get('/users/:id', (req, res) => {
    const id = req.params.id;
    var user = {};
    for(let u of usuarios){
        if(u.id == id){
            user = u;
        }
    }

    // Comprimir la llave del usuario
    res.status(200).send(comprimirCadena(user.clave));
})

app.get('/publish', (req, res) => {

    // Una forma de ejecutar comandos de la terminar desde Nodejs
    exec("aws --region us-east-1 iot-data publish --topic 'inTopic' --cli-binary-format raw-in-base64-out --payload '\"llave\" : \"valor\"'", (error, stdout, stderr) => {
        if(error){
            res.status(200).send(error);
        }
        if(stderr){
            res.status(200).send(stderr);
        }
        res.status(200).send("Enviado correctamente :D");
    })
})

//#endregion

//#region Funciones.

const ordenarNumeros = function(numerosDesordenados){

    var bandera = 0;
    var numeroAux = numerosDesordenados[0];
    console.log(numerosDesordenados)

    do{
        bandera = 0;
        numeroAux = numerosDesordenados[0];
        for(const i  in numerosDesordenados){

            if(i > 0){

                if(numerosDesordenados[i] < numeroAux){
                    var aux = numerosDesordenados[i];
                    numerosDesordenados[i] = numeroAux;
                    numerosDesordenados[i - 1] = aux;
                    bandera = 1;
    
                }else {
                    numeroAux = numerosDesordenados[i];
                }
            }

            
        }
    }while(bandera == 1)

    console.log(numerosDesordenados);
    return numerosDesordenados;
}

const comprimirCadena = (cadena) => {

    var charRef = '';
    var contador = 0;
    var contadorCadena = 0;
    var nuevaCadena = '';

    for(let c of cadena ){
        contadorCadena = contadorCadena + 1;
        if(c === charRef){
            contador = contador + 1;
        } else{
            nuevaCadena = nuevaCadena + charRef
            if(c !== '' && contador > 0){
                nuevaCadena = nuevaCadena + String(contador + 1);
            }
            charRef = c;
            contador = 0;
        }
        if(contadorCadena === cadena.length){
            nuevaCadena = nuevaCadena + c;
            if(c !== '' && contador > 0) nuevaCadena = nuevaCadena + String(contador + 1);
        }
    }

    return nuevaCadena;
}

//#endregion

module.exports = app;