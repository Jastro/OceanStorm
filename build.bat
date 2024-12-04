@echo off
echo Creating directories if they don't exist
if not exist "obj" mkdir obj
if not exist "bin" mkdir bin

set DEVTOOLS=.\dev
set COMPILER=%DEVTOOLS%\compile.exe
set ASSEMBLER=%DEVTOOLS%\assemble.exe
set PACKER=%DEVTOOLS%\packrom.exe
set PNG2VIRCON=%DEVTOOLS%\png2vircon.exe

echo Convert PNG textures
echo --------------------------
%PNG2VIRCON% assets/airplane.png -o obj/airplane.vtx || goto :failed
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
%PNG2VIRCON% assets/player.png -o obj/player.vtx || goto :failed
%PNG2VIRCON% assets/commander.png -o obj/commander.vtx || goto :failed
%PNG2VIRCON% assets/enemySoldier.png -o obj/enemySoldier.vtx || goto :failed
%PNG2VIRCON% assets/enemyBoss.png -o obj/enemyBoss.vtx || goto :failed
%PNG2VIRCON% assets/dialog.png -o obj/dialog.vtx || goto :failed

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