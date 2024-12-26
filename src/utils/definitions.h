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
enum Textures
{
    TextureMenu,
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
    TexturePickups,
    TextureEnemySoldier,
    TextureEnemySoldierDeath,
    TextureEnemySoldierBlood,
    TextureEnemyHelicopter,
    TextureGui,
    TextureEnding,
    TextureExplosionFX,
    TextureSplashFX,
};

// ---- REGIONES ----
#define RegionHeli 0
#define RegionCarrier 0
#define RegionIsland 0
#define RegionSoldier 0
#define RegionTurretBase 0
#define RegionTurretGun 1

enum RegionsPortraits
{
    RegionPortraitPlayer,
    RegionPortraitCommander,
    RegionPortraitSoldier,
    RegionPortraitBoss
};

enum RegionsMinimap
{
    RegionMapFrame,
    RegionMapSmallIsland,
    RegionMapLargeIsland,
    RegionMapCarrier,
    RegionMapHeli,
    RegionMapSoldier,
    RegionMapEnemyPlane
};

enum RegionsMenu
{
    RegionMenuBackground,
    RegionMenuLogo,
    RegionMenuCursor,
    RegionMenuStartENG,
    RegionMenuOptionsENG,
    RegionMenuStartSPA,
    RegionMenuOptionsSPA,
    RegionMenuLanguageENG,
    RegionMenuLanguageSPA,
    RegionMenuEnglishENG,
    RegionMenuSpanishENG,
    RegionMenuEnglishSPA,
    RegionMenuSpanishSPA,
    RegionMenuNoBloodENG,
    RegionMenuNoBloodSPA
};

enum RegionsGui
{
    RegionHeliFuel,
    RegionHeliHealth,
    RegionHeliAmmo,
    RegionSoldierAmmo,
    RegionSoldierBombs,
    RegionSoldierArmor,
    RegionObjectiveTurrets,
    RegionObjectivePlanes,
    RegionObjectiveBoss,
    RegionEnterSign,
    RegionExitSign,
    RegionSoldierHealth,
    RegionSoldierReloading,
    RegionBombSign,
    RegionBomb,
    RegionCannonArrow
};

// ---- IDIOMAS ----
enum GameLanguages
{
    LanguageEnglish,
    LanguageSpanish
};

// ---- SISTEMA DE TILES ----
#define TileSize 64
#define MaxTilesX 9
#define MaxTilesY 9
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
#define HeliMaxHealth 200
#define RotationSpeed 0.05
#define MovementSpeed 3.0
#define AscendSpeed 0.005
#define DescentSpeed 0.005
#define MaxScale 0.6
#define MinScale 0.2
#define HeliShadowOffset 40
#define HeliShadowBaseOffset 10
#define MaxFuel 1500
#define LowFuel (MaxFuel * 0.25)
#define FuelConsumption 0.5
#define RefuelRate 3.0
#define InitialScale 0.5
#define LandingScale 0.5
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
#define TurretVisionRange 270.0
#define TurretFireRate 6.0
#define TurretBulletSpeed 5.0
#define TurretBulletRange 350.0
#define TurretMinSpread 3
#define TurretMaxSpread 6
#define TurretDamagePerBullet 25

// ---- CONFIGURACIÓN DE ENEMIGOS ----
#define MaxEnemies 20

// Tipos de enemigos
enum EnemyTypes
{
    EnemyTypeNormal,
    EnemyTypeKamikaze,
    EnemyTypePlane,
    EnemyTypeBoss,
    EnemyTypeSoldier
};


// Puntos de vida
#define EnemyHealthNormal 1000
#define EnemyHealthKamikaze 50
#define EnemyHealthPlane 500
#define EnemyHealthBoss 10000

// Características de enemigos
#define EnemyBulletRange 300.0
#define EnemyBlinkTime 0.1
#define KamikazeDamage 25
#define SoldierStateSpawning 4
#define SpawnAnimationTime 0.5
#define SpawnBlinkRate 4

// Velocidades
#define EnemySpeedNormal 3.0
#define EnemySpeedKamikaze 4.0
#define EnemySpeedBoss 2.0
#define EnemySpeedPlane 3.0
#define KamikazeBulletSpeed 4.0
#define PlaneBulletSpeed 3.0
#define BossBulletSpeed 4.5
#define NormalEnemyBulletSpeed 2.5

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
#define EnemyHoverFrameWidth 228
#define EnemyHoverFrameHeight 160
#define EnemyBossFrameWidth 320
#define EnemyBossFrameHeight 240
#define EnemyKamikazeFrameWidth 96
#define EnemyKamikazeFrameHeight 144
#define EnemyHelicopterFrameWidth 96
#define EnemyHelicopterFrameHeight 96

