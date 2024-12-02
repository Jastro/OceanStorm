#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// ---- TEXTURAS ----
#define TextureAirplane 0
#define TextureCarrier 1
#define TextureIsland 2
#define TextureBullet 3
#define TextureSoldier 4
#define TextureTurretBase 5
#define TextureTurretGun 6
#define TextureEnemyBullet 7

// ---- REGIONES ----
#define RegionAirplane 0
#define RegionAirplaneShadow 2
#define RegionCarrier 0
#define RegionIsland 0
#define RegionSoldier 0
#define RegionTurretBase 0
#define RegionTurretGun 1

// ---- DIMENSIONES DE SPRITES ----
// Avión
#define AirplaneSpriteWidth 128
#define AirplaneSpriteHeight 91
#define AirplaneFrameWidth 128
#define AirplaneFrameHeight 91
#define AirplaneFramesPerRow 1

// Soldado
#define SoldierWidth 32
#define SoldierHeight 32

// Torreta
#define TurretBaseWidth 32
#define TurretBaseHeight 32
#define TurretGunWidth 32
#define TurretGunHeight 38

// ---- SISTEMA DE TILES ----
#define TileSize 64
#define MaxTilesX 4
#define MaxTilesY 3

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

// ---- CONFIGURACIÓN DE CÁMARA ----
#define CameraZoomAir 1.0
#define CameraZoomGround 0.5
#define CameraZoomSpeed 0.05

// ---- SISTEMA DE PROYECTILES ----
#define MaxBullets 50
#define BulletSpeed 5.0
#define BulletSize 8

// ---- CONFIGURACIÓN DE TORRETAS ----
#define MaxTurrets 10
#define TurretVisionRange 300.0
#define TurretFireRate 3.0
#define TurretBulletSpeed 8.0
#define TurretBulletRange 400.0
#define TurretDamage 10.0

// ---- DIMENSIONES DEL MUNDO ----
#define WorldWidth 2000
#define WorldHeight 2000
#define ScreenWidth 640
#define ScreenHeight 360
#define ScreenCenterX (ScreenWidth / 2)
#define ScreenCenterY (ScreenHeight / 2)

// ---- CONFIGURACIÓN DEL AVIÓN ----
#define NumAirplaneFrames 2
#define AirplaneAnimSpeed 10
#define RotationSpeed 0.05
#define MovementSpeed 3.0
#define AscendSpeed 0.001
#define DescentSpeed 0.001
#define MaxScale 0.8
#define MinScale 0.2
#define AirplaneShadowOffset 20
#define MaxFuel 1000
#define FuelConsumption 0.5
#define RefuelRate 2.0
#define InitialScale 0.8
#define LandingScale 0.4

// ---- ÁREA DE ATERRIZAJE ----
#define LandingAreaWidth 77
#define LandingAreaHeight 255

// ---- PORTAAVIONES ----
#define StartingX (WorldWidth / 2)
#define StartingY (WorldHeight / 2)
#define CarrierWidth 77
#define CarrierHeight 255
#define LandingDistance 80

// ---- ISLAS ----
#define MaxIslands 10
#define MinIslandSize 64
#define MaxIslandSize 192
#define NumIslandVariants 4

// ---- EVENTOS ----
#define EventFirstExit 0
#define EventFirstTurret 1
#define EventFirstPickup 2
#define MaxEvents 10

// ---- ESTADOS DEL JUEGO ----
#define StateMenu 0
#define StateGame 1
#define StateGameOver 2

// ---- COLORES ----
#define BackgroundColor 0xFF000000
#define TextColor 0xFFFFFFFF
#define RedColor 0xFF0000FF
#define SeaColor 0xFF805020
#define ShadowColor 0x80000000
#define IslandColor 0xFF604020

#define BlinkRate 30

#endif