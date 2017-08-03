%module open_lua
%{
#include <open/openDef.h>
#include <open/Maths.h>
#include <RenderSystem/RenderEnumeration.h>
#include <RenderSystem/BufferInterface.h>
#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>

#include <Render/DrawableInterface.h>
#include <Render/MaterialInterface.h>
#include <Render/RenderFactoryInterface.h>

#include <Engine/EngineInterface.h>

#include "factory.h"

using namespace open;
%}

%include <carrays.i>

%include "../../inc/open/openDef.h"
%include "maths.i"

%ignore RenderWindowCreator;
%ignore registeRenderWindowCreator;
%ignore InputListener;
%ignore FrameListener;
%ignore dispose();
%ignore accept();

%include "../../inc/open/interface/RenderSystem/RenderEnumeration.h"
%include "../../inc/open/interface/RenderSystem/BufferInterface.h"
%include "../../inc/open/interface/RenderSystem/RenderStatesInterface.h"
%include "../../inc/open/interface/RenderSystem/RenderInterface.h"


//%include "../../inc/open/interface/Render/DrawableInterface.h"
%include "../../inc/open/interface/Render/MaterialInterface.h"

%include "../../inc/open/interface/Engine/EngineInterface.h"


%include "factory.h"

%template(Vec3f) open::Vec3<float>;