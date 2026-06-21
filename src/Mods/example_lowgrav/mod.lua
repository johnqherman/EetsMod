-- Example mod: toggle low gravity with CTRL+G.
-- Demonstrates: register, init, key binds, World_GetGravity/World_SetGravity, Vector2.

local normal = nil   -- captured first toggle
local low    = false

Mods.register{
	name    = "example_lowgrav",
	version = "1.0",

	init = function()
		Mods.log("example_lowgrav ready - press CTRL+G in a level")
	end,

	keys = {
		["CTRL+G"] = function()
			if normal == nil then normal = World_GetGravity() end   -- Vector2
			low = not low
			if low then
				World_SetGravity(Vector2(normal.x, normal.y * 0.25), 0)
				Mods.log("low gravity ON")
			else
				World_SetGravity(Vector2(normal.x, normal.y), 0)
				Mods.log("low gravity OFF")
			end
		end,
	},
}
