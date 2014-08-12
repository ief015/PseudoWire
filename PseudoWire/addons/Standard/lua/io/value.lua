local outnum = 0
local outstr = ""
local isnum = true

function Component:onSpawn()
	self:SetImage("io_value_num.png")
	self:SetOutputs({
		{ "Value", TYPE_NUMBER, self:GetWidth()-2, self:GetHeight()/2 }
	})
	self:SetToolTip("Constant Value")
end

function Component:ChangeType(t)
	if(t == TYPE_NUMBER) then
		if(not isnum) then
			self:SetOutputs({
				{ "Value", TYPE_NUMBER, self:GetWidth()-2, self:GetHeight()/2 }
			})
			self:SetImage("io_value_num.png")
		end
		isnum = true
	elseif(t == TYPE_STRING) then
		if(isnum) then
			self:SetOutputs({
				{ "Value", TYPE_STRING, self:GetWidth()-2, self:GetHeight()/2 }
			})
			self:SetImage("io_value_str.png")
		end
		isnum = false
	end
end

function Component:onBuildEdit()
	self:AddUIRadio("radio_num", "Is Number", "type", isnum)
	self:AddUINumber("text_num", "Number Value", outnum)
	
	self:AddUISeparator()
	
	self:AddUIRadio("radio_str", "Is String", "type", not isnum)
	self:AddUIString("text_str", "String Value", true, outstr, 3)
	
	return true
end

function Component:onEditSubmit(settings)
	outnum = settings["text_num"]
	outstr = settings["text_str"]

	if(settings["radio_num"]) then
		self:ChangeType(TYPE_NUMBER)
		self:SetToolTip("Value [Number]\n" .. outnum)
		self:TriggerOutput(1, outnum)
	elseif(settings["radio_str"]) then
		self:ChangeType(TYPE_STRING)
		self:SetToolTip("Value [String]\n" .. outstr)
		self:TriggerOutput(1, outstr)
	end
end

function Component:onGameSave()
	local data = {}
	
	if(isnum) then
		data["type"] = TYPE_NUMBER
	else
		data["type"] = TYPE_STRING
	end
	
	data["valuenum"] = outnum
	data["valuestr"] = outstr
	
	return data
end

function Component:onGameLoad(data)
	outnum = data["valuenum"]
	outstr = data["valuestr"]
	
	self:ChangeType(data["type"])
	
	if(isnum) then
		self:SetToolTip("Value [Number]\n" .. outnum)
		self:TriggerOutput(1, outnum)
	else
		self:SetToolTip("Value [String]\n" .. outstr)
		self:TriggerOutput(1, outstr)
	end
end
