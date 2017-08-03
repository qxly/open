require('open_lua')

vec = open_lua.Vec3f();
vec:setX(1)
print("vec.x", vec:x())

collectgarbage()


