function onSpawn()
	SetInputs({
		"A", "number", 4, 10,
		"B", "number", 4, 18,
		"C", "number", 4, 26,
		"D", "number", 4, 34,
		"E", "number", 4, 42,
		"F", "number", 4, 50,
		"G", "number", 4, 58,
		"H", "number", 4, 66
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, 10
	})
	SetToolTip("Binary -> Byte")
	SetImage("Default\\img\\convert_bin_byte.png")
end

local out = 0;

function BinToNum(InputTable)
	local converted = 0;
	--for( int i = binaryNumber.Length – 1; i >= 0; i–)
	--	if( binaryNumber[i] == ‘1' ) converted = converted | (((UInt32)1) << (binaryNumber.Length-1-i));
	local i = #InputTable
	while i > 0 do
		if not(InputTable[i] == 0) then
			converted = BIT_OR(converted, BIT_LSHIFT(1, #InputTable-i))
		end
		i = i - 1
	end
	
	return converted;
end

function onTriggerInput(port)
	out = BinToNum({GetInput(7), GetInput(6), GetInput(5), GetInput(4), GetInput(3), GetInput(2), GetInput(1), GetInput(0)})
	SetToolTip(
		IFELSE(GetInput(7) == 0, "0", "1") .. IFELSE(GetInput(6) == 0, "0", "1") .. IFELSE(GetInput(5) == 0, "0", "1") .. IFELSE(GetInput(4) == 0, "0", "1") .. IFELSE(GetInput(3) == 0, "0", "1") .. IFELSE(GetInput(2) == 0, "0", "1") .. IFELSE(GetInput(1) == 0, "0", "1") .. IFELSE(GetInput(0) == 0, "0", "1") .. " = " .. out
	)
	TriggerOutput(0, out)
end
