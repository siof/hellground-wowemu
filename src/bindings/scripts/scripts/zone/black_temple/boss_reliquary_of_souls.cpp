/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Boss_Reliquary_of_Souls
SD%Complete: 90
SDComment:
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "def_black_temple.h"
#include "Spell.h"

//Sound'n'speech
//Suffering
#define SUFF_SAY_FREED              -1564047
#define SUFF_SAY_AGGRO              -1564048
#define SUFF_SAY_SLAY1              -1564049
#define SUFF_SAY_SLAY2              -1564050
#define SUFF_SAY_SLAY3              -1564051
#define SUFF_SAY_RECAP              -1564052
#define SUFF_SAY_AFTER              -1564053
#define SUFF_EMOTE_ENRAGE           -1564054

//Desire
#define DESI_SAY_FREED              -1564055
#define DESI_SAY_SLAY1              -1564056
#define DESI_SAY_SLAY2              -1564057
#define DESI_SAY_SLAY3              -1564058
#define DESI_SAY_SPEC               -1564059
#define DESI_SAY_RECAP              -1564060
#define DESI_SAY_AFTER              -1564061

//Anger
#define ANGER_SAY_FREED             -1564062
#define ANGER_SAY_FREED2            -1564063
#define ANGER_SAY_SLAY1             -1564064
#define ANGER_SAY_SLAY2             -1564065
#define ANGER_SAY_SPEC              -1564066
#define ANGER_SAY_BEFORE            -1564067
#define ANGER_SAY_DEATH             -1564068

//Spells
#define AURA_OF_SUFFERING               41292
#define AURA_OF_SUFFERING_ARMOR         42017 // linked aura, need core support
#define ESSENCE_OF_SUFFERING_PASSIVE    41296 // periodic trigger 41294
#define ESSENCE_OF_SUFFERING_PASSIVE2   41623
#define SPELL_FIXATE_TARGET             41294 // dummy, select target
#define SPELL_FIXATE_TAUNT              41295 // force taunt
#define SPELL_ENRAGE                    41305
#define SPELL_SOUL_DRAIN                41303

#define AURA_OF_DESIRE                  41350
#define AURA_OF_DESIRE_DAMAGE           41352
#define SPELL_RUNE_SHIELD               41431
#define SPELL_DEADEN                    41410
#define SPELL_SOUL_SHOCK                41426

#define AURA_OF_ANGER                   41337
#define SPELL_SELF_SEETHE               41364 // force cast 41520
#define SPELL_ENEMY_SEETHE              41520
#define SPELL_SOUL_SCREAM               41545
#define SPELL_SPITE_TARGET              41376 // cast 41377 after 6 sec
#define SPELL_SPITE_DAMAGE              41377

#define ENSLAVED_SOUL_PASSIVE           41535
#define SPELL_SOUL_RELEASE              41542
#define SPELL_SUBMERGE                  37550 //dropout 'head'

#define CREATURE_ENSLAVED_SOUL          23469
#define NUMBER_ENSLAVED_SOUL            8

struct Position
{
    float x,y;
};

static Position Coords[]=
{
    {450.4, 212.3},
    {542.1, 212.3},
    {542.1, 168.3},
    {542.1, 137.4},
    {450.4, 137.4},
    {450.4, 168.3}
};

struct TRINITY_DLL_DECL npc_enslaved_soulAI : public ScriptedAI
{
    npc_enslaved_soulAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)c->GetInstanceData();
    }

    ScriptedInstance *pInstance;

    void Reset(){}

    void Aggro(Unit* who)
    {
        DoCast(m_creature, ENSLAVED_SOUL_PASSIVE, true);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance)
            pInstance->SetData(DATA_ENSLAVED_SOUL, 1);

        DoCast(m_creature, SPELL_SOUL_RELEASE, true);
    }

};

