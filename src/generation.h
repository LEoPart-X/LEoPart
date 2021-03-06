// Copyright: (c) 2020 Chris Richardson and Jakob Maljaars
// This file is part of LEoPart-X, a particle-in-cell package for DOLFIN-X
// License: GNU Lesser GPL version 3 or any later version
// SPDX-License-Identifier:    LGPL-3.0-or-later

#include <Eigen/Dense>

#pragma once

namespace dolfinx
{
namespace mesh
{
class Mesh;
enum class CellType;
} // namespace mesh
} // namespace dolfinx

namespace leopart
{
namespace generation
{

/// Create a set of points, distributed pro rata to the cell volumes.
/// The total number of particles will approximately equal \p density particles.
/// @return tuple with point coordinates, cell indices
std::tuple<
    Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>,
    std::vector<int>>
mesh_fill(const dolfinx::mesh::Mesh& mesh, double density);

/// Create a set of n points at random positions within the cell.
/// @return
Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
random_reference(dolfinx::mesh::CellType celltype, int n);

/// Create a set of n points at random positions within the reference
/// tetrahedron
/// @return Array of R^3-coordinates
Eigen::Array<double, Eigen::Dynamic, 3, Eigen::RowMajor>
random_reference_tetrahedron(int n);

/// Create a set of n points at random positions within the reference
/// triangle
/// @return Array of R^2-coordinates
Eigen::Array<double, Eigen::Dynamic, 2, Eigen::RowMajor>
random_reference_triangle(int n);
} // namespace generation
}; // namespace leopart