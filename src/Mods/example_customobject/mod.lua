-- Example: spawn a custom blueprint at runtime.
-- The framework auto-copies objects/superbounce.lua -> Data/Objects/ at boot;
-- World_CreateObject lazy-loads it on first spawn.

Mods.register{
	name    = "example_customobject",
	version = "1.0",

	init = function()
		Mods.log("example_customobject ready - press CTRL+B in a level to spawn 'superbounce'")
	end,

	keys = {
		["CTRL+B"] = function()
			if World_IsInMainMenu and World_IsInMainMenu() then return end
			local eets = World_GetEets and World_GetEets()
			local pos = eets and eets:GetPosition() or Vector2(400, 300)
			Mods.spawn("superbounce", pos.x, pos.y - 80)
			Mods.log("spawned superbounce")
		end,
	},
}
