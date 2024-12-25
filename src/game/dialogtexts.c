// dialogtexts.c
#include "dialogtexts.h"

DialogWindow DW_StartOperation =
{
    RegionPortraitCommander,
    {
        "Liutenant Rykov, start operation 'Nemesis'. \n"
        "Your mission is clear: \n"
        "- Eliminate the defense turrets \n"
        "The Black Hand must not activate its \n"
        "prototype. Good luck.",
        
        "Teniente Rykov, inicie operaci" o_ "n 'N" e_ "mesis' \n"
        "Su misi" o_ "n es clara: \n"
        "- Elimine las torretas de defensa \n"
        "La Mano Negra no debe activar su proto- \n"
        "tipo. Buena suerte."
    }
};
    
DialogWindow DW_StartOperationReply =
{
    RegionPortraitPlayer,
    {
        "Understood, commander. Entering hostile \n"
        "zone. It is time to show them who is in \n"
        "control of the sky here",
        
        "Entendido, comandante. Entrando en zona \n"
        "hostil. Es hora de demostrarles qui" e_ "n \n"
        "domina estos cielos."
    }
};

DialogWindow DW_FleeIsland =
{
    RegionPortraitCommander,
    {
        "Rykov, you can't leave the island yet! \n"
        "Defeat the enemy soldiers or else they \n"
        "will repair the turret.",
        
        exclam "Rykov, a" u_ "n no puede dejar la isla! \n"
        "Si no vence a los soldados enemigos \n"
        "reparar" a_ "n la torreta."
    }
};

DialogWindow DW_FleeIslandReply =
{
    RegionPortraitPlayer,
    {
        "I'm sorry, commander. You are right...",
        
        "Disculpe, comandante. Tiene raz" o_ "n..."
    }
};

DialogWindow DW_TurretDestroyed =
{
    RegionPortraitPlayer,
    {
        "First turret destroyed. This is going to \n"
        "be a piece of cake... or is it?",
        
        "Primera torreta destruida. Esto va a ser \n"
        "pan comido... " interr "o no?"
    }
};
   
DialogWindow DW_EnemyDetected =
{
    RegionPortraitCommander,
    {
        "Good job, Rykov. But be careful, we just \n"
        "detected enemy activity. Get ready to \n"
        "meet some resistance",
        
        "Buen trabajo, Rykov. Pero cuidado, hemos \n"
        "detectado actividad enemiga. Prep" a_ "rese \n"
        "para recibir resistencia."
    }
};
   
DialogWindow DW_EnemyPenetration =
{
    RegionPortraitSoldier,
    {
        "The enemy has penetrated our defenses! \n"
        "Don't let them advance!",
        
        exclam "El enemigo ha penetrado las defensas! \n"
        exclam "No dejen que avance!"
    }
};
    
DialogWindow DW_EnemyFighters =
{
    RegionPortraitCommander,
    {
        "Attention, Rykov. We have enemy fighters \n"
        "coming out of the air base. Prepare \n"
        "for intense combat!",
        
        "Atenci" o_ "n, Rykov. Tenemos cazas enemigos \n"
        "saliendo de la base a" e_ "rea. \n"
        exclam "Prep" a_ "rese para un combate intenso!"
    }
};
    
DialogWindow DW_EnemyFightersReply =
{
    RegionPortraitPlayer,
    {
        "I knew it, they couldn't stay still for \n"
        "too long. Come and test my accuracy",
        
        "Lo sab" i_ "a, no pod" i_ "an quedarse quietos por \n"
        "mucho tiempo. Vamos, pongan a prueba mi \n"
        "punter" i_ "a."
    }
};
    
DialogWindow DW_AttackHeli =
{
    RegionPortraitSoldier,
    {
        "Fighter Alpha 1 in position! Objective is \n"
        "within sight, all fighters: \n"
        "attack the helicopter!",
        
        exclam "Caza Alpha 1 en posici" o_ "n! Objetivo \n"
        "visualizado, todos los cazas: \n"
        exclam "ataquen al helic" o_ "ptero!"
    }
};
    
DialogWindow DW_GoHome =
{
    RegionPortraitCommander,
    {
        "That's all guys. Go back home... \n"
        "or what's left of it",
        
        "Eso es todo, chicos. Vuelvan a casa... \n"
        "o lo que quede de ella"
    }
};
    
