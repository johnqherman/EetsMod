-- Custom blueprint (reuses stock Star animations + a high-restitution physics).
-- Proves runtime drop-in objects: the framework copies this to Data/Objects/,
-- and World_CreateObject("superbounce", pos) lazy-loads it (no AllObjects.lua,
-- no restart). Verified against ObjectMgr::CreateObject behaviour.
ObjectData =
{
	MotionModel =
	{
		DefaultAnimation = [[normal]],
		Motions =
		{
			motion1 =
			{
				Animations =
				{
					anim1 =
					{
						Path = [[DATA:Animations\Star\starman_faces.anim]],
						Name = [[starman_faces]],
						MinFreq = 1.000000,
						MaxFreq = 1.000000,
					},
				},
				Name = [[normal]],
			},
		},
	},
	Extensions =
	{
		PhysicsExtension =
		{
			restitution = 0.95,   -- very bouncy
			mass = 5.0,
		},
		PositionExtension = {},
		LevelEditorExtension = {},
	},
}

-- Begin Scripting --
function OnInit()
	this:CreateEffect( "Star/sprinkle" )
end

function Update()
	Profile_Start("superbounce")
	Profile_Stop("superbounce")
end
