# Alarma para carro

Asegurar un automóvil alertando a su responsable para evitar daños y/o robos. Incluye algunas funciones generales de las alarmas, se implementa con Micro-controladores PIC16F877A en un circuito impreso especialmente diseñado para este propósito usando Proteus.

Este proyecto se desarrollo en el año 2016.

## Comenzando 🚀

```sh
git clone https://github.com/MrCabrac/alarma-carro.git
```
Se puede llevar a cabo una simulación en el programa **Proteus 8.7** abriendo el archivo `Simulacion.pdsprj` disponible en la carpeta `simulation`.

El circuito impreso principal del sistema se puede encontrar en la carpeta `impreso` con el nombre `impreso_v2_full.pdf` listo para imprimir y fabricar por el método del planchado. Así mismo, el esquemático y PCB se puede extraer del archivo `Impreso v2.pdsprj` usando **Proteus**.

Otros circuitos adicionales que complementan el principal son:
* Indicador: Que muestra en un arreglo de 6 LED el estado de las 6 puertas de un automóvil, 4 para pasajeros, baúl y *capot*. Disponible en la carpeta `indicator` como circuito impreso y esquemático en **Proteus**.
* Potencia (*etapa de potencia*): Que sirve a la tarjeta central para controlar algunos de los los actuadores del automóvil sin comprometer eléctronicamente a la tarjeta principal:
    * Bocina
    * Estacionarias
    * Luces altas
    * Poder (?)
    * Luz de cielo
    * Luz de baúl
    * Luz de capot

Disponible en la carpeta `potencia` como circuito impreso y esquemático en **Proteus**.


### Pre-requisitos 📋
Para probar el proyecto:
* Tarjeta principal.
* Tarjeta de potencia.
* Programador para microcontroladores.
* Microcontrolador con firmware `Alarma.hex` en la carpeta `code`.

Si se desea desarrollar sobre el proyecto:
* PIC C Compiler o MPLAB
* Sofware de simulación para microcontroladores: Proteus o NI Multisim

<!--
>
### Instalación 🔧

## Ejecutando las pruebas ⚙️

## Despliegue 📦

## Construido con 🛠️

## Contribuyendo 🖇️

## Wiki 📖

## Versionado 📌
<-->
## Autores ✒️
* **Brayan Martínez** - *Trabajo Inicial* - [MrCabrac](https://github.com/MrCabrac)

## Licencia 📄

## Expresiones de Gratitud 🎁
A los profesores **Jorge Mario** y **Antonio Galeano** de la **I.E La Milagrosa** que en su momento sirvieron como guía para este proyecto.