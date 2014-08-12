local rand_type = 0 --[[
	0 Simple Random
	1 Clocked Random
	2 Simple Random (between min and max)
	3 Clocked Random (between min and max)
]]

local use_custom_seed = false
local custom_seed = 0

local min = 0
local max = 0

function Component:SetRandType(t)
	rand_type = t
	
	if(t == 0) then
		self:SetImage("math_rand.png")
		self:SetToolTip("Random\nSimple")
		self:SetInputs({})
	elseif(t == 1) then
		self:SetImage("math_randclk.png")
		self:SetInputs({
			{"Clk", TYPE_NUMBER, 2, self:GetHeight()/2}
		})
		self:SetToolTip("Random\nClocked")
		self:TriggerOutput(1, 0)
	elseif(t == 2) then
		self:SetImage("math_randint.png")
		self:SetInputs({
			{"Min", TYPE_NUMBER, 2, 12},
			{"Max", TYPE_NUMBER, 2, 20}
		})
		self:SetToolTip("Random\nMin/Max")
	elseif(t == 3) then
		self:SetImage("math_randintclk.png")
		self:SetInputs({
			{"Clk", TYPE_NUMBER, 2, 12},
			{"Min", TYPE_NUMBER, 2, 28},
			{"Max", TYPE_NUMBER, 2, 36}
		})
		self:SetToolTip("Random\nClocked Min/Max")
		self:TriggerOutput(1, 0)
	end
end

function Component:onSpawn()
	math.randomseed(os.time())
	self:SetImage("math_rand.png")
	self:SetToolTip("Random\nSimple")
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, self:GetHeight()/2}
	})
end

function Component:onBuildEdit()
	
	local clk = (rand_type == 1 or rand_type == 3)
	local minmax = (rand_type == 2 or rand_type == 3)
	self:AddUICheck("bClk", "Clocked", clk)
	self:AddUICheck("bMinMax", "Use Min/Max", minmax)
	
	self:AddUISeparator()
	
	self:AddUICheck("bCustomSeed", "Use Custom Seed", use_custom_seed)
	self:AddUINumber("nCustomSeed", "Seed", custom_seed)
	
	return true
end

function Component:onEditSubmit(settings)
	if(settings["bClk"] and settings["bMinMax"]) then
		if(rand_type ~= 3) then
			self:SetRandType(3)
		end
	elseif(not settings["bClk"] and settings["bMinMax"]) then
		if(rand_type ~= 2) then
			self:SetRandType(2)
		end
	elseif(settings["bClk"] and not settings["bMinMax"]) then
		if(rand_type ~= 1) then
			self:SetRandType(1)
		end
	elseif(not settings["bClk"] and not settings["bMinMax"]) then
		if(rand_type ~= 0) then
			self:SetRandType(0)
		end
	end
	
	use_custom_seed = settings["bCustomSeed"]
	if(use_custom_seed) then
		custom_seed = settings["nCustomSeed"]
		math.randomseed(custom_seed)
	else
		math.randomseed(os.time())
	end
end

function Component:onGameSave()
	local data = {}
	
	data.bseed = use_custom_seed
	data.nseed = custom_seed
	
	data.randtype = rand_type
	
	return data
end

function Component:onGameLoad(data)
	self:SetRandType(data.randtype)
	
	use_custom_seed = data.bseed
	if(use_custom_seed) then
		custom_seed = data.nseed
		math.randomseed(custom_seed)
	else
		math.randomseed(os.time())
	end
end

function Component:onThink(dt)
	if(rand_type == 0) then
		self:TriggerOutput(1, math.random())
	elseif(rand_type == 2) then
		if(max >= min) then
			self:TriggerOutput(1, math.random(min, max))
		end
	end
end

function Component:onInputsTriggered()
	if(rand_type == 1) then
		if(self:IsInputTriggered(1) and self:GetInputValue(1) ~= 0) then
			self:TriggerOutput(1, math.random())
		end
	elseif(rand_type == 2) then
		if(self:IsInputTriggered(1)) then	
			min = self:GetInputValue(1)
		end
		if(self:IsInputTriggered(2)) then	
			max = self:GetInputValue(2)
		end
	elseif(rand_type == 3) then
		if(self:IsInputTriggered(2)) then	
			min = self:GetInputValue(2)
		end
		if(self:IsInputTriggered(3)) then	
			max = self:GetInputValue(3)
		end
		if(self:IsInputTriggered(1) and self:GetInputValue(1) ~= 0) then
			if(max >= min) then
				self:TriggerOutput(1, math.random(min, max))
			end
		end
	end
end
