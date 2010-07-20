/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ScriptData
SDName: Dustwallow_Marsh
SD%Complete: 95
SDComment: Quest support: 11180, 558, 11126. Vendor Nat Pagle
SDCategory: Dustwallow Marsh
EndScriptData */

/* ContentData
mobs_risen_husk_spirit
npc_restless_apparition
npc_deserter_agitator
npc_lady_jaina_proudmoore
npc_nat_pagle
EndContentData */

#include "precompiled.h"

/*######
## mobs_risen_husk_spirit
######*/

#define SPELL_SUMMON_RESTLESS_APPARITION    42511
#define SPELL_CONSUME_FLESH                 37933           //Risen Husk
#define SPELL_INTANGIBLE_PRESENCE           43127           //Risen Spirit

struct TRINITY_DLL_DECL mobs_risen_husk_spiritAI : public ScriptedAI
{
    mobs_risen_husk_spiritAI(Creature *c) : ScriptedAI(c) {}

    uint32 ConsumeFlesh_Timer;
    uint32 IntangiblePresence_Timer;

    void Reset()
    {
        ConsumeFlesh_Timer = 10000;
        IntangiblePresence_Timer = 5000;
    }

    void Aggro(Unit* who) { }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if( done_by->GetTypeId() == TYPEID_PLAYER )
            if( damage >= m_creature->GetHealth() && ((Player*)done_by)->GetQuestStatus(11180) == QUEST_STATUS_INCOMPLETE )
                m_creature->CastSpell(done_by,SPELL_SUMMON_RESTLESS_APPARITION,false);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if( ConsumeFlesh_Timer < diff )
        {
            if( m_creature->GetEntry() == 23555 )
                DoCast(m_creature->getVictim(),SPELL_CONSUME_FLESH);
            ConsumeFlesh_Timer = 15000;
        } else ConsumeFlesh_Timer -= diff;

        if( IntangiblePresence_Timer < diff )
        {
            if( m_creature->GetEntry() == 23554 )
                DoCast(m_creature->getVictim(),SPELL_INTANGIBLE_PRESENCE);
            IntangiblePresence_Timer = 20000;
        } else IntangiblePresence_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mobs_risen_husk_spirit(Creature *_Creature)
{
    return new mobs_risen_husk_spiritAI (_Creature);
}

/*######
## npc_restless_apparition
######*/

bool GossipHello_npc_restless_apparition(Player *player, Creature *_Creature)
{
    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    player->TalkedToCreature(_Creature->GetEntry(), _Creature->GetGUID());
    _Creature->SetInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

    return true;
}

/*######
## npc_deserter_agitator
######*/

struct TRINITY_DLL_DECL npc_deserter_agitatorAI : public ScriptedAI
{
    npc_deserter_agitatorAI(Creature *c) : ScriptedAI(c) {}

    void Reset()
    {
        m_creature->setFaction(894);
    }

