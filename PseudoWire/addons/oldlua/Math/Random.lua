function onSpawn()
	SetImage("Default\\img\\math_rand.png")
	SetOutputs({
		"Out", "number", GetWidth() - 2, GetWidth()/2
	})
	SetToolTip("Out = Random")
	math.randomseed(os.time())
end

function onTick(tick, elapsed, ticktime)
	local out = math.random()
	SetToolTip("Out = " .. out)
	TriggerOutput(0, out)
end