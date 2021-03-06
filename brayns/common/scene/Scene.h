/* Copyright (c) 2015-2016, EPFL/Blue Brain Project
 * All rights reserved. Do not distribute without permission.
 * Responsible Author: Cyrille Favreau <cyrille.favreau@epfl.ch>
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

#ifndef SCENE_H
#define SCENE_H

#include <brayns/api.h>
#include <brayns/common/types.h>
#include <brayns/common/material/Texture2D.h>
#include <brayns/common/material/Material.h>
#include <brayns/common/geometry/Sphere.h>
#include <brayns/common/geometry/Cylinder.h>
#include <brayns/common/geometry/Cone.h>
#include <brayns/common/geometry/TrianglesMesh.h>

namespace brayns
{

/**

   Scene object

   This object contains collections of geometries, materials and light sources
   that are used to describe the 3D scene to be rendered. Scene is the base
   class for rendering-engine-specific inherited scenes.
 */
class Scene
{
public:
    /**
        Creates a scene object responsible for handling geometry, materials and
        light sources.
        @param renderers Renderers to be used to render the scene
        @param sceneParameters Parameters defining how the scene is constructed
        @param geometryParameters Parameters defining how the geometry is
               constructed

        @todo The scene must not know about the renderer
              https://bbpteam.epfl.ch/project/issues/browse/VIZTM-574
    */
    BRAYNS_API Scene(
        Renderers renderers,
        SceneParameters& sceneParameters,
        GeometryParameters& geometryParameters);
    BRAYNS_API virtual ~Scene();

    BRAYNS_API virtual void commit() = 0;

    /**
        Creates the materials handled by the scene, and available to the
        scene geometry
        @param materialType Specifies the algorithm that is used to create
               the materials. For instance MT_RANDOM creates materials with
               random colors, transparency, reflection, and light emission
        @param nbMaterials The number of materials to create
    */
    BRAYNS_API virtual void setMaterials(
        MaterialType materialType,
        size_t nbMaterials);

    /**
        Returns the material object for a given index
        @return Material object
    */
    BRAYNS_API MaterialPtr getMaterial( size_t index );

    /**
        Commit materials to renderers
        @param updateOnly If true, materials are not recreated and textures are
               not reassigned
    */
    BRAYNS_API virtual void commitMaterials(
        const bool updateOnly = false ) = 0;

    /**
        Commit lights to renderers
    */
    BRAYNS_API virtual void commitLights() = 0;

    /**
        Converts scene geometry into rendering engine specific data structures
    */
    BRAYNS_API virtual void buildGeometry() = 0;

    /**
        Returns the bounding box for the whole scene
    */
    Boxf& getWorldBounds() { return _bounds; }
    const Boxf& getWorldBounds() const { return _bounds; }

    /**
        Build an environment in addition to the loaded data, and according to
        the geometry parameters (command line parameter --scene-environment).
    */
    BRAYNS_API void buildEnvironment();

    /**
        Attaches a light source to the scene
        @param light Object representing the light source
    */
    BRAYNS_API void addLight( LightPtr light );

    /**
        Gets a light source from the scene for a given index
        @return Pointer to light source
    */
    BRAYNS_API LightPtr getLight( const size_t index );

    /**
        Removes a light source from the scene for a given index
        @param light Light source to be removed
    */
    BRAYNS_API void removeLight( LightPtr light );

    /**
        Removes all light sources from the scene
    */
    BRAYNS_API void clearLights();

    /**
        Builds a default scene made of a Cornell box, a refelctive cube, and
        a transparent sphere
    */
    BRAYNS_API void buildDefault();

    /**
        Return true if the scene does not contain any geometry. False otherwise
    */
    BRAYNS_API bool isEmpty() { return _isEmpty; }

    BRAYNS_API GeometryParameters& getGeometryParameters() { return _geometryParameters; }
    BRAYNS_API SceneParameters& getSceneParameters() { return _sceneParameters; }
    BRAYNS_API PrimitivesMap& getPrimitives() { return _primitives; }
    BRAYNS_API Materials& getMaterials() { return _materials; }
    BRAYNS_API TexturesMap& getTextures() { return _textures; }
    BRAYNS_API TrianglesMeshMap& getTriangleMeshes() { return _trianglesMeshes; }

protected:
    // Parameters
    SceneParameters& _sceneParameters;
    GeometryParameters& _geometryParameters;
    Renderers _renderers;

    // Model
    PrimitivesMap _primitives;
    TrianglesMeshMap _trianglesMeshes;
    Materials _materials;
    TexturesMap _textures;
    Lights _lights;

    Boxf _bounds;
    bool _isEmpty;
};

}
#endif // SCENE_H
