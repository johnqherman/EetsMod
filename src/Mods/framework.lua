-- ============================================================================
-- Eets Mod Framework  (Lua 5.0, runs from localexec.lua at boot)
-- ----------------------------------------------------------------------------
-- Boot order (game.log): Lua Game Lib -> Register Extensions -> World/Simulator
--   -> default key binds -> dofile(DATA:localexec.lua) -> here.
-- The full C API + luabind classes are live by now. Object blueprints &
-- extensions are already loaded, so brand-new *blueprints* must be drop-in
-- files; behaviour / config / event / per-frame mods work at runtime.
--
-- Verified-real globals (from live _G dump, NOT the C++ symbol names):
--   Bind(key, luaCodeString)      -- bind a key to a Lua string  (e.g. "CTRL+G")
--   Print(string)                 -- engine console print
--   LoadFile(path)                -- engine resource-aware loader
--   Update(), OnPause(), OnUnpause()  -- GLOBAL hooks the engine calls
--   class(...)                    -- luabind class system
--   World_*/Object_*/Sound_*/FX_*/Profile_*  -- gameplay API
--   Vector2, Colour, Object, *Extension, MotionModel, Motion, Animation ... (ctors)
--   full stdlib: os io string table math debug coroutine
-- ============================================================================

Mods = {
	version   = "0.2.0",
	root      = "Data/Mods",
	_list     = {},
	_byname   = {},
	_handlers = {},   -- id -> fn (key binds)
	_hid      = 0,
	_ticks    = {},   -- per-frame callbacks
	_onpause  = {},
	_onunpause= {},
	frame     = 0,
}

-- ---- logging --------------------------------------------------------------
function Mods.log(msg)
	msg = "[Mods] " .. tostring(msg)
	if Print then Print(msg) end
	local f = io.open("Log/mods.log", "a")
	if f then f:write(msg .. "\n"); f:close() end
end

-- ---- registration ---------------------------------------------------------
-- mod.lua calls Mods.register{ name=, version=, init=, update=, onpause=,
--                              onunpause=, keys={ ["CTRL+G"]=fn } }
function Mods.register(def)
	if type(def) ~= "table" or not def.name then Mods.log("register: needs .name"); return end
	if Mods._byname[def.name] then Mods.log("register: dup '"..def.name.."'"); return end
	Mods._byname[def.name] = def
	table.insert(Mods._list, def)

	if def.keys then for key, fn in pairs(def.keys) do Mods.bindkey(key, fn) end end
	if type(def.update)    == "function" then table.insert(Mods._ticks,     def.update) end
	if type(def.onpause)   == "function" then table.insert(Mods._onpause,   def.onpause) end
	if type(def.onunpause) == "function" then table.insert(Mods._onunpause, def.onunpause) end
	if type(def.init) == "function" then
		local ok, err = pcall(def.init)
		if not ok then Mods.log("'"..def.name.."' init error: "..tostring(err)) end
	end
	Mods.log("loaded '"..def.name.."' "..tostring(def.version or ""))
end

-- ---- key binds (engine global: Bind(key, luaCodeString)) -------------------
function Mods.bindkey(key, fn)
	if not Bind then Mods.log("Bind() unavailable"); return end
	Mods._hid = Mods._hid + 1
	Mods._handlers[Mods._hid] = fn
	Bind(key, "Mods._fire("..Mods._hid..")")
	Mods.log("bound "..key)
end
function Mods._fire(id)
	local f = Mods._handlers[id]
	if f then local ok, err = pcall(f); if not ok then Mods.log("key err: "..tostring(err)) end end
end

-- ---- global hook chaining (Update / OnPause / OnUnpause) ------------------
-- Engine-invoked global hooks. We wrap any pre-existing one so we don't clobber
-- the game's own, then fan out to registered mod callbacks.
local function chain(globalName, queue)
	local prev = rawget(getfenv(1), globalName)
	setfenv(1, getfenv(1))  -- no-op guard; keep env
	_G[globalName] = function(...)
		if prev then pcall(prev) end           -- call original (arg-less hooks)
		for _, fn in ipairs(queue) do pcall(fn) end
	end
end

function Mods._installHooks()
	-- per-frame
	local prevUpdate = _G.Update
	_G.Update = function(...)
		Mods.frame = Mods.frame + 1
		if prevUpdate then pcall(prevUpdate) end
		local i, n = 1, table.getn(Mods._ticks)
		while i <= n do pcall(Mods._ticks[i]); i = i + 1 end
	end
	-- pause events
	local prevPause = _G.OnPause
	_G.OnPause = function(...)
		if prevPause then pcall(prevPause) end
		for _, fn in ipairs(Mods._onpause) do pcall(fn) end
	end
	local prevUnpause = _G.OnUnpause
	_G.OnUnpause = function(...)
		if prevUnpause then pcall(prevUnpause) end
		for _, fn in ipairs(Mods._onunpause) do pcall(fn) end
	end
end

-- ---- discovery ------------------------------------------------------------
local function listdirs(path)
	local out, p = {}, io.popen('ls -1 "'..path..'" 2>/dev/null')
	if not p then return out end
	while true do
		local line = p:read("*l"); if not line then break end
		if line ~= "" and string.sub(line, -4) ~= ".lua" then table.insert(out, line) end
	end
	p:close(); return out
end
local function fileexists(path)
	local f = io.open(path, "r"); if f then f:close(); return true end; return false
end

function Mods.loadAll()
	Mods.log("framework "..Mods.version.." scanning "..Mods.root)
	Mods._installHooks()
	local count = 0
	for _, name in ipairs(listdirs(Mods.root)) do
		local modfile = Mods.root.."/"..name.."/mod.lua"
		if fileexists(modfile) then
			count = count + 1
			local ok, err = pcall(dofile, modfile)
			if not ok then Mods.log("dofile '"..modfile.."' failed: "..tostring(err)) end
		end
	end
	Mods.log("done; "..table.getn(Mods._list).." mod(s) from "..count.." candidate(s)")
end

Mods.loadAll()
