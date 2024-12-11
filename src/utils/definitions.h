#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Incluir librerias aquí para que las puedan
// usar todos los demás archivos del proyecto
#include "video.h"
#include "math.h"
#include "time.h"
#include "string.h"
#include "misc.h"
#include "input.h"
#include "../libraries/vector2d.h"
#include "../libraries/tilemap.h"

// ---- TEXTURAS ----
enum Textures {
    TextureHeli,
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
    TexturePortraits,
    TextureDialog,
    TextureMinimap,
};

// ---- REGIONES ----
#define RegionHeli 0
#define RegionCarrier 0
#define RegionIsland 0
#define RegionSoldier 0
#define RegionTurretBase 0
#define RegionTurretGun 1

enum RegionsPortraits {
    RegionPortraitPlayer,
    RegionPortraitCommander,
    RegionPortraitSoldier,
    RegionPortraitBoss
};

enum RegionsMinimap {
    RegionMapFrame,
    RegionMapSmallIsland,
    RegionMapLargeIsland,
    RegionMapCarrier,
    RegionMapHeli
};

// ---- SISTEMA DE TILES ----
#define TileSize 64
#define MaxTilesX 6
#define MaxTilesY 6
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

// ---- CONFIGURACIÓN DEL AVIÓN ----
#define HeliSpriteWidth 128
#define HeliSpriteHeight 91
#define HeliFrameWidth 128
#define HeliFrameHeight 91
#define HeliFramesPerRow 1
#define NumHeliFrames 2
#define HeliAnimSpeed 10
#define HeliMaxHealth 100
#define RotationSpeed 0.05
#define MovementSpeed 3.0
#define AscendSpeed 0.005
#define DescentSpeed 0.005
#define MaxScale 0.6
#define MinScale 0.2
#define HeliShadowOffset 40
#define HeliShadowBaseOffset 10
#define MaxFuel 1000
#define FuelConsumption 0.5
#define RefuelRate 2.0
#define InitialScale 0.4
#define LandingScale 0.4
#define HeliMaxAmmo 100
#define HeliFireRate 0.1
#define HeliReloadRate 2.0
#define HeliBulletRange 500.0
#define HeliBulletSpeed 8.0
#define HeliBulletDamage 25.0

// ---- CONFIGURACIÓN DEL SOLDADO ----
#define SoldierWidth 32
#define SoldierHeight 32
#define PistolDamage 25
#define ShotgunDamage 15
#define SubmachineGunDamage 10
#define SoldierMaxHealth 100
#define SoldierBarWidth 50
#define SoldierBarHeight 5
#define SoldierBarOffsetY 40
#define SoldierSwimSpeed (SoldierSpeed * 0.5)
#define MaxStamina 50
#define StaminaDrainRate 10
#define SoldierDrownScale 0.3

// ---- CONFIGURACIÓN DE TORRETAS TERRESTRES ----
#define MaxTurrets 9
#define TurretBaseWidth 32
#define TurretBaseHeight 32
#define TurretGunWidth 32
#define TurretGunHeight 38
#define TurretVisionRange 300.0
#define TurretFireRate 10.0
#define TurretBulletSpeed 8.0
#define TurretBulletRange 400.0
#define TurretMinSpread 3
#define TurretMaxSpread 6
#define TurretDamagePerBullet 25

// ---- CONFIGURACIÓN DE ENEMIGOS ----
#define MaxEnemies 20

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

// Velocidades
#define EnemySpeedNormal 3.0
#define EnemySpeedKamikaze 2.0
#define EnemySpeedBoss 1.0
#define KamikazeBulletSpeed 4.0
#define BossBulletSpeed 4.5
#define NormalEnemyBulletSpeed 4.0

// Comportamientos IA
#define AIBehaviorChase 0
#define AIBehaviorShootAndRun 1
#define AIBehaviorKamikaze 2
#define AIBehaviorBomber 3

// Patrones de disparo
#define SpreadTypeNormal 0
#define SpreadTypeCircle 1
#define SpreadTypeCross 2
#define SpreadTypeWall 3
#define SpreadTypeShotgun 4
#define SpreadTypeSpiral 5
#define SpreadTypeSingle 6

// ---- DIMENSIONES DE SPRITES ENEMIGOS ----
#define EnemyHoverFrameWidth 114
#define EnemyHoverFrameHeight 80
#define EnemyBossFrameWidth 320
#define EnemyBossFrameHeight 240
#define EnemyKamikazeFrameWidth 96
#define EnemyKamikazeFrameHeight 144

// ---- CONFIGURACIÓN DE SOLDADOS ENEMIGOS ----
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

// ---- CONFIGURACIÓN DEL JEFE ----
#define BossPhase1Health 400
#define BossPhase2Health 200
#define BossPhase1FireRate 0.2
#define BossPhase2FireRate 0.5
#define BossPhase3FireRate 0.1

// ---- DIMENSIONES DEL MUNDO Y PANTALLA ----
#define WorldTilesX 32
#define WorldTilesY 32
#define WorldWidth  (WorldTilesX * TileSize) // 2048
#define WorldHeight (WorldTilesY * TileSize) // 2048
#define ScreenWidth 640
#define ScreenHeight 360
#define ScreenCenterX (ScreenWidth / 2)
#define ScreenCenterY (ScreenHeight / 2)

// ---- SISTEMA DE PROYECTILES ----
#define MaxBullets 50
#define BulletSpeed 5.0
#define BulletSize 8

// ---- SISTEMA DE BOMBAS ----
#define MaxActiveBombs 5
#define BombTimerSeconds 5.0
#define BombExplosionRadius 50.0

// ---- PORTAAVIONES ----
#define StartingX (WorldWidth / 2)
#define StartingY (WorldHeight / 2)
#define CarrierWidth 75
#define CarrierHeight 250
#define LandingDistance 80
#define LandingAreaWidth 77
#define LandingAreaHeight 255

// ---- ISLAS ----
#define MaxIslands 9
#define MinIslandSize 100
#define MaxIslandSize 192
#define NumIslandVariants 4

// ---- EVENTOS Y ESTADOS ----
#define MaxEvents 10
#define EventFirstExit 0
#define EventFirstTurret 1
#define EventFirstPickup 2

#define StartGame 0
#define TurretDestroyed 1
#define SpawnFlyingEnemies 2
#define SpawnBoss 3
#define LowFuel 4

#define StateMenu 0
#define StateGame 1
#define StateGameOver 2

// ---- INTERFAZ Y UI ----
#define UIDialogFrameWidth 552
#define UIDialogFrameHeight 124
#define UIDialogPortraitSize 100
#define ObjectiveY 80
#define ObjectiveSpacing 20
#define MinimapTileSize 3
#define BlinkRate 30
#define HealthFlashTime 0.5
#define ReloadTime 2.0
#define ReloadTextOffset 30

// ---- CÁMARA ----
#define CameraZoomAir 1.0
#define CameraZoomGround 0.5
#define CameraZoomSpeed 0.05

// ---- COLORES ----
#define BackgroundColor 0xFF000000
#define TextColor 0xFFFFFFFF
#define RedColor 0xFF0000FF
#define SeaColor 0xFF805020
#define ShadowColor 0x80000000
#define IslandColor 0xFF604020
#define GreenColor 0xFF00FF00

#endif