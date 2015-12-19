/*
===========================================================================

Copyright (c) 2010-2015 Darkstar Dev Teams

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

This file is part of DarkStar-server source code.

===========================================================================
*/

#include "pet_controller.h"

#include "../ai_container.h"
#include "../../status_effect_container.h"
#include "../../entities/petentity.h"
#include "../../../common/utils.h"

CPetController::CPetController(CPetEntity* _PPet) :
    CAIController(_PPet), PPet(_PPet)
{
}

void CPetController::DoRoamTick(time_point tick)
{
    if (PPet->PMaster == nullptr || PPet->PMaster->isDead()) {
        PPet->Die();
        return;
    }

    //automaton, wyvern
    if (PPet->getPetType() == PETTYPE_WYVERN || PPet->getPetType() == PETTYPE_AUTOMATON) {
        if (PetIsHealing()) {
            return;
        }
    }

    float currentDistance = distance(PPet->loc.p, PPet->PMaster->loc.p);


    // this is broken until pet / mob relationship gets fixed
    // pets need to extend mob or be a mob because pet has no spell list!
    //if (PPet->getPetType() == PETTYPE_AVATAR && PPet->m_Family == 104 && m_Tick >= m_LastActionTime + 30000 && currentDistance < PET_ROAM_DISTANCE * 2)
    //{
    //    int16 spellID = 108;
    //    // define this so action picks it up
    //    SetCurrentSpell(spellID);
    //    m_PBattleSubTarget = PPet->PMaster;

    //    m_ActionType = ACTION_MAGIC_START;
    //    ActionMagicStart();
    //    return;
    //}

    if (currentDistance > PetRoamDistance)
    {
        if (currentDistance < 35.0f && PPet->PAI->PathFind->PathAround(PPet->PMaster->loc.p, 2.0f, PATHFLAG_RUN | PATHFLAG_WALLHACK))
        {
            PPet->PAI->PathFind->FollowPath();
        }
        else if (PPet->GetSpeed() > 0)
        {
            PPet->PAI->PathFind->WarpTo(PPet->PMaster->loc.p, PetRoamDistance);
        }
    }
}

bool CPetController::PetIsHealing()
{
    bool isMasterHealing = (PPet->PMaster->animation == ANIMATION_HEALING);
    bool isPetHealing = (PPet->animation == ANIMATION_HEALING);

    if (isMasterHealing && !isPetHealing && !PPet->StatusEffectContainer->HasPreventActionEffect()) {
        //animation down
        PPet->animation = ANIMATION_HEALING;
        PPet->StatusEffectContainer->AddStatusEffect(new CStatusEffect(EFFECT_HEALING, 0, 0, 10, 0));
        PPet->updatemask |= UPDATE_HP;
        return true;
    }
    else if (!isMasterHealing && isPetHealing) {
        //animation up
        PPet->animation = ANIMATION_NONE;
        PPet->StatusEffectContainer->DelStatusEffect(EFFECT_HEALING);
        PPet->updatemask |= UPDATE_HP;
        return false;
    }
    return isMasterHealing;
}