DialogWindow DW_TurretsRemain =
{
    RegionPortraitCommander,
    {
        "Good job, Rykov. But don't let your \n"
        "guard down. The remaining turrets are \n"
        "still active and the enemy will not \n"
        "surrender without a fight",
        
        "Buen trabajo, Rykov. Pero no baje la \n"
        "guardia. Las torretas restantes todav" i_ "a \n"
        "est" a_ "n activas y el enemigo no se \n"
        "rendir" a_ " sin pelear"
    }
};
    
DialogWindow DW_TurretsRemainReply =
{
    RegionPortraitPlayer,
    {
        "They never do. \n"
        "Let's get this over with.",
        
        "Nunca lo hacen. \n"
        "Vamos a terminar con esto."
    }
};
    
DialogWindow DW_MaxAlert =
{
    RegionPortraitCommander,
    {
        "Maximum alert, Rykov! The prototype is \n"
        "airborne. I repeat: the prototype has \n"
        "just taken off!",
        
        exclam "Alerta m" a_ "xima, Rykov! El prototipo est" a_ " \n"
        "en el aire. Repito: " exclam "el prototipo ha \n"
        "despegado!"
    }
};
    
DialogWindow DW_MaxAlertReply =
{
    RegionPortraitPlayer,
    {
        "So here it is... the great monster that \n"
        "everyone was afraid of. I hope you are \n"
        "worth it. \n",
        
        "As" i_ " que aqu" i_ " est" a_ "s... el gran monstruo \n"
        "que todos tem" i_ "an. Espero que valgas la \n"
        "pena."
    }
};
    
DialogWindow DW_DarkPhoenix =
{
    RegionPortraitBoss,
    {
        "This is Captain Kross, pilot of the Dark \n"
        "Phoenix. This will be your last flight, \n"
        "intruder",
        
        "Aqu" i_ " Capit" a_ "n Kross, piloto del F" e_ "nix \n"
        "Oscuro. Este ser" a_ " tu " u_ "ltimo vuelo, \n"
        "intruso."
    }
};
    
DialogWindow DW_DarkPhoenix_Reply =
{
    RegionPortraitPlayer,
    {
        "We'll see about that, Kross. Only one \n"
        "pilot will survive this, and it won't \n"
        "be you",
        
        "Eso ya lo veremos, Kross. S" o_ "lo un piloto \n"
        "saldr" a_ " vivo de esto, y no ser" a_ "s t" u_
    }
};

DialogWindow DW_FuelHalf =
{
    RegionPortraitCommander,
    {
        "Rykov, attention. You are reaching 50% of \n"
        "your fuel reserve. Remember that the \n"
        "carrier is there to supply you with fuel \n"
        "and ammunition. Don't risk running out of \n"
        "resources!",
        
        "Rykov, atenci" o_ "n. Est" a_ "s alcanzando el \n"
        "50% de tu combustible. Recuerda que el \n"
        "portaaviones est" a_ " disponible para \n"
        "reabastecerte de gasolina y munici" o_ "n. \n"
        exclam "No te arriesgues a quedarte sin recursos!"
    }
};
    
DialogWindow DW_FuelHalfReply =
{
    RegionPortraitPlayer,
    {
        "Understood, commander. I will make a stop \n"
        "as soon as I need. But for now I still \n"
        "have enough to create some chaos before \n"
        "going back.",
        
        "Entendido, comandante. Har" e_ " una \n"
        "parada en cuanto lo necesite. Pero a" u_ "n \n"
        "tengo suficiente para causar algo de \n"
        "caos antes de volver."
    }
};

DialogWindow DW_GameOver =
{
    RegionPortraitCommander,
    {
        "Rykov... Rykov, do you copy?? \n"
        "...Rykov!! \n"
        "...!? Nooooo!!",
        
        "Rykov... " interr "Rykov, me copias? " interr "\n"
        "..." interr "Rykov!" exclam "\n"
        "..." interr exclam "Nooooo!" exclam
    }
};

DialogWindow DW_GameRestart =
{
    RegionPortraitPlayer,
    {
        "Commander, Rykov here. Somehow I was able \n"
        "to recover and come back. But the enemy \n"
        "was able to fix the turrets in that time. ",
        
        "Comandante, aqu" i_ " Rykov. De alg" u_ "n modo he \n"
        "podido recuperar y volver. Pero el ene- \n"
        "migo ha podido arreglar las torretas en \n"
        "ese tiempo."
    }
};

DialogWindow DW_GameRestartReply =
{
    RegionPortraitCommander,
    {
        "Rykov! Glad to see you are still there. \n"
        "Understood, let's try again from scratch.",
        
        exclam "Rykov! Me alegra saber que sigue ah" i_ ". \n"
        "Entendido, empecemos de nuevo desde cero."
    }
};
