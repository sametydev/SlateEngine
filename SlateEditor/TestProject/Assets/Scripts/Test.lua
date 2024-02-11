Log:Add("test")

local t = connectedEntity:GetComponent_Transform()

local v = vec3f(10.0,10.0,10.0)


Log:Add(tostring(v.x))

function OnUpdate(deltaTime)
	v.x = v.x+0.01
	t:SetRotation(v)
end