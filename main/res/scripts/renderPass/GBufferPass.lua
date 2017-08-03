GBufferPass = {};

GBufferPass.Properties = {

	Shaders = {
			vertex = "Shaders/GBuffer.vert",
			frag = "Shaders/GBuffer.frag"
		},

	OutFrags = { "_normal_shininess" ,"_diffuse_specular" ,"_emissive_materailMask" },

	Uniforms = { { "_shininess", "int"} ,{ "_diffuseColor", "float3"},{ "_emissiveColor", "float3"},{ "_diffuseTexture", "texture"},{ "_emissiveTexture", "texture"}},

	beforeRender = true,
	stepRender = true;
};

function GBufferPass.Properties.Uniforms:_shininess(material)
	return material:getShininess() / 128.0;
end

function GBufferPass.Properties.Uniforms:_diffuseColor(context)
	return material:getShininess() / 128.0;
end

function GBufferPass.Properties.Uniforms:_emissiveColor(context)
	return material:getShininess() / 128.0;
end

function GBufferPass.Properties.Uniforms:_diffuseTexture(context)
	return material:getShininess() / 128.0;
end

function GBufferPass.Properties.Uniforms:_emissiveTexture(context)
	return material:getShininess() / 128.0;
end

function GBufferPass:createRenderTarget(context)
	local frameBuffer = open_lua.createFrameBuffer();
	local gbuffer = context.gbuffer;
	gbuffer.depthTexture = open_lua.createTexture();
	gbuffer.normal_shininessTexture = open_lua.createTexture();
	gbuffer.diffuse_specularTexture = open_lua.createTexture();
	gbuffer.emissive_materialTexture = open_lua.createTexture();

	frameBuffer:attach(DEPTH_ATTACHMENT, gbuffer.depthTexture);
	frameBuffer:attach(COLOR_ATTACHMENT0, gbuffer.normal_shininessTexture);
	frameBuffer:attach(COLOR_ATTACHMENT1, gbuffer.diffuse_specularTexture);
	frameBuffer:attach(COLOR_ATTACHMENT2, gbuffer.emissive_materialTexture);
end