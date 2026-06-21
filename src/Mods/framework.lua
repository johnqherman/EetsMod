-- eets mod framework (lua 5.0, run from localexec.lua at boot).
-- blueprints load lazily on first World_CreateObject, so new objects are runtime
-- drop-ins; new extension TYPES need a restart (registered before we run).
-- note lua 5.0 stdlib quirks: table.getn / string.gfind / math.mod.

Mods = {
	version   = "0.3.0",
	root      = "Data/Mods",
	gamedata  = "Data",
	_list     = {},
	_byname   = {},
	_handlers = {},
	_hid      = 0,
	_ticks    = {},
	_onpause  = {},
	_onunpause= {},
	_installed= {},
	frame     = 0,
}

-- ---- logging --------------------------------------------------------------
function Mods.log(msg)
	msg = "[Mods] " .. tostring(msg)
	if Print then Print(msg) end
	local f = io.open("Log/mods.log", "a")
	if f then f:write(msg .. "\n"); f:close() end
end

-- ---- filesystem helpers ---------------------------------------------------
local function fileexists(path)
	local f = io.open(path, "r"); if f then f:close(); return true end; return false
end
local function readall(path, bin)
	local f = io.open(path, bin and "rb" or "r"); if not f then return nil end
	local s = f:read("*a"); f:close(); return s
end
local function writeall(path, data, bin)
	local f = io.open(path, bin and "wb" or "w"); if not f then return false end
	f:write(data); f:close(); return true
end
local function listentries(path)
	local out, p = {}, io.popen('ls -1 "'..path..'" 2>/dev/null')
	if not p then return out end
	while true do local l = p:read("*l"); if not l then break end
		if l ~= "" then table.insert(out, l) end end
	p:close(); return out
end
local function copyfile(src, dst)
	local data = readall(src, true); if not data then return false end
	return writeall(dst, data, true)
end
Mods.fileexists = fileexists
Mods.readall = readall
Mods.writeall = writeall

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

-- ---- convenience ----------------------------------------------------------
function Mods.spawn(name, x, y)        -- World_CreateObject wrapper
	return World_CreateObject(name, Vector2(x, y))
end

-- ---- registration (phase 1: just record; init happens after sort) ---------
function Mods.register(def)
	if type(def) ~= "table" or not def.name then Mods.log("register: needs .name"); return end
	if Mods._byname[def.name] then Mods.log("register: dup '"..def.name.."'"); return end
	def.priority = tonumber(def.priority) or 0
	Mods._byname[def.name] = def
	table.insert(Mods._list, def)
end

-- per-mod config: Data/Mods/<mod>/config.lua returns a table; merged over defaults
function Mods.config(name, defaults)
	local cfg = {}
	if type(defaults) == "table" then for k, v in pairs(defaults) do cfg[k] = v end end
	local path = Mods.root.."/"..name.."/config.lua"
	if fileexists(path) then
		local chunk = loadfile(path)
		if chunk then
			local ok, t = pcall(chunk)
			if ok and type(t) == "table" then for k, v in pairs(t) do cfg[k] = v end
			else Mods.log("config '"..name.."' error: "..tostring(t)) end
		end
	end
	return cfg
end

