/* Copyright (c) 2015-2016, EPFL/Blue Brain Project
 * All rights reserved. Do not distribute without permission.
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

#pragma once

#include <brayns/common/types.h>
#include "ospray/geometry/Geometry.h"

namespace brayns
{
struct ExtendedCylinders : public ospray::Geometry
{
    std::string toString() const final { return "ospray::Cylinders"; }
    void finalize(ospray::Model *model) final;

    float radius;
    int32 materialID;

    size_t numExtendedCylinders;
    size_t bytesPerCylinder;
    int64 offset_v0;
    int64 offset_v1;
    int64 offset_radius;
    int64 offset_timestamp;
    int64 offset_value;
    int64 offset_materialID;

    ospray::Ref<ospray::Data> data;

    ExtendedCylinders();
};

} // ::brayns
