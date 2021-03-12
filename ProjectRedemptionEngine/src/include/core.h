#pragma once
// core application
#include <core/preapplication.h>
#include <core/preapplicationconfig.h>

// subsystems

// input
#include <core/subsystems/input/preinputconfig.h>
#include <core/subsystems/input/preinput.h>
#include <core/subsystems/input/prekeycode.h>

// rendering
#include <core/subsystems/rendering/prerenderingconfig.h>
#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/rendering/prerendertexture.h>
#include <core/subsystems/rendering/preshader.h>
#include <core/subsystems/rendering/premesh.h>
#include <core/subsystems/rendering/preboneconfig.h>
#include <core/subsystems/rendering/preskeleton.h>
#include <core/subsystems/rendering/pretexture.h>
#include <core/subsystems/rendering/prematerial.h>
#include <core/subsystems/rendering/preskybox.h>

// animation
#include <core/subsystems/rendering/preanimationchannelconfig.h>
#include <core/subsystems/rendering/preanimationconfig.h>
#include <core/subsystems/rendering/preanimation.h>
#include <core/subsystems/rendering/preanimatorconfig.h>
#include <core/subsystems/rendering/preanimator.h>

// time
#include <core/subsystems/time/pretimeconfig.h>
#include <core/subsystems/time/pretime.h>

// world
#include <core/subsystems/world/preworldconfig.h>
#include <core/subsystems/world/preworld.h>
#include <core/subsystems/world/pregameobjecttemplate.h>
#include <core/subsystems/world/pregameobjectcomponent.h>

// asset
#include <core/subsystems/asset/preassetmanagerconfig.h>
#include <core/subsystems/asset/preassetmanager.h>

// components
#include <core/components/precameracomponent.h>
#include <core/components/premodelrenderercomponent.h>
#include <core/components/preanimatorcomponent.h>
#include <core/components/pretransformcomponent.h>