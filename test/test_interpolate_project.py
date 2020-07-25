import dolfinx
from mpi4py import MPI
import numpy as np
import pyleopart


def test_interpolate_project():
    npart = 20
    mesh = dolfinx.UnitSquareMesh(MPI.COMM_WORLD, 5, 5)
    ncells = mesh.topology.index_map(2).size_local
    x, c = pyleopart.mesh_fill(mesh, ncells*npart)
    p = pyleopart.Particles(x, c)

    Q = dolfinx.function.VectorFunctionSpace(mesh, ("DG", 2))
    pbasis = pyleopart.get_particle_contributions(p, Q._cpp_object)

    def sq_val(x):
        return [x[0]**2, x[1]**2]

    u = dolfinx.function.Function(Q)
    u.interpolate(sq_val)

    p.add_field("v", [2])
    v = p.field("v")

    # Transfer from Function "u" to field "v"
    pyleopart.transfer_to_particles(p, v, u._cpp_object, pbasis)

    # Compare fields "w" and "x"(squared)
    for pidx in range(len(x)):
        assert np.isclose(p.field("v").data(pidx), p.field("x").data(pidx)**2).all()

    # Transfer from field "v" back to Function "u"
    pyleopart.transfer_to_function(u._cpp_object, p, v, pbasis)

    p.add_field("w", [2])
    w = p.field("w")
    # Transfer from Function "u" to field "w"
    pyleopart.transfer_to_particles(p, w, u._cpp_object, pbasis)

    # Compare fields "w" and "x"(squared)
    for pidx in range(len(x)):
        assert np.isclose(p.field("w").data(pidx), p.field("x").data(pidx)**2).all()
