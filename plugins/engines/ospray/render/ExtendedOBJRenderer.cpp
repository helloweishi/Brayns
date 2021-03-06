/* Copyright (c) 2015-2016, EPFL/Blue Brain Project
 * All rights reserved. Do not distribute without permission.
 * Responsible Author: Cyrille Favreau <cyrille.favreau@epfl.ch>
 *
 * Based on OSPRay implementation
 *
 * This file is part of Brayns <https://github.com/BlueBrain/Brayns>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

// obj
#include "ExtendedOBJRenderer.h"
#include "ExtendedOBJMaterial.h"
// ospray
#include <ospray/common/Model.h>
#include <ospray/common/Data.h>
#include <ospray/camera/Camera.h>
#include <ospray/lights/Light.h>
//embree
#include <embree2/rtcore.h>
//sys
#include <vector>
// ispc exports
#include "ExtendedOBJRenderer_ispc.h"

#define OSP_REGISTER_EXRENDERER( InternalClassName, external_name )\
extern "C" ospray::Renderer *ospray_create_renderer__##external_name( )\
{\
    return new InternalClassName;\
}

namespace brayns
{

void ExtendedOBJRenderer::commit( )
{
    Renderer::commit();

    lightData = ( ospray::Data* )getParamData( "lights" );
    lightArray.clear( );

    if( lightData )
        for( size_t i=0; i < lightData->size( ); ++i )
            lightArray.push_back(
                ( ( ospray::Light** )lightData->data )[ i ]->getIE( ));

    void **lightArrayPtr = lightArray.empty() ? nullptr : &lightArray[0];

    ospray::vec3f bgColor = getParam3f( "bgColor", ospray::vec3f( 1.f ));

    shadowsEnabled = bool( getParam1i( "shadowsEnabled", 1 ));

    softShadowsEnabled = bool(getParam1i( "softShadowsEnabled", 1 ));
    ambientOcclusionStrength = getParam1f( "ambientOcclusionStrength", 0.f );
    shadingEnabled = bool( getParam1i( "shadingEnabled", 1 ));
    randomNumber = getParam1i( "randomNumber", 0 );
    moving = bool( getParam1i( "moving", 0 ));
    timestamp = getParam1f( "timestamp", 0.f );
    spp = getParam1i("spp", 1);
    electronShadingEnabled = bool( getParam1i( "electronShading", 0 ));

    // Those materials are used for skybox mapping only
    materialData = ( ospray::Data* )getParamData( "materials" );
    materialArray.clear( );
    if( materialData )
        for( size_t i = 0; i < materialData->size( ); ++i )
            materialArray.push_back(
                ( ( ospray::Material** )materialData->data )[i]->getIE( ));
    void **materialArrayPtr =
        materialArray.empty( ) ? nullptr : &materialArray[0];

    ispc::ExtendedOBJRenderer_set(
                getIE( ),
                ( ispc::vec3f& )bgColor,
                shadowsEnabled,
                softShadowsEnabled,
                ambientOcclusionStrength,
                shadingEnabled,
                randomNumber,
                moving,
                timestamp,
                spp,
                electronShadingEnabled,
                lightArrayPtr, lightArray.size( ),
                materialArrayPtr, materialArray.size( ));
}

ExtendedOBJRenderer::ExtendedOBJRenderer( )
{
    ispcEquivalent = ispc::ExtendedOBJRenderer_create( this );
}

/*! \brief create a material of given type */
ospray::Material *ExtendedOBJRenderer::createMaterial( const char * )
{
    ospray::Material *mat = new brayns::obj::ExtendedOBJMaterial;
    return mat;
}

OSP_REGISTER_EXRENDERER( ExtendedOBJRenderer, EXOBJ );
OSP_REGISTER_EXRENDERER( ExtendedOBJRenderer, exobj );
} // ::brayns
