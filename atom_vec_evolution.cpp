/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   https://www.lammps.org/, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#include "atom_vec_evolution.h"

#include "atom.h"

#include <cmath>

using namespace LAMMPS_NS;

/* ---------------------------------------------------------------------- */

AtomVecEvolution::AtomVecEvolution(LAMMPS *lmp) : AtomVec(lmp)
{
  molecular = Atom::ATOMIC;
  mass_type = PER_TYPE;

  atom->q_flag = atom->mu_flag = 1;

  // strings with peratom variables to include in each AtomVec method
  // strings cannot contain fields in corresponding AtomVec default strings
  // order of fields in a string does not matter
  // except: fields_data_atom & fields_data_vel must match data file

  fields_grow = {"q", "mu", "phi", "q_reward", "f_active"};
  fields_copy = {"q", "mu", "phi", "q_reward","f_active"};
  fields_comm = {"mu3"};
  fields_comm_vel = {"mu3"};
  fields_border = {"q", "mu", "phi", "q_reward","f_active"};
  fields_border_vel = {"q", "mu", "phi", "q_reward","f_active"};
  fields_exchange = {"q", "mu", "phi", "q_reward","f_active"};
  fields_restart = {"q", "mu", "phi", "q_reward","f_active"};
  fields_create = {"q", "mu", "phi", "q_reward","f_active"};
  fields_data_atom = {"id", "type", "q", "x", "mu3"};
  fields_data_vel = {"id", "v"};

  setup_fields();
}

/* ----------------------------------------------------------------------
   set local copies of all grow ptrs used by this class, except defaults
   needed in replicate when 2 atom classes exist and it calls pack_restart()
------------------------------------------------------------------------- */

void AtomVecEvolution::grow_pointers()
{
  mu = atom->mu;
  phi = atom->phi;
  q_reward = atom->q_reward;
  f_active = atom->f_active;
}

/* ----------------------------------------------------------------------
   modify what AtomVec::data_atom() just unpacked
   or initialize other atom quantities
------------------------------------------------------------------------- */

void AtomVecEvolution::data_atom_post(int ilocal)
{
  double *mu_one = mu[ilocal];
  mu_one[3] = sqrt(mu_one[0] * mu_one[0] + mu_one[1] * mu_one[1] + mu_one[2] * mu_one[2]);
}