struct TRINITY_DLL_DECL boss_reliquary_of_soulsAI : public ScriptedAI
{
    boss_reliquary_of_soulsAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        EssenceGUID = 0;
    }

    ScriptedInstance* pInstance;

    uint64 EssenceGUID;

    uint32 Phase;
    uint32 Counter;
    uint32 Timer;

    uint32 SoulCount;
    uint32 SoulDeathCount;

    void Reset()
    {
        if(pInstance)
            pInstance->SetData(DATA_RELIQUARYOFSOULSEVENT, NOT_STARTED);

        if(EssenceGUID)
        {
            if(Unit* Essence = Unit::GetUnit(*m_creature, EssenceGUID))
            {
                Essence->SetVisibility(VISIBILITY_OFF);
                Essence->setDeathState(DEAD);
            }
            EssenceGUID = 0;
        }

        Phase = 0;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
        m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
    }

    void Aggro(Unit* who)
    {
        m_creature->AddThreat(who, 10000.0f);
        DoZoneInCombat();
        
        if(pInstance)
            pInstance->SetData(DATA_RELIQUARYOFSOULSEVENT, IN_PROGRESS);

        Phase = 1;
        Counter = 0;
        Timer = 0;
    }

    void AttackStart(Unit* who)
    {
        if (!InCombat)
        {
            Aggro(who);
            InCombat = true;
        }
    }

    bool SummonSoul()
    {
        uint32 random = rand()%6;
        float x = Coords[random].x;
        float y = Coords[random].y;
        
        Creature* Soul = m_creature->SummonCreature(CREATURE_ENSLAVED_SOUL, x, y, m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
        
        if(!Soul)
            return false;

        if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0, 200, true))
            Soul->AI()->AttackStart(target);
        else
            EnterEvadeMode();

        return true;
    }

    void JustDied(Unit* killer)
    {
        if(pInstance)
            pInstance->SetData(DATA_RELIQUARYOFSOULSEVENT, DONE);

        InCombat = false;
    }

    bool FindPlayers()
    {
        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
        if(m_threatlist.empty())
            return false;

        for(std::list<HostilReference*>::iterator itr = m_threatlist.begin(); itr != m_threatlist.end(); ++itr)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
            if(pUnit && pUnit->isAlive() && pUnit->IsWithinDistInMap(m_creature, 200.0f)) 
                return true;
        }
        return false;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!Phase)
            return;

        if(!FindPlayers()) // Reset if event is begun and we don't have a threatlist
        {
            EnterEvadeMode();
            return;
        }

        Creature* Essence;
        if(EssenceGUID)
        {
            Essence = Unit::GetCreature(*m_creature, EssenceGUID);
            if(!Essence)
            {
                EnterEvadeMode();
                return;
            }
        }

        if(Timer < diff)
        {
            switch(Counter)
            {
            case 0:
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);  // I R ANNNGRRRY!
                Timer = 3000;
                break;
            case 1:
                Timer = 2800;
                DoCast(m_creature,SPELL_SUBMERGE);
                break;
            case 2:
                Timer = 5000;
                if(Creature* Summon = DoSpawnCreature(23417+Phase, 0, 0, 0, 0, TEMPSUMMON_DEAD_DESPAWN, 0))
                {
                    if(Unit* target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0))
                    {
                        Summon->AI()->AttackStart(target);
                        EssenceGUID = Summon->GetGUID();
                    }
                }
                else
                    EnterEvadeMode();
                break;
            case 3:
                Timer = 1000;
                if(Phase == 3)
                {
                    if(!Essence->isAlive())
                        DoCast(m_creature, 7, true);
                    else
                        return;
                }
                else
                {
                    if(Essence->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
                        Essence->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(),m_creature->GetPositionY(), m_creature->GetPositionZ());
                    else
                        return;
                }
                break;
            case 4:
                Timer = 1500;
                if(Essence->IsWithinDistInMap(m_creature, 10))
                    m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                else
                {
                    Essence->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(),m_creature->GetPositionY(), m_creature->GetPositionZ());
                    return;
                }
                break;
            case 5:
                if(Phase == 1)
                    DoScriptText(SUFF_SAY_AFTER, Essence);
                else
                    DoScriptText(DESI_SAY_AFTER, Essence);

                Essence->SetVisibility(VISIBILITY_OFF);
                Essence->setDeathState(DEAD);
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);

                EssenceGUID = 0;
                SoulCount = 0;
                SoulDeathCount = 0;
                Timer = 3000;
                break;
            case 6:
                if(SoulCount < NUMBER_ENSLAVED_SOUL)
                {
                    if(pInstance && !SoulCount)
                        pInstance->SetData(DATA_ENSLAVED_SOUL, 0);
                        
                    if(SummonSoul())
                        SoulCount++;

                    Timer = 200;
                    return;
                }
                break;
            case 7:
                if(pInstance->GetData(DATA_ENSLAVED_SOUL) >= SoulCount)
                {
                    Counter = 1;
                    Phase++;
                    Timer = 5000;
                }
                return;
            default:
                break;
            }
            Counter++;
        }
        else
            Timer -= diff;
    }
};

