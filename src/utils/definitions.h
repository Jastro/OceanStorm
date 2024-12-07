#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// ---- TEXTURAS ----
enum Textures {
    TextureAirplane,
    TextureCarrier,
    TextureIsland,
    TextureBullet,
    TextureSoldier,
    TextureTurretBase,
    TextureTurretGun,
    TextureEnemyBullet,
    TextureEnemy,
    TextureEnemyKamikaze,
    TextureEnemyBoss,
    TexturePortraitPlayer,
    TexturePortraitCommander,
    TexturePortraitSoldier,
    TexturePortraitBoss,
    TextureDialog,
    TextureMinimap,
    TextureLargeShip,
    TextureMediumShip,
    TextureSmallShip,
    TextureShipTurret,
    TextureShipRocket
};

// ---- REGIONES ----
#define RegionAirplane 0
#define RegionAirplaneShadow 2
#define RegionCarrier 0
#define RegionIsland 0
#define RegionSoldier 0
#define RegionTurretBase 0
#define RegionTurretGun 1

enum RegionsMinimap {
    RegionMapFrame,
    RegionMapSmallIsland,
    RegionMapLargeIsland,
    RegionMapCarrier,
    RegionMapHeli
};

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
#define PistolDamage 25
#define ShotgunDamage 15 // Por cada perdigón
#define SubmachineGunDamage 10

// Torreta
#define TurretBaseWidth 32
#define TurretBaseHeight 32
#define TurretGunWidth 32
#define TurretGunHeight 38

// ---- SISTEMA DE TILES ----
#define TileSize 64
#define MaxTilesX 6
#define MaxTilesY 6

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

// ---- SISTEMA DE BOMBAS ----
#define BombTimerSeconds 5.0
#define MaxActiveBombs 5
#define BombExplosionRadius 50.0

// ---- SISTEMA DE PROYECTILES ----
#define MaxBullets 50
#define BulletSpeed 5.0
#define BulletSize 8

// ---- CONFIGURACIÓN DE TORRETAS ----
#define MaxTurrets 10
#define TurretVisionRange 300.0
#define TurretFireRate 10.0
#define TurretBulletSpeed 8.0
#define TurretBulletRange 400.0
#define TurretMinSpread 3
#define TurretMaxSpread 6
#define TurretDamagePerBullet 25

// ---- ENEMIGOS ----
#define MaxEnemies 20
#define EnemyTypeSoldier 0
#define EnemyTypeAirplane 1
#define EnemyBulletRange 300.0
#define EnemyBlinkTime 0.1
#define ReloadTime 2.0
#define ReloadTextOffset 30

#define KamikazeBulletSpeed 4.0
#define BossBulletSpeed 4.5
#define NormalEnemyBulletSpeed 4.0

// Soldados enemigos
#define MinSoldiersPerTurret 2
#define MaxSoldiersPerTurret 4
#define SoldierEnemyHealth 10
#define SoldierEnemyArmor 2
#define SoldierEnemySpeed 0.4
#define SoldierEnemyFireRate 0.05
#define SoldierEnemyDamage 10
#define SoldierBlinkTime 0.2
#define SoldierEnemyRange 150.0
#define SoldierSpawnRadius 100
#define SoldierBulletSpeed 2.0
#define EnemyCollisionRadius 40

#define BossPhase1Health 400   // Cambia de fase cuando baja a este valor
#define BossPhase2Health 200   // Y a este
#define BossPhase1FireRate 0.2 // Dispara rápido pero preciso
#define BossPhase2FireRate 0.5 // Más lento pero más disperso
#define BossPhase3FireRate 0.1

// BARCOS
#define LargeShipWidth 44
#define LargeShipHeight 142
#define LargeTurretWidth 20
#define LargeTurretHeight 18

#define MediumShipWidth 16
#define MediumShipHeight 58
#define MediumRocketWidth 12
#define MediumRocketHeight 10
#define MediumMachineGunWidth 9
#define MediumMachineGunHeight 13

#define SmallShipWidth 14
#define SmallShipHeight 39


// Tipos de enemigos
#define EnemyTypeNormal 0
#define EnemyTypeKamikaze 1
#define EnemyTypeBoss 2
#define EnemyTypeSoldier 3

