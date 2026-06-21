-- Example: toggle low gravity with a configurable key + factor.
-- Demonstrates Mods.config, priority, key binds, World gravity API.

local cfg = Mods.config("example_lowgrav", {
	key    = "CTRL+G",   -- toggle key
	factor = 0.25,       -- gravity multiplier when "low" is on
})

local normal, low = nil, false

Mods.register{
	name     = "example_lowgrav",
	version  = "1.1",
	priority = 0,

	init = function()
		Mods.log("example_lowgrav ready - press "..cfg.key.." in a level (factor "..cfg.factor..")")
	end,

	keys = {
		[cfg.key] = function()
			if normal == nil then normal = World_GetGravity() end
			low = not low
			local m = low and cfg.factor or 1
			World_SetGravity(Vector2(normal.x, normal.y * m), 0)
			Mods.log(low and "low gravity ON" or "low gravity OFF")
		end,
	},
}