//This is used to sort the players by distance in preparation for the Fixate cast.
struct TargetDistanceOrder : public std::binary_function<const Unit, const Unit, bool>
{
    const Unit* MainTarget;
    TargetDistanceOrder(const Unit* Target) : MainTarget(Target) {};
    // functor for operator "<"
    bool operator()(const Unit* _Left, const Unit* _Right) const
    {
        return (MainTarget->GetDistance(_Left) < MainTarget->GetDistance(_Right));
    }
};

struct TRINITY_DLL_DECL boss_essence_of_sufferingAI : public ScriptedAI
{
    boss_essence_of_sufferingAI(Creature *c) : ScriptedAI(c)
    {
        SpellEntry *tempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(ESSENCE_OF_SUFFERING_PASSIVE);
        if(tempSpell)
            tempSpell->EffectTriggerSpell[1] = 0;
    }

    uint64 StatAuraGUID;

    uint32 AggroYellTimer;
    uint32 FixateTimer;
    uint32 EnrageTimer;
    uint32 SoulDrainTimer;
    uint32 AuraTimer;

    bool emoteDone;

    void Reset()
    {
        StatAuraGUID = 0;

        AggroYellTimer = 5000;
        FixateTimer = 8000;
        EnrageTimer = 30000;
        SoulDrainTimer = 45000;
        AuraTimer = 5000;

        emoteDone = false;
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage >= m_creature->GetHealth())
        {
            damage = 0;
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            if(!emoteDone)
            {
                DoScriptText(SUFF_SAY_RECAP, m_creature);
                emoteDone = true;
            }
        }
    }

    void Aggro(Unit *who)
    {
         if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
             return;

        DoZoneInCombat();
        DoScriptText(SUFF_SAY_FREED, m_creature);
        DoCast(m_creature, AURA_OF_SUFFERING, true);
        DoCast(m_creature, ESSENCE_OF_SUFFERING_PASSIVE, true);
        DoCast(m_creature, ESSENCE_OF_SUFFERING_PASSIVE2, true);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SUFF_SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SUFF_SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SUFF_SAY_SLAY3, m_creature); break;
        }
    }

    void CastFixate()
    {
        std::list<Unit *> targets;
        Map *map = m_creature->GetMap();
        if(map->IsDungeon())
        {
            InstanceMap::PlayerList const &PlayerList = ((InstanceMap*)map)->GetPlayers();
            for (InstanceMap::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (Player* i_pl = i->getSource())
                {
                    if(i_pl && i_pl->isAlive() && !i_pl->isGameMaster())
                        targets.push_back(i_pl);
                }
            }
        }

        if(targets.empty())
            return; // No targets added for some reason. No point continuing.

        targets.sort(TargetDistanceOrder(m_creature)); // Sort players by distance.
        targets.resize(1); // Only need closest target.
        
        Unit* target = targets.front();
        if(target)
        {
            DoResetThreat();

            target->CastSpell(m_creature, SPELL_FIXATE_TAUNT, true);
            m_creature->AddAura(SPELL_FIXATE_TARGET, target);
            m_creature->AddThreat(target, 1000000.0f);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(InCombat)
        {
            //Supposed to be cast on nearest target
            if(FixateTimer < diff)
            {
                CastFixate();
                if(urand(0,16) == 0)
                    DoScriptText(SUFF_SAY_AGGRO, m_creature);

                FixateTimer = 5000; 
            }
            else
                FixateTimer -= diff;
        }

        //Return since we have no target
        if (!UpdateVictim())
            return;

        if(EnrageTimer < diff)
        {
            DoCast(m_creature, SPELL_ENRAGE, true);
            DoScriptText(SUFF_EMOTE_ENRAGE, m_creature);
            EnrageTimer = 60000;
        }
        else
            EnrageTimer -= diff;

        if(SoulDrainTimer < diff)
        {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0, 200, true))
                DoCast(target, SPELL_SOUL_DRAIN);

            SoulDrainTimer = 60000;
        }
        else
            SoulDrainTimer -= diff;
        
        if(!m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            DoMeleeAttackIfReady();
    }
};

