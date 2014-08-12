local maxsize = 65536
local ram = {}

function Component:onSpawn()
	self:SetImage("ram_64k.png")
	self:SetToolTip("64K-Store Ram\n(65536)")
	
	--initialize ram
	for i = 1, maxsize do
		ram[i] = 0
	end
	
	self:SetInputs({
		{"Value", TYPE_NUMBER, 2, 12},
		{"Address", TYPE_NUMBER, 2, 20},
		{"Write", TYPE_NUMBER, 2, 28},
		{"Read", TYPE_NUMBER, 2, 36}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	--bounds check
	local outofbounds = (inputs[2] <= 0 or inputs[2] > maxsize)
	
	if(self:IsInputTriggered(3) and inputs[3] ~= 0 and (not outofbounds)) then
		--write
		ram[inputs[2]] = inputs[1]
	end
	if(self:IsInputTriggered(4) and inputs[4] ~= 0) then
		--read
		if(outofbounds) then
			self:TriggerOutput(1, 0)
		else
			self:TriggerOutput(1, ram[inputs[2]])
		end
	end
end
