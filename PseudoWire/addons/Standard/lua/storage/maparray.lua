local ram = {}
local size = 0

function Component:onSpawn()
	self:SetImage("ram_dynamic.png")
	self:SetToolTip("Map\nDynamic Array")
	
	self:SetInputs({
		{"Value", TYPE_NUMBER, 2, 12},
		{"Address", TYPE_STRING, 2, 20},
		{"Add (+) | Rem (-)", TYPE_NUMBER, 2, 28},
		{"Read", TYPE_NUMBER, 2, 36}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12},
		{"Size", TYPE_NUMBER, self:GetWidth()-2, 20}
	})
end

local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	--bounds check
	local badaddress = (ram[inputs[2]] == nil)
	
	if(self:IsInputTriggered(3) and inputs[3] ~= 0) then
		if(inputs[3] > 0) then
			--add/write
			ram[inputs[2]] = inputs[1]
			if(badaddress) then
				size = size + 1
				self:TriggerOutput(2, size)
			end
		else
			--remove
			if(not badaddress) then
				size = size - 1
				table.remove(ram, inputs[2])
				self:TriggerOutput(2, size)
			end
		end
	end
	
	if(self:IsInputTriggered(4) and inputs[4] ~= 0) then
		--read
		if(badaddress) then
			self:TriggerOutput(1, 0)
		else
			self:TriggerOutput(1, ram[inputs[2]])
		end
	end
end