// ---- CONFIGURACIÓN DE SOLDADOS ENEMIGOS ----
#define MinSoldiersPerTurret 2
#define MaxSoldiersPerTurret 5
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

// ---- CONFIGURACIÓN DE CADAVERES ENEMIGOS ----
#define MaxCorpses 40
#define CorpseFrames 4
#define CorpseFrameTime 0.2 // Tiempo por frame de animación

// Estados del cadáver
enum CorpseStates
{
    CorpseStateAnimating,  // Reproduciendo animación de muerte
    CorpseStateStatic,     // Cadáver en el suelo con charco
    CorpseStateHidden      // Fuera de pantalla (pero guardamos posición)
};

// ---- CONFIGURACIÓN DEL JEFE ----
enum BossPhases
{
    BossPhaseOne,
    BossPhaseTwo,
    BossPhaseThree
};

#define BossHealth1 2000
#define BossHealth2 1500
#define BossHealth3 1000

#define BossSpeed1 1.0   // Fase 1: más lento pero metódico
#define BossSpeed2 1.5   // Fase 2: velocidad media
#define BossSpeed3 2.0   // Fase 3: muy rápido y agresivo

// ---- SISTEMA DE PICKUPS ----
#define MaxPickups 20      // Máximo número de pickups activos
#define PickupShotgun 0    // Frame 0 - Escopeta
#define PickupSubmachine 1 // Frame 1 - Metralleta
#define PickupArmor 2      // Frame 2 - Armadura
#define PickupHealth 3     // Frame 3 - Vida

// ---- DIMENSIONES DEL MUNDO Y PANTALLA ----
#define WorldTilesX 48
#define WorldTilesY 48
#define WorldWidth (WorldTilesX * TileSize)  // 3072
#define WorldHeight (WorldTilesY * TileSize) // 3072
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
#define BombTimerSeconds 3.0
#define BombExplosionRadius 50.0

// ---- PORTAAVIONES ----
#define StartingX (WorldWidth / 2)
#define StartingY (WorldHeight / 2)
#define CarrierWidth 75
#define CarrierHeight 250
#define LandingDistance 80
#define LandingAreaWidth 77
#define LandingAreaHeight 255

#define MaxFX 20
#define FXFrameTime 0.05

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

enum TYPE_FX {
    Explosion,
    Splash
};

enum Events
{
    StartGame,
    FleeIsland,
    TurretDestroyed,
    SpawnFlyingEnemies,
    FlyingEnemiesDestroyed,
    SpawnBoss,
    FuelWarning,
    GameOver,
    GameRestart
};

enum GameScenes
{
    SceneMenu,
    SceneGame,
    SceneGameOver,
    SceneBossExplosion,
    SceneEnding
};

// ---- INTERFAZ Y UI ----
#define UIDialogFrameWidth 552
#define UIDialogFrameHeight 124
#define UIDialogPortraitSize 100
#define ObjectiveY 80
#define ObjectiveSpacing 20
#define MinimapTileSize 2
#define BlinkRate 30
#define HealthFlashTime 0.5
#define ReloadTime 2.0
#define ReloadTextOffset 27
#define ShakeTimeOnDamage 0.25  // Duración del temblor en segundos
#define ShakeIntensity 15.0    // Amplitud máxima del temblor en píxeles

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

// ---- SOUND CHANNELS----

#define ChannelHeli   13
#define ChannelOnOff  14
#define ChannelMusic  15

enum GameSounds
{
    // Separamos: primero las musicas
    MusicMenu,
    MusicTurrets,
    MusicPlanes,
    MusicBoss,
    MusicEnding,
    
    // y luego los sonidos
    SoundHurt,
    SoundDeath,
    SoundFall,
    SoundPickup,
    SoundChangeWeapon,
    SoundShoot,
    SoundHeli,
    SoundEnterHeli,
    SoundLowFuel,
    SoundBombBeep,
    SoundExplosion,
    SoundSplash,
    SoundEnemyHit,
    SoundEnemyShoot,
    SoundMenuMove,
    SoundMenuAccept,
    SoundActionCancelled
};

#endif