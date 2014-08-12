function onSpawn()
	SetInputs({
		"In", "number", GetWidth() - 2, 10
	})
	SetOutputs({
		"A", "number", 4, 10,
		"B", "number", 4, 18,
		"C", "number", 4, 26,
		"D", "number", 4, 34,
		"E", "number", 4, 42,
		"F", "number", 4, 50,
		"G", "number", 4, 58,
		"H", "number", 4, 66
	})
	SetToolTip("Byte -> Binary")
	SetImage("Default\\img\\convert_bin_byte.png")
end

local out = {0, 0, 0, 0, 0, 0, 0, 0}

function NumToBin(num)
	local i = #out
	while i >= 1 do
		if not(BIT_AND(num, BIT_LSHIFT(1, i-1)) == 0) then
			out[i] = 1
		else
			out[i] = 0
		end
		i = i - 1
	end
end

function onTriggerInput(port)
	NumToBin(GetInput(0))
	SetToolTip(
		GetInput(0) .. " = " .. out[8] .. out[7] .. out[6] .. out[5] .. out[4] .. out[3] .. out[2] .. out[1]
	)
	TriggerOutput(7, out[8])
	TriggerOutput(6, out[7])
	TriggerOutput(5, out[6])
	TriggerOutput(4, out[5])
	TriggerOutput(3, out[4])
	TriggerOutput(2, out[3])
	TriggerOutput(1, out[2])
	TriggerOutput(0, out[1])
end