-- ---- asset / object drop-in sync (copies a mod's files into game Data) -----
-- a mod dir may contain objects/*.lua, extensions/*.lua, assets/<rel>.
-- skip if the destination exists and is NOT framework-managed, so we never
-- clobber stock files. managed files are tracked in mods_installed.txt.
local function syncTree(moddir, sub, destroot, label)
	local srcdir = moddir.."/"..sub
	if not fileexists(srcdir) and table.getn(listentries(srcdir)) == 0 then return end
	for _, name in ipairs(listentries(srcdir)) do
		local src = srcdir.."/"..name
		local dst = destroot.."/"..name
		if fileexists(dst) and not Mods._installed[dst] then
			Mods.log("skip "..label.." '"..name.."' (stock file exists)")
		else
			if copyfile(src, dst) then
				Mods._installed[dst] = true
				Mods.log("installed "..label..": "..dst)
			end
		end
	end
end

local function syncAssets(moddir)
	syncTree(moddir, "objects",    Mods.gamedata.."/Objects",    "object")
	syncTree(moddir, "extensions", Mods.gamedata.."/Extensions", "extension")
	-- assets/: mirror arbitrary relative paths under Data/
	local adir = moddir.."/assets"
	for _, top in ipairs(listentries(adir)) do
		-- one level deep is enough for the common case (Images/, Sound/, ...)
		local p = io.popen('find "'..adir..'" -type f 2>/dev/null')
		if p then
			while true do local rel = p:read("*l"); if not rel then break end
				local tail = string.sub(rel, string.len(adir) + 2)  -- strip "adir/"
				local dst = Mods.gamedata.."/"..tail
				if copyfile(rel, dst) then Mods._installed[dst] = true; Mods.log("asset: "..dst) end
			end
			p:close()
		end
		break
	end
end

-- ---- global hook chaining (Update / OnPause / OnUnpause) ------------------
function Mods._installHooks()
	local prevUpdate = _G.Update
	_G.Update = function(...)
		Mods.frame = Mods.frame + 1
		if prevUpdate then pcall(prevUpdate) end
		local i, n = 1, table.getn(Mods._ticks)
		while i <= n do pcall(Mods._ticks[i]); i = i + 1 end
	end
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

-- ---- discovery + two-phase load ------------------------------------------
local function moddirs()
	local out = {}
	for _, name in ipairs(listentries(Mods.root)) do
		if string.sub(name, -4) ~= ".lua" and name ~= "" then
			if fileexists(Mods.root.."/"..name.."/mod.lua") then table.insert(out, name) end
		end
	end
	return out
end

-- ---- sandboxed content tier ----------------------------------------------
-- content mods (Data/Mods/content/*.lua) run with no io/os/dofile/loadfile/
-- loadstring/require/package/debug - only gameplay API, safe stdlib, limited
-- Mods API, for sharing untrusted content. not a hard resource sandbox
-- (infinite loops / huge allocs are still possible).
function Mods.makeSandbox()
	local g = getfenv(1)
	local sb = {}
	-- gameplay api: any global Word_Word function (World_*, Object_*, FX_*, ...)
	for k, v in pairs(g) do
		if type(v) == "function" and string.find(k, "^[A-Z][A-Za-z]+_") then sb[k] = v end
		if type(v) == "userdata" then sb[k] = v end          -- class ctors (Vector2, Object, ...)
	end
	-- safe base + stdlib
	local allow = { "tostring","tonumber","type","pairs","ipairs","next","select",
		"unpack","assert","error","pcall","xpcall","rawget","rawset","rawequal",
		"setmetatable","getmetatable","print","class","_VERSION","math","string","table" }
	for _, k in ipairs(allow) do sb[k] = g[k] end
	-- restricted Mods API (no fs helpers, no eet/raw access)
	sb.Mods = {
		register = Mods.register, log = Mods.log, spawn = Mods.spawn,
		config = Mods.config, frame = 0,
	}
	sb._G = sb
	return sb
end

local function contentfiles()
	local out = {}
	for _, name in ipairs(listentries(Mods.root.."/content")) do
		if string.sub(name, -4) == ".lua" then table.insert(out, name) end
	end
	return out
end

function Mods.loadAll()
	Mods.log("framework "..Mods.version.." scanning "..Mods.root)
	Mods._installHooks()

	-- phase 1: dofile every mod.lua (each calls Mods.register to record its def)
	local dirByName = {}
	for _, name in ipairs(moddirs()) do
		local modfile = Mods.root.."/"..name.."/mod.lua"
		local before = table.getn(Mods._list)
		local ok, err = pcall(dofile, modfile)
		if not ok then Mods.log("dofile '"..modfile.."' failed: "..tostring(err))
		else
			-- associate the just-registered def(s) with this dir
			for i = before + 1, table.getn(Mods._list) do dirByName[Mods._list[i].name] = name end
		end
	end

	-- phase 1b: sandboxed content mods (Data/Mods/content/*.lua)
	local sb = Mods.makeSandbox()
	for _, f in ipairs(contentfiles()) do
		local path = Mods.root.."/content/"..f
		local chunk, err = loadfile(path)
		if not chunk then Mods.log("content '"..f.."' load error: "..tostring(err))
		else
			setfenv(chunk, sb)
			local ok, e = pcall(chunk)
			if not ok then Mods.log("content '"..f.."' error: "..tostring(e))
			else Mods.log("loaded content (sandboxed): "..f) end
		end
	end

	-- phase 2: sort by priority (high first), then activate
	table.sort(Mods._list, function(a, b) return (a.priority or 0) > (b.priority or 0) end)
	for _, def in ipairs(Mods._list) do
		local dir = Mods.root.."/"..(dirByName[def.name] or def.name)
		syncAssets(dir)
		if def.keys then for key, fn in pairs(def.keys) do Mods.bindkey(key, fn) end end
		if type(def.update)    == "function" then table.insert(Mods._ticks,     def.update) end
		if type(def.onpause)   == "function" then table.insert(Mods._onpause,   def.onpause) end
		if type(def.onunpause) == "function" then table.insert(Mods._onunpause, def.onunpause) end
		if type(def.init) == "function" then
			local ok, err = pcall(def.init)
			if not ok then Mods.log("'"..def.name.."' init error: "..tostring(err)) end
		end
		Mods.log("loaded '"..def.name.."' "..tostring(def.version or "").." (prio "..def.priority..")")
	end
	Mods.log("done; "..table.getn(Mods._list).." mod(s)")
end

-- ---- level (.eet) toolchain ----------------------------------------------
-- .eet = lua 5.0 precompiled chunk that builds the level's global table.
-- uses the engine's own VM, so output is byte-compatible (verified).
Mods.eet = {}

-- compile Lua level source -> .eet bytes
function Mods.eet.compile(src)
	local chunk, err = loadstring(src, "level")
	if not chunk then return nil, err end
	return string.dump(chunk)
end

-- write Lua level source straight to a .eet file
function Mods.eet.build(src, outpath)
	local bytes, err = Mods.eet.compile(src)
	if not bytes then Mods.log("eet compile error: "..tostring(err)); return false end
	return writeall(outpath, bytes, true)
end

-- load a .eet (or .lua) level chunk in a sandbox and return the globals table
-- it produced ("decompile to data" - gives the Level/Objects schema).
function Mods.eet.read(path)
	local chunk = loadfile(path)
	if not chunk then return nil, "loadfile failed" end
	local env = {}
	setfenv(chunk, env)
	local ok, err = pcall(chunk)
	if not ok then return nil, err end
	return env
end

-- serialize a Lua table back to source (for .eet -> readable .lua)
local function ser(v, ind)
	local t = type(v)
	if t == "string" then return string.format("%q", v) end
	if t == "number" or t == "boolean" then return tostring(v) end
	if t == "table" then
		local s, ni = "{\n", ind.."  "
		for k, val in pairs(v) do
			local key
			if type(k) == "string" and string.find(k, "^[%a_][%w_]*$") then key = k.." = "
			else key = "["..ser(k, ni).."] = " end
			s = s..ni..key..ser(val, ni)..",\n"
		end
		return s..ind.."}"
	end
	return "nil"
end
function Mods.eet.serialize(name, tbl) return name.." = "..ser(tbl, "").."\n" end

Mods.loadAll()
