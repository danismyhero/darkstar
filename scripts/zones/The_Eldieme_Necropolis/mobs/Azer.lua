-----------------------------------
-- Area: The Eldieme Necropolis
--  Mob: Azer
-----------------------------------
require("scripts/globals/regimes")
-----------------------------------

function onMobDeath(mob, player, isKiller)
    dsp.regime.checkRegime(player, mob, 673, 2, dsp.regime.type.GROUNDS)
end;