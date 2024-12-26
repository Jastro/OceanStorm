// dialogtexts.h
#ifndef DIALOGTEXTS_H
#define DIALOGTEXTS_H

// 256 caracteres es mas de lo que cabe en la ventana
typedef int[ 256 ] DialogText;

struct DialogWindow
{
    int portrait_region;
    DialogText[ 2 ] texts;
};

// todos los textos del juego
extern DialogWindow DW_StartOperation;
extern DialogWindow DW_StartOperationReply;
extern DialogWindow DW_FleeIsland;
extern DialogWindow DW_FleeIslandReply;
extern DialogWindow DW_TurretDestroyed;
extern DialogWindow DW_EnemyDetected;
extern DialogWindow DW_EnemyPenetration;
extern DialogWindow DW_EnemyFighters;
extern DialogWindow DW_EnemyFightersReply;
extern DialogWindow DW_AttackHeli;
extern DialogWindow DW_GoHome;
extern DialogWindow DW_TurretsRemain;
extern DialogWindow DW_TurretsRemainReply;
extern DialogWindow DW_MaxAlert;
extern DialogWindow DW_MaxAlertReply;
extern DialogWindow DW_DarkPhoenix;
extern DialogWindow DW_DarkPhoenix_Reply;
extern DialogWindow DW_FuelHalf;
extern DialogWindow DW_FuelHalfReply;
extern DialogWindow DW_GameOver;
extern DialogWindow DW_GameRestart;
extern DialogWindow DW_GameRestartReply;

// para facilitar los caracteres en español
#define a_      "\xE1"   // á
#define e_      "\xE9"   // é
#define i_      "\xED"   // í
#define o_      "\xF3"   // ó
#define u_      "\xFA"   // ú
#define A_      "\xC1"   // Á
#define E_      "\xC9"   // É
#define I_      "\xCD"   // Í
#define O_      "\xD3"   // Ó
#define U_      "\xDA"   // Ú
#define n_      "\xF1"   // ñ
#define N_      "\xD1"   // Ñ
#define u_diere "\xFC"   // ü
#define U_diere "\xDC"   // Ü
#define exclam  "\xA1"   // ¡
#define interr  "\xBF"   // ¿
#define n_avo   "\xBA"   // º
#define n_ava   "\xAA"   // ª

#endif