    void Aggro(Unit* who) {}
};

CreatureAI* GetAI_npc_deserter_agitator(Creature *_Creature)
{
    return new npc_deserter_agitatorAI (_Creature);
}

bool GossipHello_npc_deserter_agitator(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(11126) == QUEST_STATUS_INCOMPLETE)
    {
        _Creature->setFaction(1883);
        player->TalkedToCreature(_Creature->GetEntry(), _Creature->GetGUID());
    }
    else
        player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

/*######
## npc_lady_jaina_proudmoore
######*/

#define GOSSIP_ITEM_JAINA "I know this is rather silly but i have a young ward who is a bit shy and would like your autograph."

bool GossipHello_npc_lady_jaina_proudmoore(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if( player->GetQuestStatus(558) == QUEST_STATUS_INCOMPLETE )
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_JAINA, GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO );

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_lady_jaina_proudmoore(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if( action == GOSSIP_SENDER_INFO )
    {
        player->SEND_GOSSIP_MENU( 7012, _Creature->GetGUID() );
        player->CastSpell( player, 23122, false);
    }
    return true;
}

/*######
## npc_nat_pagle
######*/

bool GossipHello_npc_nat_pagle(Player *player, Creature *_Creature)
{
    if(_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if(_Creature->isVendor() && player->GetQuestRewardStatus(8227))
    {
        player->ADD_GOSSIP_ITEM(1, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        player->SEND_GOSSIP_MENU( 7640, _Creature->GetGUID() );
    }
    else
        player->SEND_GOSSIP_MENU( 7638, _Creature->GetGUID() );

    return true;
}

bool GossipSelect_npc_nat_pagle(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_TRADE)
        player->SEND_VENDORLIST( _Creature->GetGUID() );

    return true;
}

/*######
## npc_theramore_combat_dummy
######*/

struct TRINITY_DLL_DECL npc_theramore_combat_dummyAI : public Scripted_NoMovementAI
{
    npc_theramore_combat_dummyAI(Creature *c) : Scripted_NoMovementAI(c)
    {
        // niech kto� to przepisze na zapytanie plx
        CreatureInfo *cInfo = (CreatureInfo *)m_creature->GetCreatureInfo();
        if (cInfo)
            cInfo->flags_extra |= CREATURE_FLAG_EXTRA_NO_DAMAGE_TAKEN;
    }

    uint64 AttackerGUID;
    uint32 Check_Timer;

    void Reset()
    {
        m_creature->SetNoCallAssistance(true);
        m_creature->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_STUN, true);
        AttackerGUID = 0;
        Check_Timer = 0;
    }

    void Aggro(Unit* who)
    {
        AttackerGUID = ((Player*)who)->GetGUID();
        m_creature->SetStunned(true);
    }

    void DamageTaken(Unit *attacker, uint32 &damage)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        Player* attacker = Player::GetPlayer(AttackerGUID);

        if (!UpdateVictim())
            return;

        if (attacker && Check_Timer < diff)
        {
            if(m_creature->GetDistance2d(attacker) > 12.0f)
                EnterEvadeMode();

            Check_Timer = 3000;
        }
        else
            Check_Timer -= diff;
    }
};

CreatureAI* GetAI_npc_theramore_combat_dummy(Creature *_Creature)
{
    return new npc_theramore_combat_dummyAI (_Creature);
}

#define QUEST_THE_GRIMTOTEM_WEAPON      11169
#define AURA_CAPTURED_TOTEM             42454
#define NPC_CAPTURED_TOTEM              23811

/*######
## mob_mottled_drywallow_crocolisks
######*/

struct TRINITY_DLL_DECL mob_mottled_drywallow_crocolisksAI : public ScriptedAI
{
   mob_mottled_drywallow_crocolisksAI(Creature *c) : ScriptedAI(c) {}

    void Reset() {}
    void JustDied (Unit* killer)
    {
        if (killer)
        {
            Player* pl;
            if (killer->GetTypeId() != TYPEID_PLAYER)
                pl = (Player*)m_creature->GetUnit(*m_creature, killer->GetOwnerGUID());
            else
                pl = (Player*)killer;

            if(pl->GetTypeId() == TYPEID_PLAYER && pl->GetQuestStatus(QUEST_THE_GRIMTOTEM_WEAPON) == QUEST_STATUS_INCOMPLETE)
            {
                Unit* totem = FindCreature(NPC_CAPTURED_TOTEM, 20.0, m_creature);   //blizzlike(?) check by dummy aura is NOT working, mysteriously...
                if(totem)
                    pl->KilledMonster(NPC_CAPTURED_TOTEM, pl->GetGUID());
            }
        }
    }
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff)
    {
        if(!UpdateVictim()) { return; }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_mottled_drywallow_crocolisks(Creature *_Creature)
{
    return new mob_mottled_drywallow_crocolisksAI (_Creature);
}

void AddSC_dustwallow_marsh()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mobs_risen_husk_spirit";
    newscript->GetAI = &GetAI_mobs_risen_husk_spirit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_restless_apparition";
    newscript->pGossipHello =   &GossipHello_npc_restless_apparition;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_deserter_agitator";
    newscript->GetAI = &GetAI_npc_deserter_agitator;
    newscript->pGossipHello = &GossipHello_npc_deserter_agitator;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_lady_jaina_proudmoore";
    newscript->pGossipHello = &GossipHello_npc_lady_jaina_proudmoore;
    newscript->pGossipSelect = &GossipSelect_npc_lady_jaina_proudmoore;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_nat_pagle";
    newscript->pGossipHello = &GossipHello_npc_nat_pagle;
    newscript->pGossipSelect = &GossipSelect_npc_nat_pagle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_theramore_combat_dummy";
    newscript->GetAI = &GetAI_npc_theramore_combat_dummy;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_mottled_drywallow_crocolisks";
    newscript->GetAI = &GetAI_mob_mottled_drywallow_crocolisks;
    newscript->RegisterSelf();
}

