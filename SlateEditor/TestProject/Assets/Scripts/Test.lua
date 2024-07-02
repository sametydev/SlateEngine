-- We are adding some logs for editor, in cooked versions, log:add writing on log.txt file
Log:Add("Hi from Lua!")

-- we are getting transform component from our connected entity, what is connectedEntity, its entity to attached our Lua Script
local transform = connectedEntity:GetComponent_Transform()
-- we are checking our entity has a transform component? always yes because all entities must be contain transform component xdd
local transformCheck = connectedEntity:HasComponent_Transform()

-- creating a vector 3
local v = vec3f(10.0,10.0,10.0)

-- testing stuffs
Log:Add("New Vec3 X Value : " .. tostring(v.x))
Log:Add("This entity has a transform component ? : " .. tostring(transformCheck))


-- OnUpdate function
function OnUpdate(deltaTime)
	-- changing our vectors x value with deltaTime
	v.x = v.x+100*deltaTime
	-- and finally we are change the our rotation! yippi!
	transform:SetRotation(v)
	-- Log:Add(tostring(InputSystem_MousePos.x))
end