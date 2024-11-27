#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Tipos de tiles
#define TileEmpty 0
#define TileGround 1
#define TileEdgeTop 2
#define TileEdgeRight 3
#define TileEdgeBottom 4
#define TileEdgeLeft 5
#define TileCornerTL 6
#define TileCornerTR 7
#define TileCornerBR 8
#define TileCornerBL 9
#define TileHouse 10

#define TileSize 64
#define MaxTilesX 4
#define MaxTilesY 3

// Camara
#define CameraZoomAir 1.0
#define CameraZoomGround 0.5
#define CameraZoomSpeed 0.05

//Bullets
#define BulletSpeed 5.0
#define MaxBullets 50
#define BulletSize 16

// sprites
#define AirplaneSpriteWidth 128    // Ancho total del sprite
#define AirplaneSpriteHeight 91    // Alto total del sprite
#define AirplaneFrameWidth 128     // Cada frame es del ancho total
#define AirplaneFrameHeight 91     // Cada frame es del alto total
#define AirplaneFramesPerRow 1     // Los frames están uno sobre otro
#define IslandSpriteSize 64

#define TextureAirplane 0
#define TextureCarrier 1
#define TextureIsland 2
#define TextureBullet 3
#define TextureSoldier 4  // Ya que es la quinta textura (0-4)


// Define regiones para sprites
#define RegionAirplane 0
#define RegionAirplaneShadow 2
#define RegionCarrier 0
#define RegionIsland 0

// Dimensiones del mundo
#define WorldWidth 2000
#define WorldHeight 2000
#define ScreenWidth 640
#define ScreenHeight 360
#define ScreenCenterX (ScreenWidth / 2)
#define ScreenCenterY (ScreenHeight / 2)

// Configuración del avión
#define NumAirplaneFrames 2
#define AirplaneAnimSpeed 10
#define RotationSpeed 0.05
#define MovementSpeed 3.0
#define DescentSpeed 0.01
#define MaxScale 1.0
#define MinScale 0.2
#define AirplaneShadowOffset 20
#define MaxFuel 1000
#define FuelConsumption 0.5
#define RefuelRate 2.0
#define InitialScale 0.3
#define LandingScale 0.5

// Area de aterrizaje
#define LandingAreaWidth 60
#define LandingAreaHeight 60

// Portaaviones
#define StartingX (WorldWidth / 2)
#define StartingY (WorldHeight / 2)
#define CarrierWidth 77
#define CarrierHeight 255
#define LandingDistance 80

// Islas
#define MaxIslands 10
#define MinIslandSize 64
#define MaxIslandSize 192
#define NumIslandVariants 4

//Soldier
#define SoldierWidth 32
#define SoldierHeight 32
#define RegionSoldier 0

// Colores (formato ABGR)
#define BackgroundColor 0xFF000000
#define TextColor 0xFFFFFFFF
#define RedColor 0xFF0000FF
#define SeaColor 0xFF805020
#define ShadowColor 0x80000000
#define IslandColor 0xFF604020

// Estados del juego
#define StateMenu 0
#define StateGame 1
#define StateGameOver 2

#define BlinkRate 30

#endif