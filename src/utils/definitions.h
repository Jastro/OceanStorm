#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Define algunas constantes para el juego
#define RegionAirplane 0
#define RegionAirplaneShadow 1
#define RegionCarrier 2

// Dimensiones del mundo
#define WorldWidth 2000
#define WorldHeight 2000
#define ScreenWidth 640
#define ScreenHeight 360
#define ScreenCenterX (ScreenWidth / 2)
#define ScreenCenterY (ScreenHeight / 2)

// Configuración del avión
#define RotationSpeed 0.05
#define MovementSpeed 3.0
#define DescentSpeed 0.01
#define MaxScale 1.0
#define MinScale 0.2
#define AirplaneShadowOffset 10  // Offset de la sombra

// Portaaviones
#define CarrierX 400
#define CarrierY 300
#define CarrierWidth 100
#define LandingDistance 50

// Colores (formato ABGR)
#define BackgroundColor 0xFF000000  // Negro opaco
#define TextColor 0xFFFFFFFF       // Blanco opaco
#define RedColor 0xFF0000FF       // Rojo opaco
#define SeaColor 0xFF805020       // Azul marino oscuro
#define ShadowColor 0x80000000    // Negro semitransparente

// Estados del juego
#define StateMenu 0
#define StateGame 1
#define StateGameOver 2

// Framerate para el parpadeo del texto
#define BlinkRate 30

#endif