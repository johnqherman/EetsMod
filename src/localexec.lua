-- localexec.lua : engine dofile's this at boot, after Lua API + extensions register.
-- Zero-patch mod-loader entry point. Keep this tiny; real work in Mods/framework.lua.

local ok, err = pcall(dofile, "Data/Mods/framework.lua")
if not ok then
	-- framework.lua not present or errored; log and continue so the game still boots.
	if Print then Print("[Mods] framework failed: " .. tostring(err)) end
	local f = io.open("Log/mods.log", "a")
	if f then f:write("framework failed: " .. tostring(err) .. "\n"); f:close() end
end