// Puntos de vida
#define EnemyHealthNormal 100
#define EnemyHealthKamikaze 50
#define EnemyHealthBoss 500

// Características de enemigos
#define EnemyBulletRange 300.0
#define EnemyBlinkTime 0.1
#define KamikazeDamage 5

// Velocidades de movimiento
#define EnemySpeedNormal 3.0
#define EnemySpeedKamikaze 2.0
#define EnemySpeedBoss 5.0

#define AIBehaviorChase 0       // Persigue al jugador
#define AIBehaviorShootAndRun 1 // Dispara y se aleja
#define EnemySpeedBoss 1.0

#define AIBehaviorChase 0       // Persigue al jugador
#define AIBehaviorShootAndRun 1 // Dispara y se aleja
#define AIBehaviorKamikaze 2    // Va directo a chocar
#define AIBehaviorBomber 3      // Bombardea al suelo

// Patrones de disparo
#define SpreadTypeNormal 0
#define SpreadTypeCircle 1  // 8 balas en círculo
#define SpreadTypeCross 2   // 4 balas en cruz
#define SpreadTypeWall 3    // Muro de balas con espacios
#define SpreadTypeShotgun 4 // Grupo de balas con dispersión
#define SpreadTypeSpiral 5
#define SpreadTypeSingle 6

#define EnemyHoverFrameWidth 114
#define EnemyHoverFrameHeight 80

#define EnemyBossFrameWidth 320
#define EnemyBossFrameHeight 240

#define EnemyKamikazeFrameWidth 96
#define EnemyKamikazeFrameHeight 144


// ---- DIMENSIONES DEL MUNDO ----
#define WorldWidth 2048
#define WorldHeight 2048
#define ScreenWidth 640
#define ScreenHeight 360
#define ScreenCenterX (ScreenWidth / 2)
#define ScreenCenterY (ScreenHeight / 2)

// ---- CONFIGURACIÓN DEL AVIÓN ----
#define NumAirplaneFrames 2
#define AirplaneAnimSpeed 10
#define AirplaneMaxHealth 100
#define RotationSpeed 0.05
#define MovementSpeed 3.0
#define AscendSpeed 0.005
#define DescentSpeed 0.005
#define MaxScale 0.6
#define MinScale 0.2
#define AirplaneShadowOffset 60     // Offset máximo de la sombra (cuando está en altura máxima)
#define AirplaneShadowBaseOffset 10 // Offset mínimo de la sombra
#define MaxFuel 1000
#define FuelConsumption 0.5
#define RefuelRate 2.0
#define InitialScale 0.4
#define LandingScale 0.4
#define AirplaneMaxAmmo 100
#define AirplaneFireRate 0.1
#define AirplaneReloadRate 2.0
#define AirplaneBulletRange 500.0
#define AirplaneBulletSpeed 8.0
#define AirplaneBulletDamage 25.0

// ---- ÁREA DE ATERRIZAJE ----
#define LandingAreaWidth 77
#define LandingAreaHeight 255

// ---- PORTAAVIONES ----
#define StartingX (WorldWidth / 2)
#define StartingY (WorldHeight / 2)
#define CarrierWidth 75
#define CarrierHeight 250
#define LandingDistance 80

// ---- ISLAS ----
#define MaxIslands 9
#define MinIslandSize 100
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

// ---- EVENTOS DEL JUEGO ----
#define StartGame 0
#define TurretDestroyed 1
#define SpawnFlyingEnemies 2
#define SpawnBoss 3
#define LowFuel 4

// ---- COLORES ----
#define BackgroundColor 0xFF000000
#define TextColor 0xFFFFFFFF
#define RedColor 0xFF0000FF
#define SeaColor 0xFF805020
#define ShadowColor 0x80000000
#define IslandColor 0xFF604020
#define GreenColor 0xFF00FF00

// ---- UI ----
#define BlinkRate 30
#define HealthFlashTime 0.5

#define UIDialogFrameWidth 552
#define UIDialogFrameHeight 124
#define UIDialogPortraitSize 100
#define ObjectiveY 80       // Posición Y donde empezarán a mostrarse los objetivos
#define ObjectiveSpacing 20 // Espacio entre cada línea de objetivo

#define MinimapTileSize 3

#endif