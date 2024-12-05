// dialogtexts.h
#ifndef DIALOGTEXTS_H
#define DIALOGTEXTS_H

// 256 caracteres es mas de lo que cabe en la ventana
typedef int[ 256 ] DialogText;

// todos los textos del juego
extern DialogText DT_StartOperation;
extern DialogText DT_StartOperationReply;
extern DialogText DT_TurretDestroyed;
extern DialogText DT_EnemyDetected;
extern DialogText DT_EnemyPenetration;
extern DialogText DT_EnemyFighters;
extern DialogText DT_EnemyFightersReply;
extern DialogText DT_AttackHeli;
extern DialogText DT_GoHome;
extern DialogText DT_TurretsRemain;
extern DialogText DT_TurretsRemainReply;
extern DialogText DT_MaxAlert;
extern DialogText DT_MaxAlertReply;
extern DialogText DT_DarkPhoenix;
extern DialogText DT_DarkPhoenix_Reply;

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