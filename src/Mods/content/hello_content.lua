-- Runs SANDBOXED: io, os, dofile, loadfile, require are nil here.
Mods.register{
	name = "hello_content",
	version = "1.0",
	init = function()
		Mods.log("hello_content: sandboxed (io="..tostring(io)..", os="..tostring(os)..", dofile="..tostring(dofile)..")")
		Mods.log("hello_content: gameplay API ok, World_GetGravity="..tostring(World_GetGravity))
	end,
}
