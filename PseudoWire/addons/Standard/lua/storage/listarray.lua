local ram = {}

function Component:onSpawn()
	self:SetImage("ram_dynamic.png")
	self:SetToolTip("List\nDynamic Array")
	
	self:SetInputs({
		{"Value", TYPE_NUMBER, 2, 12},
		{"Address", TYPE_NUMBER, 2, 20},
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
	local outofbounds = (inputs[2] <= 0 or inputs[2] > #ram)
	
	if(self:IsInputTriggered(3) and inputs[3] ~= 0) then
		if(inputs[3] > 0) then
			--add
			ram[#ram + 1] = inputs[1]
			self:TriggerOutput(2, #ram)
		else
			--remove
			if(not outofbounds) then
				table.remove(ram, inputs[2])
				self:TriggerOutput(2, #ram)
			end
		end
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
