local isnum = false
local multistep = false
local multistepnum = 1

function Component:onSpawn()
	self:SetImage("io_buffer.png")
	self:SetInputs({
		{ "In", TYPE_NUMBER, 2, self:GetHeight()/2 }
	})
	self:ChangeType(TYPE_NUMBER)
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, self:GetInputValue(1))
end

function Component:ChangeType(t)
	if(t == TYPE_NUMBER) then
		if(not isnum) then
			self:SetOutputs({
				{ "Out", TYPE_NUMBER, self:GetWidth()-2, self:GetHeight()/2 }
			})
		end
		isnum = true
		self:SetToolTip("Buffer\n[Number]")
	elseif(t == TYPE_STRING) then
		if(isnum) then
			self:SetOutputs({
				{ "Out", TYPE_STRING, self:GetWidth()-2, self:GetHeight()/2 }
			})
		end
		isnum = false
		self:SetToolTip("Buffer\n[String]")
	end
end

function Component:onBuildEdit()
	
	self:AddUICheck("check_advsteps", "Multi-step (Not yet implemented)", multistep)
	self:AddUINumber("num_advsteps", "Steps", multistepnum)
	
	self:AddUISeparator()
	
	self:AddUIRadio("radio_num", "Is Number", "type", isnum)
	self:AddUIRadio("radio_str", "Is String", "type", not isnum)
	
	return true
end

function Component:onEditSubmit(settings)
	multistep = settings["check_advsteps"]
	multistepnum = math.floor(settings["num_advsteps"])
	if(multistepnum < 1) then
		multistepnum = 1
	end
	
	if(settings["radio_num"]) then
		self:ChangeType(TYPE_NUMBER)
	elseif(settings["radio_str"]) then
		self:ChangeType(TYPE_STRING)
	end
end

function Component:onGameSave()
	local data = {}
	
	if(isnum) then
		data["type"] = TYPE_NUMBER
	else
		data["type"] = TYPE_STRING
	end
	
	data["b_multistep"] = multistep
	data["n_multistep"] = multistepnum
	
	return data
end

function Component:onGameLoad(data)
	self:ChangeType(data["type"])
	multistep = data["b_multistep"]
	multistepnum = data["n_multistep"]
end

