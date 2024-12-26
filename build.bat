@echo off
echo Creating directories if they don't exist
if not exist "obj" mkdir obj
if not exist "bin" mkdir bin

set DEVTOOLS=.\dev
set COMPILER=%DEVTOOLS%\compile.exe
set ASSEMBLER=%DEVTOOLS%\assemble.exe
set PACKER=%DEVTOOLS%\packrom.exe
set PNG2VIRCON=%DEVTOOLS%\png2vircon.exe
set WAV2VIRCON=%DEVTOOLS%\wav2vircon.exe

echo Convert PNG textures
echo --------------------------
%PNG2VIRCON% assets/menu.png -o obj/menu.vtx || goto :failed
%PNG2VIRCON% assets/heli.png -o obj/heli.vtx || goto :failed
%PNG2VIRCON% assets/carrier.png -o obj/carrier.vtx || goto :failed
%PNG2VIRCON% assets/tileset.png -o obj/tileset.vtx || goto :failed
%PNG2VIRCON% assets/bullet.png -o obj/bullet.vtx || goto :failed
%PNG2VIRCON% assets/soldier.png -o obj/soldier.vtx || goto :failed
%PNG2VIRCON% assets/base.png -o obj/base.vtx || goto :failed
%PNG2VIRCON% assets/machinegun.png -o obj/machinegun.vtx || goto :failed
%PNG2VIRCON% assets/enemyBullet.png -o obj/enemyBullet.vtx || goto :failed
%PNG2VIRCON% assets/hoverEnemy.png -o obj/hoverEnemy.vtx || goto :failed
%PNG2VIRCON% assets/kamikaze.png -o obj/kamikaze.vtx || goto :failed
%PNG2VIRCON% assets/boss.png -o obj/boss.vtx || goto :failed
%PNG2VIRCON% assets/portraits.png -o obj/portraits.vtx || goto :failed
%PNG2VIRCON% assets/dialog.png -o obj/dialog.vtx || goto :failed
%PNG2VIRCON% assets/miniMap.png -o obj/miniMap.vtx || goto :failed
%PNG2VIRCON% assets/pickups.png -o obj/pickups.vtx || goto :failed
%PNG2VIRCON% assets/enemySoldier.png -o obj/enemySoldier.vtx || goto :failed
%PNG2VIRCON% assets/soldierDeath.png -o obj/soldierDeath.vtx || goto :failed
%PNG2VIRCON% assets/soldierBlood.png -o obj/soldierBlood.vtx || goto :failed
%PNG2VIRCON% assets/enemyHelicopter.png -o obj/enemyHelicopter.vtx || goto :failed
%PNG2VIRCON% assets/gui.png -o obj/gui.vtx || goto :failed
%PNG2VIRCON% assets/ending.png -o obj/ending.vtx || goto :failed
%PNG2VIRCON% assets/explosion.png -o obj/explosion.vtx || goto :failed
%PNG2VIRCON% assets/splash.png -o obj/splash.vtx || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
%WAV2VIRCON% assets\music\menu.wav -o obj\music-menu.vsnd || goto :failed
%WAV2VIRCON% assets\music\turrets.wav -o obj\music-turrets.vsnd || goto :failed
%WAV2VIRCON% assets\music\planes.wav -o obj\music-planes.vsnd || goto :failed
%WAV2VIRCON% assets\music\boss.wav -o obj\music-boss.vsnd || goto :failed
%WAV2VIRCON% assets\music\ending.wav -o obj\music-ending.vsnd || goto :failed

%WAV2VIRCON% assets\sounds\hurt.wav -o obj\hurt.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\death.wav -o obj\death.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\fall.wav -o obj\fall.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\pickup.wav -o obj\pickup.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\changeweapon.wav -o obj\changeweapon.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\shoot.wav -o obj\shoot.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\heli.wav -o obj\heli.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\enterheli.wav -o obj\enterheli.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\lowfuel.wav -o obj\lowfuel.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\bombbeep.wav -o obj\bombbeep.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\explosion.wav -o obj\explosion.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\splash.wav -o obj\splash.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\enemyhit.wav -o obj\enemyhit.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\enemyshoot.wav -o obj\enemyshoot.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\menumove.wav -o obj\menumove.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\menuaccept.wav -o obj\menuaccept.vsnd || goto :failed
%WAV2VIRCON% assets\sounds\actioncancelled.wav -o obj\actioncancelled.vsnd || goto :failed

echo Compile the C code
echo --------------------------
%COMPILER% src/main.c -o obj/main.asm || goto :failed

echo Assemble the ASM code
echo --------------------------
%ASSEMBLER% obj/main.asm -o obj/main.vbin || goto :failed

echo Pack the ROM
echo --------------------------
%PACKER% RomDefinition.xml -o bin/OceanStorm.v32 || goto :failed
goto :succeeded

:failed
echo BUILD FAILED
exit /b %errorlevel%

:succeeded
echo BUILD SUCCESSFUL
echo.
echo ROM created at bin/OceanStorm.v32
pause
exit /b

@echo on