struct TRINITY_DLL_DECL boss_essence_of_desireAI : public ScriptedAI
{
    boss_essence_of_desireAI(Creature *c) : ScriptedAI(c) {}

    uint32 RuneShieldTimer;
    uint32 DeadenTimer;
    uint32 SoulShockTimer;

    bool emoteDone;

    void Reset()
    {
        RuneShieldTimer = 60000;
        DeadenTimer = 30000;
        SoulShockTimer = 5000;
        m_creature->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);

        emoteDone = false;
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage >= m_creature->GetHealth())
        {
            damage = 0;
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            if(!emoteDone)
            {
                DoScriptText(DESI_SAY_RECAP, m_creature);
                emoteDone = true;
            }
        }
        else
        {
            int32 bp0 = damage / 2;
            m_creature->CastCustomSpell(done_by, AURA_OF_DESIRE_DAMAGE, &bp0, NULL, NULL, true);
        }
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if(m_creature->m_currentSpells[CURRENT_GENERIC_SPELL])
        {
            for(uint8 i = 0; i < 3; ++i)
            {
                if(spell->Effect[i] == SPELL_EFFECT_INTERRUPT_CAST)
                {
                    SpellEntry const *temp = m_creature->m_currentSpells[CURRENT_GENERIC_SPELL]->m_spellInfo;
                    if(temp->Id == SPELL_SOUL_SHOCK || temp->Id == SPELL_DEADEN)
                    {
                        m_creature->InterruptSpell(CURRENT_GENERIC_SPELL, false);
                        return;
                    }
                }
            }
        }
    }

    void Aggro(Unit *who)
    {
        if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
             return;

        DoZoneInCombat();
        DoCast(m_creature, AURA_OF_DESIRE, true);
        DoScriptText(DESI_SAY_FREED, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(DESI_SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(DESI_SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(DESI_SAY_SLAY3, m_creature); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if(RuneShieldTimer < diff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCast(m_creature, SPELL_RUNE_SHIELD, true);
            SoulShockTimer += 2000;
            DeadenTimer += 2000;
            RuneShieldTimer = 60000;
        }
        else
            RuneShieldTimer -= diff;

        if(SoulShockTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SOUL_SHOCK);
            SoulShockTimer = 5000;
        }
        else
            SoulShockTimer -= diff;

        if(DeadenTimer < diff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCast(m_creature->getVictim(), SPELL_DEADEN);

            if(urand(0,1))
                DoScriptText(DESI_SAY_SPEC, m_creature);

            DeadenTimer = 25000 + rand()%10000;
        }
        else
            DeadenTimer -= diff;

        if(!m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            DoMeleeAttackIfReady();
    }
};

struct TRINITY_DLL_DECL boss_essence_of_angerAI : public ScriptedAI
{
    boss_essence_of_angerAI(Creature *c) : ScriptedAI(c) {}

    uint64 AggroTargetGUID;

    uint32 CheckTankTimer;
    uint32 SoulScreamTimer;
    uint32 SpiteTimer;

    std::list<uint64> SpiteTargetGUID;

    bool CheckedAggro;

    void Reset()
    {
        AggroTargetGUID = 0;

        CheckTankTimer = 5000;
        SoulScreamTimer = 10000;
        SpiteTimer = 30000;

        SpiteTargetGUID.clear();

        CheckedAggro = false;
    }

    void Aggro(Unit *who)
    {
        switch(rand()%2)
        {
        case 0: DoScriptText(ANGER_SAY_FREED, m_creature); break;
        case 1: DoScriptText(ANGER_SAY_FREED2, m_creature); break;
        }

        DoZoneInCombat();
        DoCast(m_creature, AURA_OF_ANGER, true);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(ANGER_SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
        case 0: DoScriptText(ANGER_SAY_SLAY1, m_creature); break;
        case 1: DoScriptText(ANGER_SAY_SLAY2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        if(!CheckedAggro)
        {
            AggroTargetGUID = m_creature->getVictim()->GetGUID();
            CheckedAggro = true;
        }

        if(CheckTankTimer < diff)
        {
            if(m_creature->getVictim()->GetGUID() != AggroTargetGUID)
            {
                DoScriptText(ANGER_SAY_BEFORE, m_creature);
                DoCast(m_creature, SPELL_SELF_SEETHE, true);
                AggroTargetGUID = m_creature->getVictim()->GetGUID();
            }
            CheckTankTimer = 2000;
        }
        else
            CheckTankTimer -= diff;

        if(SoulScreamTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SOUL_SCREAM);

            if(!urand(0,2))
                DoScriptText(ANGER_SAY_SPEC, m_creature);
            
            SoulScreamTimer = 9000 + rand()%2000;
        }
        else
            SoulScreamTimer -= diff;

        if(SpiteTimer < diff)
        {
            DoCast(m_creature, SPELL_SPITE_TARGET);
            DoScriptText(ANGER_SAY_SPEC, m_creature);
            SpiteTimer = 30000;
        }
        else
            SpiteTimer -= diff;

        if(!m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_reliquary_of_souls(Creature *_Creature)
{
    return new boss_reliquary_of_soulsAI (_Creature);
}

CreatureAI* GetAI_boss_essence_of_suffering(Creature *_Creature)
{
    return new boss_essence_of_sufferingAI (_Creature);
}

CreatureAI* GetAI_boss_essence_of_desire(Creature *_Creature)
{
    return new boss_essence_of_desireAI (_Creature);
}

CreatureAI* GetAI_boss_essence_of_anger(Creature *_Creature)
{
    return new boss_essence_of_angerAI (_Creature);
}

CreatureAI* GetAI_npc_enslaved_soul(Creature *_Creature)
{
    return new npc_enslaved_soulAI (_Creature);
}

void AddSC_boss_reliquary_of_souls()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_reliquary_of_souls";
    newscript->GetAI = &GetAI_boss_reliquary_of_souls;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="boss_essence_of_suffering";
    newscript->GetAI = &GetAI_boss_essence_of_suffering;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="boss_essence_of_desire";
    newscript->GetAI = &GetAI_boss_essence_of_desire;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="boss_essence_of_anger";
    newscript->GetAI = &GetAI_boss_essence_of_anger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_enslaved_soul";
    newscript->GetAI = &GetAI_npc_enslaved_soul;
    newscript->RegisterSelf();
}

